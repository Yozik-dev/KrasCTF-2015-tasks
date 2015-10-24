#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stddef.h>
#include <sys/ptrace.h>
#include <sys/wait.h>

#define BUF_LIMIT_SIZE 128

#define INIT_HANDLERS_COUNT   3
#define SIGNAL_HANDLERS_COUNT 3
#define DATA_WRITERS_COUNT    1
#define CHECKERS_COUNT        2

#define BASE_INIT_INDX  102

#define IO_HANDLER_ID    0
#define USR1_HANDLER_ID  1
#define USR2_HANDLER_ID  2
#define STDOUT_WRITER_ID 0
#define MAIN_CHECKER_ID  0
#define FAKE_CHECKER_ID  1

#define LIBRARY_CODE_SECTION "__libc_freeres_fn"

#define __init(x) __attribute__((constructor(x)))
#define __to_section(x) __attribute__((section(x)))

struct encoded_string;

typedef int (*init_handler_t)(void);
typedef void (*signal_handler_t)(int);
typedef void (*writer_t)(int, struct encoded_string *);
typedef int (*password_checker_t)(const char *);

typedef struct encoded_string {
	uint64_t *enc_data;
	int enc_step;
	int enc_start;
	const unsigned char enc_key;
	const size_t enc_len;
} encoded_string_t;

typedef struct call_table {
	unsigned char data_0[0x7];
	init_handler_t init_functions[INIT_HANDLERS_COUNT];
	unsigned char data_1[0x10];
	signal_handler_t signal_handlers[SIGNAL_HANDLERS_COUNT];
	unsigned char data_2[0x10];
	password_checker_t checkers[CHECKERS_COUNT];
	unsigned char data_3[0xE];
	writer_t writers[DATA_WRITERS_COUNT];
	unsigned char data_4[0x6];
} call_table_t;

static int set_usr1_handler();
static int set_usr2_handler();
static int set_io_handler();

static void io_signal_handler(int);
static void usr1_signal_handler(int);
static void usr2_signal_handler(int);

static void decode_and_write(int fd, encoded_string_t *message);
static int process_user_password(const char *password);
static int fake_password_checker(const char *password);

static call_table_t call_table = {
	.data_0 = {0xde, 0x3b, 0x70, 0x8, 0x5d, 0xa1, 0xfc},
	.init_functions = {
		set_io_handler,
		set_usr1_handler,
		set_usr2_handler
	},
	.data_1 = {
		0x67, 0xdc, 0xf8, 0x08, 0x04, 0x25, 0x25, 0xca,
		0xe6, 0x11, 0x85, 0xbf, 0x08, 0x04, 0xd1, 0xb4
	},
	.signal_handlers = {
		io_signal_handler,
		usr1_signal_handler,
		usr2_signal_handler
	},
	.data_2 = {
		0x2a, 0x22, 0x08, 0xa7, 0x73, 0x92, 0x08, 0x99,
		0x5b, 0x08, 0xcb, 0xca, 0x04, 0x6b, 0x08, 0x4e
	},
	.writers = { decode_and_write },
	.data_3 = {
		0xcb, 0xa4, 0x08, 0xd0, 0x00, 0xa8, 0x97, 0xc6,
		0x08, 0x4c, 0xda, 0x0c, 0x04, 0xda
	},
	.checkers = {
		process_user_password,
		fake_password_checker
	},
	.data_4 = {
		0x3a, 0xfe, 0x08, 0x60, 0x4d, 0x51
	}
};

#define enc_str_set_to_default(x)   \
	x.enc_data  = NULL;	    \
	x.enc_step  = 0;	    \
	x.enc_start = 0;

static encoded_string_t success_message = {
	.enc_data = NULL,
	.enc_step = 0,
	.enc_start = 0,
	.enc_key = 0x48,
	.enc_len = 46
};

static encoded_string_t failed_message = {
	.enc_data = NULL,
	.enc_step = 0,
	.enc_start = 0,
	.enc_key = 0xf1,
	.enc_len = 19
};

static encoded_string_t encrypted_key = {
	.enc_data = NULL,
	.enc_step = 0,
	.enc_start = 0,
	.enc_key = 0x53,
	.enc_len = 32
};

static uint64_t message_buffer[] = {
	0xc4dbf9a6e3b8931e,
	0x617f5d0b5a0120a7,
	0xd4dcc5c5e291db20,
	0x60675c2f1b2c219b,
	0x9e9cf288e391d128,
	0x000000000056658e,
	0xd2dbf992b096d935,
	0x0000000000000000,
	0xd3c1e2c5b084986c,
	0x0000000000000000,
	0x00009c82a289de65,
	/* encrypted key */
	0x9b81e9c9becbc66a,
	0x8c30af51f027ad3e,
	0x9a82e8c9b9ce9a38,
	0x62be08a700ea87a1,
	0x9b85b59bee98913b,
	0xc850ab8b3f01ed80,
	0x94d0bfc7ed989166
};

__attribute__((always_inline))
static inline int decode_from_ll(uint64_t src, char *dst,
				 unsigned char xor_key)
{
	int i;
	for (i = 0; i < sizeof(uint64_t); ++i)
		dst[i] = ((src >> (i * 8)) & 0xFF) ^ xor_key;
	return i;
}

__attribute__((always_inline))
static inline int decode_from_ll_partialy(uint64_t src, char *dst,
					  int bytes_to_copy, unsigned char xor_key)
{
	int i;
	for (i = 0; i < bytes_to_copy; ++i)
		dst[i] = ((src >> (i * 8)) & 0xFF) ^ xor_key;
	return i;
}

__attribute__((always_inline))
static inline init_handler_t init_handler_pointer(int indx)
{
	unsigned char *array_ptr  = (unsigned char *) &call_table;
	unsigned int in_array_off = offsetof(struct call_table, init_functions);
	init_handler_t *func  = (init_handler_t *) (array_ptr + in_array_off);
	return func[indx];
}

__attribute__((always_inline))
static inline signal_handler_t signal_handler_pointer(int indx)
{
	unsigned char *array_ptr  = (unsigned char *) &call_table;
	unsigned int in_array_off = offsetof(struct call_table, signal_handlers);
	signal_handler_t *func = (signal_handler_t *) (array_ptr + in_array_off);
	return func[indx];
}

__attribute__((always_inline))
static inline writer_t writer_function(int indx)
{
	unsigned char *array_ptr  = (unsigned char *) &call_table;
	unsigned int in_array_off = offsetof(struct call_table, writers);
	writer_t *writers = (writer_t *) (array_ptr + in_array_off);
	return writers[indx];
}

__attribute__((always_inline))
static inline password_checker_t password_checker(int indx)
{
	unsigned char *array_ptr  = (unsigned char *) &call_table;
	unsigned int in_array_off = offsetof(struct call_table, checkers);
	password_checker_t  *checkers = (password_checker_t *)(array_ptr + in_array_off);
	return checkers[indx];
}

__attribute__((always_inline))
static inline void reset_password_checker(int old_indx, int new_indx)
{
	unsigned char *array_ptr  = (unsigned char *) &call_table;
	unsigned int in_array_off = offsetof(struct call_table, checkers);
	password_checker_t *checkers = (password_checker_t *)(array_ptr + in_array_off);
	checkers[old_indx] = checkers[new_indx];
}

__attribute__((always_inline))
static inline void chop(char *text)
{
	int text_len = strlen(text);
	if (text[text_len - 1] == '\n')
		text[text_len - 1] = '\0';
}

__attribute__((always_inline))
static inline uint64_t master_key_decode(uint64_t curnt_message_part)
{
	uint64_t master_key = 0xFEE1DEAD8BADF00D;
	uint64_t decoded = 0;
	for (int i = 0; i < 8 && curnt_message_part; ++i) {
		decoded |= ((curnt_message_part & 0xFF) ^
			    ((master_key >> (i * 8)) & 0xFF)) << (i * 8);
		curnt_message_part >>= 8;
	}
	return decoded;
}

__attribute__((always_inline))
static inline int am_i_being_traced()
{
	int ch_exit_code;
	if (fork() == 0) {
		pid_t parent = getppid();
		if (ptrace(PTRACE_ATTACH, parent, NULL, NULL) == -1)
			_exit(EXIT_FAILURE);
		waitpid(parent, NULL, 0);
		ptrace(PTRACE_CONT, parent, NULL, NULL);
		ptrace(PTRACE_DETACH, parent, NULL, NULL);
		_exit(EXIT_SUCCESS);
	}
	wait(&ch_exit_code);
	return WEXITSTATUS(ch_exit_code) == EXIT_SUCCESS ? 0 : 1;
}

__attribute__((always_inline))
static inline char *decode_string(encoded_string_t *message)
{
	int indx = message->enc_start;
	unsigned long long curnt_message_part = 0;
	char *clear_message = malloc(message->enc_len);
	if (clear_message == NULL)
		return NULL;

	for (int i = 0; i < message->enc_len;) {
		curnt_message_part = master_key_decode(message->enc_data[indx]);
		if (message->enc_len - i >= sizeof(unsigned long long))
			i += decode_from_ll(curnt_message_part,
					    clear_message + i,
					    message->enc_key);
		else
			i += decode_from_ll_partialy(curnt_message_part,
						     clear_message + i,
						     message->enc_len - i,
						     message->enc_key);
		indx += message->enc_step;
	}

	return clear_message;
}

__to_section(LIBRARY_CODE_SECTION)
static void decode_and_write(int fd, encoded_string_t *message)
{
	char *clear_message = decode_string(message);
	write(fd, clear_message, message->enc_len);
	free(clear_message);
}

__attribute__((always_inline))
static inline uint32_t generate_opcode(uint32_t usr, uint32_t org)
{
	uint32_t bases[5] = { '9', 'f', 0xA8, 0xcf, 0x78 };
	long double fpu_res;

	__asm__(
		"finit;"
		"fild %7;" /* ST6 = 0x78       */
		"fild %6;" /* ST5 = 'a' + 0x82 */
		"fild %3;" /* ST4 = '9'        */
		"fild %4;" /* ST3 = 'f'        */
		"fild %5;" /* ST2 = '0' + 0x78 */
		"fild %1;" /* ST1 = usr        */
		"fild %2;" /* ST0 = org        */
		".intel_syntax noprefix;"

		"fcomi st(0), st(4);"
		"fcmovbe st(0), st(2);"
		"fcomi st(0), st(3);"
		"fcmovbe st(0), st(5);"
		"fsub st(0), st(6);"

		".att_syntax prefix;"
		"fild %2;" /* ST0 = org        */
		"fxch;"
		".intel_syntax noprefix;"
		"fsubp;"   /* STO = int(org)   */
		"fxch;"    /* ST0 = usr        */

		"fcomi st(0), st(4);"
		"fcmovbe st(0), st(2);"
		"fcomi st(0), st(3);"
		"fcmovbe st(0), st(5);"
		"fsub st(0), st(6);"

		".att_syntax prefix;"
		"fild %1;" /* ST0 = usr        */
		"fxch;"
		".intel_syntax noprefix;"
		"fsubp;"   /* STO = int(usr)   */

		"fcomi st(0), st(1);"
		"fcmovb st(0), st(6);"
		"fxch;"
		"fsubp;"

		".att_syntax prefix;"
		: "=t"(fpu_res)
		: "m"(usr), "m"(org),
		  "m"(bases[0]), "m"(bases[1]),
		  "m"(bases[2]), "m"(bases[3]), "m"(bases[4])
	);

	return (uint32_t) fpu_res;
}

__attribute__((always_inline))
static inline int machine_step(uint32_t opcode, char *tape,
				int step, size_t tape_len)
{
	int rc = 0;
	char xor_table[0x10] = {
		0x35, 0x0A, 0x00, 0x51, 0x23, 0x46, 0x40, 0x1E,
		0x49, 0x77, 0x55, 0x22, 0x60, 0x12, 0x74, 0x38
	};

	if (step > tape_len - 1)
		return 1;

	switch (opcode) {
	case 0 ... 0xF:
		tape[step] ^= xor_table[opcode];
		break;
	default:
		rc = 1;
		break;
	}

	return rc;
}

__attribute__((always_inline))
static inline int check_result(char *tape)
{
	uint32_t tape_target[8] = {
		0x53735034, 0x28336E29, 0x27797F53,
		0x14306F14, 0x76537B50, 0x76717850,
		0x64533832, 0x1A627B7F
	};

	uint32_t *tape_dwords = (uint32_t *) tape;

	for (int i = 0; i < 8; ++i)
		if (tape_dwords[i] != tape_target[i])
			return 1;
	return 0;
}

__to_section(LIBRARY_CODE_SECTION)
static int process_user_password(const char *password)
{
	uint32_t opcode;
	uint32_t next_byte;
	uint32_t key_byte;
	int rc = 0;

	encrypted_key.enc_data = message_buffer;
	encrypted_key.enc_step = 2;
	encrypted_key.enc_start = 11;
	char *plain_key = decode_string(&encrypted_key);
	if (plain_key == NULL)
		return 1;
	enc_str_set_to_default(encrypted_key);

	for (int i = 0; i < strlen(password); ++i) {
		switch ((next_byte = password[i])) {
		case '0' ... '9':
		case 'a' ... 'f':
			key_byte = plain_key[i % encrypted_key.enc_len];
			opcode = generate_opcode(next_byte, key_byte);
			if (machine_step(opcode, plain_key, i))
				rc = 1;
			break;
		default:
			break;
		}
	}

	if (rc == 0)
		rc = check_result(plain_key, encrypted_key.enc_len);
	free(plain_key);
	return rc;
}

__to_section(LIBRARY_CODE_SECTION)
static int fake_password_checker(const char *password)
{
	signal_handler_pointer(USR2_HANDLER_ID)(SIGUSR2);
	return 0;
}

__to_section(LIBRARY_CODE_SECTION)
static void io_signal_handler(int sig)
{
	char buf[BUF_LIMIT_SIZE] = {0};
	if (sig != SIGIO)
		return;
	memset(buf, 0, BUF_LIMIT_SIZE);
	if (read(STDIN_FILENO, buf, BUF_LIMIT_SIZE - 1) == -1)
		return;
	chop(buf);
	if (password_checker(MAIN_CHECKER_ID)(buf) == 0)
		raise(SIGUSR1);
	else
		raise(SIGUSR2);
}

__to_section(LIBRARY_CODE_SECTION)
static void usr1_signal_handler(int sig)
{
	success_message.enc_data = message_buffer;
	success_message.enc_step = 2;
	success_message.enc_start = 0;
	writer_function(STDOUT_WRITER_ID)(STDOUT_FILENO, &success_message);
	enc_str_set_to_default(success_message);
	_exit(EXIT_SUCCESS);
}

__to_section(LIBRARY_CODE_SECTION)
static void usr2_signal_handler(int sig)
{
	failed_message.enc_data = message_buffer;
	failed_message.enc_step = 2;
	failed_message.enc_start = 1;
	writer_function(STDOUT_WRITER_ID)(STDOUT_FILENO, &failed_message);
	enc_str_set_to_default(failed_message);
	_exit(EXIT_SUCCESS);
}

__to_section(LIBRARY_CODE_SECTION)
static int set_io_handler()
{
	struct sigaction sig_act;
	int fd_flags = 0;

	sigemptyset(&sig_act.sa_mask);
	sig_act.sa_flags = SA_RESTART;
	sig_act.sa_handler = signal_handler_pointer(IO_HANDLER_ID);
	if (sigaction(SIGIO, &sig_act, NULL) == -1)
		return -1;
	fcntl(STDIN_FILENO, F_SETOWN, getpid());
	fd_flags = fcntl(STDIN_FILENO, F_GETFL);
	fcntl(STDIN_FILENO, F_SETFL, fd_flags | O_ASYNC | O_NONBLOCK);
	return 0;
}

__to_section(LIBRARY_CODE_SECTION)
static int set_usr1_handler()
{
	struct sigaction sig_act;

	sigemptyset(&sig_act.sa_mask);
	sig_act.sa_flags = SA_RESTART;
	sig_act.sa_handler = signal_handler_pointer(USR1_HANDLER_ID);
	return sigaction(SIGUSR1, &sig_act, NULL);
}

__to_section(LIBRARY_CODE_SECTION)
static int set_usr2_handler()
{
	struct sigaction sig_act;

	sigemptyset(&sig_act.sa_mask);
	sig_act.sa_flags = SA_RESTART;
	sig_act.sa_handler = signal_handler_pointer(USR2_HANDLER_ID);
	return sigaction(SIGUSR2, &sig_act, NULL);
}

__to_section(LIBRARY_CODE_SECTION)
__init(BASE_INIT_INDX + IO_HANDLER_ID) static void init_io_handlers()
{
	if (am_i_being_traced())
		reset_password_checker(MAIN_CHECKER_ID, FAKE_CHECKER_ID);
	init_handler_pointer(IO_HANDLER_ID)();
}

__to_section(LIBRARY_CODE_SECTION)
__init(BASE_INIT_INDX + USR1_HANDLER_ID) static void init_usr1_handler()
{
	if (!am_i_being_traced())
		init_handler_pointer(USR1_HANDLER_ID)();
}

__to_section(LIBRARY_CODE_SECTION)
__init(BASE_INIT_INDX + USR2_HANDLER_ID) static void init_usr2_handler()
{
	if (!am_i_being_traced())
		init_handler_pointer(USR2_HANDLER_ID)();
}

static int wait_term_signal()
{
	sigset_t wait_set;
	int sig_acpted;
	sigemptyset(&wait_set);
	sigaddset(&wait_set, SIGQUIT);
	sigwait(&wait_set, &sig_acpted);
	return sig_acpted;
}

int main(int argc, char **argv)
{
	write(STDERR_FILENO, "Enter password : ", 17);
	wait_term_signal();
	return 0;
}

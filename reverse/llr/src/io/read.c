#include <common.h>

#include <asm/io/display.h>
#include <io.h>
#include <bool.h>

#define CURSOR_SYMBOL 176

static inline void put_cursor_symbol(void)
{
	putc(CURSOR_SYMBOL);
	putc('\b');
}

static inline void del_cursor_symbol(void)
{
	putc(' ');
	putc('\b');
}

static inline void del_prev_char(void)
{
	del_cursor_symbol();
	putc('\b');
	putc(' ');
	putc('\b');
}

static inline char filter_ascii_code(char ascii_code)
{
	if (ascii_code >= 0x20 && ascii_code <= 0x7E)
		return ascii_code;

	switch (ascii_code) {
	case '\n':
	case '\r':
	case '\b':
		return ascii_code;
	default:
		return 0;
	}
}

char getc(void)
{
	return filter_ascii_code(arch_getc() & 0xFF);
}

int read(char *buffer, size_t len)
{
	int read_cnt = 0;
	char current = 0;

	for (;;) {
		put_cursor_symbol();
		switch ((current = getc())) {
		case 0:
			continue;
		case '\n':
		case '\r':
			del_cursor_symbol();
			return read_cnt;
		case '\b':
			if (read_cnt > 0) {
				read_cnt -= 1;
				buffer[read_cnt] = 0;
				del_prev_char();
			}
			continue;
		default:
			if (read_cnt >= len)
				continue;
			putc(current);
			buffer[read_cnt++] = current;
		}
	}
}

#ifndef COMMAND_H
#define COMMAND_H

#define FILES_CNT    1
#define COMMANDS_CNT 3

typedef struct {
	const char *filename;
	const char *content;
} file_t;

typedef struct {
	const char *cmdname;
	int (*handler)(file_t *);
} command_t;

#ifndef UNKNOWN_COMMAND
#define UNKNOWN_COMMAND "uknown command\n"
#endif

int command_handler(const char *command);

#endif

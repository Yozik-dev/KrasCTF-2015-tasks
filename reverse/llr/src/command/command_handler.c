#include <common.h>
#include <command.h>
#include <string.h>
#include <io.h>
#include <flag.h>

static int cat_cmd_handler(file_t *files)
{
	int i = 0;
	const char *filename = strtok(NULL, " ");
	if (filename == NULL) {
		error("need a file name to read\n");
		return 0;
	}
	for (i = 0; i < FILES_CNT; ++i) {
		if (strcmp(files[i].filename, filename) == 0) {
			print(files[i].content);
			break;
		}
	}
	if (i == FILES_CNT)
		error("wrong file name\n");
	else
		print("\n");
	return 0;
}

static int ls_cmd_handler(file_t *files)
{
	int i = 0;
	int total_names_len = 0;

	for (i = 0; i < FILES_CNT; ++i) {
		total_names_len += strlen(files[i].filename);
		if (total_names_len > 60) {
			print("\n");
			total_names_len = 0;
		}
		print(files[i].filename);
		print(" ");
	}
	print("\n");
	return 0;
}

static int flag_cmd_handler(file_t *files)
{
	if (try_get_flag() == 0) {
		set_text_color(GREEN_TEXT_COLOR);
		print("Well done! U can get ur flag by typing "
		      "the following in ur ipython console :\n");
		set_text_color(LIGHT_RED_TEXT_COLOR);
		print("from hashlib import md5\n");
		print("print reduce(lambda x, y: "
		      "md5(x).hexdigest(), xrange(100000), password)\n");
	} else {
		set_text_color(RED_TEXT_COLOR);
		print("Sorry!\n");
	}
	set_text_color(DEFAULT_TEXT_COLOR);
	return 0;
}

static int try_to_find_cmd(const char *cmd)
{
	int i = 0;

	command_t commands[COMMANDS_CNT] = {
		{
			.cmdname = "cat",
			.handler = cat_cmd_handler
		},
		{
			.cmdname = "ls",
			.handler = ls_cmd_handler
		},
		{
			.cmdname = "flag",
			.handler = flag_cmd_handler
		}
	};

	file_t files[FILES_CNT] = {
		{
			.filename = "help",
			.content  =
			"cat filename - read the file content\n"
			"ls           - read the directory content\n"
			"flag         - get the flag"
		}
	};

	for (i = 0; i < COMMANDS_CNT; ++i)
		if (strcmp(cmd, commands[i].cmdname) == 0)
			return commands[i].handler(files);
	return -1;
}

int command_handler(const char *command)
{
	char cmdbuf[MAX_STRING_SIZE];
	char *token = NULL;

	if (strlen(command) == 0)
		return 0;
	memset(cmdbuf, 0, sizeof(cmdbuf));
	strncpy(cmdbuf, command, sizeof(cmdbuf));
	token = strtok(cmdbuf, " ");
	if (token != NULL)
		return try_to_find_cmd(token);
	return -1;
}

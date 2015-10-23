#include <common.h>

#include <io.h>
#include <system.h>
#include <display.h>
#include <string.h>
#include <command.h>

static void draw_logo(void)
{
	const char *ascii_logo =
		"   \t\t\t  __   _______________________________    \n"
		"   \t\t\t |  | _\\_   ___ \\__    ___/\\_   _____/ \n"
		"   \t\t\t |  |/ /    \\  \\/ |    |    |    __)    \n"
		"   \t\t\t |    <\\     \\____|    |    |     \\    \n"
		"   \t\t\t |__|_ \\\\______  /|____|    \\___  /    \n"
		"   \t\t\t      \\/       \\/               \\/   \n\n";
	set_text_color(CYAN_TEXT_COLOR);
	print(ascii_logo);
	set_text_color(DEFAULT_TEXT_COLOR);
}

void start(void)
{
	char buffer[MAX_STRING_SIZE];
	if (init_display(DEFAULT_VIDEO_MODE) != 0)
		error("Failed to set display mode!");

	draw_logo();
	set_text_color(BOLD_WHITE_TEXT_COLOR);
	set_cursor_position(TASK_LABEL_CURSOR_COL, TASK_LABEL_CURSOR_ROW);
	print("Reverse engineering task LLR");
	set_cursor_position(DEFAULT_USER_CURSOR_COL, DEFAULT_USER_CURSOR_ROW);

	for (;;) {
		set_text_color(BOLD_WHITE_TEXT_COLOR);
		print("# ");
		set_text_color(DEFAULT_TEXT_COLOR);

		memset(buffer, 0, sizeof(buffer));
		read(buffer, sizeof(buffer) - 1);
		print("\n");
		if (command_handler(buffer) != 0)
			error(UNKNOWN_COMMAND);
	}

	halt();
}

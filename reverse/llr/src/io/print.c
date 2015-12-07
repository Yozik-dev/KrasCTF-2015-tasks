#include <common.h>

#include <asm/io/display.h>
#include <display.h>
#include <io.h>
#include <string.h>

static text_color_t text_color = DEFAULT_TEXT_COLOR;

void set_text_color(text_color_t color_to_set)
{
	text_color = color_to_set;
}

void putc(char byte_to_print)
{
	int i = 0;

	if (byte_to_print == '\t') {
		for (i = 0; i < DEFAULT_TAB_SIZE; ++i)
			arch_putc(' ', text_color);
		return;
	}

	arch_putc(byte_to_print, text_color);
	if (byte_to_print == '\n')
		arch_putc('\r', text_color);
}

static inline int count_new_line_symbols(const char *message)
{
	int counter = 0;

	while (*message != 0) {
		if (*message == '\r' || *message == '\n')
			++counter;
		++message;
	}

	return counter;
}

void print(const char *message)
{
	video_page_t video_page = {
		.top_left_col  = DEFAULT_USER_CURSOR_COL,
		.top_left_row  = DEFAULT_USER_CURSOR_ROW,
		.bot_right_col = MAX_STRING_SIZE,
		.bot_right_row = CONSOLE_HEIGHT,
		.shift_rows_once = 0
	};

	int new_line_symbols_cnt = count_new_line_symbols(message);
	int cursor_pos = get_cursor_position();
	int row_after_print = CURSOR_POS_ROW(cursor_pos) + new_line_symbols_cnt;
	int new_cursor_row = CURSOR_POS_ROW(cursor_pos) - new_line_symbols_cnt;

	if (row_after_print >= video_page.bot_right_row) {
		video_page.shift_rows_once = new_line_symbols_cnt;
		set_cursor_position(CURSOR_POS_COL(cursor_pos),
				    new_cursor_row);
		move_active_video_page_up(&video_page);
		video_page.shift_rows_once = 0;
	}

	while (*message != 0) {
		putc(*message);
		message++;
	}

	return;
}

void error(const char *err_msg)
{
	set_text_color(RED_TEXT_COLOR);
	print(err_msg);
	set_text_color(DEFAULT_TEXT_COLOR);
}

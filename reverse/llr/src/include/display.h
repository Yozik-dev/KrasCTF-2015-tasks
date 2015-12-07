#ifndef DISPLAY_H
#define DISPLAY_H

#ifndef DEFAULT_VIDEO_MODE
#define DEFAULT_VIDEO_MODE 0x102 /* 800 x 600 x 16 */
#endif

#ifndef DEFAULT_USER_CURSOR_COL
#define DEFAULT_USER_CURSOR_COL 0
#endif

#ifndef DEFAULT_USER_CURSOR_ROW
#define DEFAULT_USER_CURSOR_ROW 8
#endif

#ifndef TASK_LABEL_CURSOR_COL
#define TASK_LABEL_CURSOR_COL 33
#endif

#ifndef TASK_LABEL_CURSOR_ROW
#define TASK_LABEL_CURSOR_ROW 7
#endif

#ifndef CONSOLE_HEIGHT
#define CONSOLE_HEIGHT 36
#endif

#ifndef TEXT_COLOR_T_DEFINED
#define TEXT_COLOR_T_DEFINED

typedef enum {
	BLACK_TEXT_COLOR,
	BLUE_TEXT_COLOR,
	GREEN_TEXT_COLOR,
	CYAN_TEXT_COLOR,
	RED_TEXT_COLOR,
	PURPLE_TEXT_COLOR,
	ORANGE_TEXT_COLOR,
	WHITE_TEXT_COLOR,
	GRAY_TEXT_COLOR,
	DARK_BLUE_TEXT_COLOR,
	LIGHT_GREEN_TEXT_COLOR,
	LIGHT_BLUE_TEXT_COLOR,
	LIGHT_RED_TEXT_COLOR,
	LIGHT_PURPLE_TEXT_COLOR,
	LIGHT_YELLOW_TEXT_COLOR,
	BOLD_WHITE_TEXT_COLOR
} text_color_t;

#endif

#ifndef VIDEO_PAGE_STRUCTURE
#define VIDEO_PAGE_STRUCTURE

typedef struct video_page {
	unsigned char top_left_row;
	unsigned char top_left_col;
	unsigned char bot_right_row;
	unsigned char bot_right_col;
	unsigned char shift_rows_once;
} video_page_t;

#endif

#ifndef CURSOR_POS_ROW
#define CURSOR_POS_ROW(pos) (((pos) >> 8) & 0xFF)
#endif

#ifndef CURSOR_POS_COL
#define CURSOR_POS_COL(pos) ((pos) & 0xFF)
#endif

int init_display(short);
void set_cursor_position(int, int);
int get_cursor_position(void);
void move_active_video_page_up(const video_page_t *);

#endif

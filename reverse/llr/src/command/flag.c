#include <common.h>
#include <io.h>
#include <string.h>

#define MAZE_DIMEN_ROWS 9
#define MAZE_DIMEN_COLS 11

#define MAX_PASSWORD_LEN  0x30
#define ORIG_PASSWORD_LEN 0x20

#define MAZE_CMD_MV_LEFT  'A'
#define MAZE_CMD_MV_RIGHT 'I'
#define MAZE_CMD_MV_UP    'R'
#define MAZE_CMD_MV_DOWN  '_'

#define MAZE_ENTRY_ROW 0
#define MAZE_ENTRY_COL 0

#define MAZE_EXIT_ROW  8
#define MAZE_EXIT_COL  10

typedef struct location {
	int row;
	int col;
} location_t;

static inline void handle_cmd_left(location_t *maze_loc,
				   char maze[][MAZE_DIMEN_COLS])
{
	if (maze_loc->col <= 0 || maze[maze_loc->row][maze_loc->col - 1])
		return;
	maze[maze_loc->row][maze_loc->col] = 1;
	maze_loc->col -= 1;
}

static inline void handle_cmd_right(location_t *maze_loc,
				    char maze[][MAZE_DIMEN_COLS])
{
	if (maze_loc->col >= MAZE_DIMEN_COLS ||
	    maze[maze_loc->row][maze_loc->col + 1])
		return;
	maze[maze_loc->row][maze_loc->col] = 1;
	maze_loc->col += 1;
}

static inline void handle_cmd_up(location_t *maze_loc,
				 char maze[][MAZE_DIMEN_COLS])
{
	if (maze_loc->row <= 0 || maze[maze_loc->row - 1][maze_loc->col])
		return;
	maze[maze_loc->row][maze_loc->col] = 1;
	maze_loc->row -= 1;
}

static inline void handle_cmd_down(location_t *maze_loc,
				   char maze[][MAZE_DIMEN_COLS])
{
	if (maze_loc->row >= MAZE_DIMEN_ROWS ||
	    maze[maze_loc->row + 1][maze_loc->col])
		return;
	maze[maze_loc->row][maze_loc->col] = 1;
	maze_loc->row += 1;
}

static void maze_step(char cmd, location_t *maze_loc,
		      char maze[][MAZE_DIMEN_COLS])
{
	switch(cmd) {
	case MAZE_CMD_MV_LEFT:
		return handle_cmd_left(maze_loc, maze);
	case MAZE_CMD_MV_RIGHT:
		return handle_cmd_right(maze_loc, maze);
	case MAZE_CMD_MV_UP:
		return handle_cmd_up(maze_loc, maze);
	case MAZE_CMD_MV_DOWN:
		return handle_cmd_down(maze_loc, maze);
	}
}

static int check_the_password(const char *password)
{
	int i = 0, pass_len = strlen(password);
	location_t cur_loc = {MAZE_ENTRY_ROW, MAZE_ENTRY_COL};

	char maze_row_0[MAZE_DIMEN_COLS] = {0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0};
	char maze_row_1[MAZE_DIMEN_COLS] = {0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0};
	char maze_row_2[MAZE_DIMEN_COLS] = {0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0};
	char maze_row_3[MAZE_DIMEN_COLS] = {0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1};
	char maze_row_4[MAZE_DIMEN_COLS] = {0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0};
	char maze_row_5[MAZE_DIMEN_COLS] = {0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0};
	char maze_row_6[MAZE_DIMEN_COLS] = {0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0};
	char maze_row_7[MAZE_DIMEN_COLS] = {1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0};
	char maze_row_8[MAZE_DIMEN_COLS] = {0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0};

	char maze[MAZE_DIMEN_ROWS][MAZE_DIMEN_COLS] = {};
	memset(maze, 0, sizeof(maze));

	memcpy(&maze[0][0], maze_row_0, MAZE_DIMEN_COLS);
	memcpy(&maze[1][0], maze_row_1, MAZE_DIMEN_COLS);
	memcpy(&maze[2][0], maze_row_2, MAZE_DIMEN_COLS);
	memcpy(&maze[3][0], maze_row_3, MAZE_DIMEN_COLS);
	memcpy(&maze[4][0], maze_row_4, MAZE_DIMEN_COLS);
	memcpy(&maze[5][0], maze_row_5, MAZE_DIMEN_COLS);
	memcpy(&maze[6][0], maze_row_6, MAZE_DIMEN_COLS);
	memcpy(&maze[7][0], maze_row_7, MAZE_DIMEN_COLS);
	memcpy(&maze[8][0], maze_row_8, MAZE_DIMEN_COLS);

	for (i = 0; i < pass_len; ++i) {
		maze_step(password[i], &cur_loc, maze);
	}

	if (cur_loc.row == MAZE_EXIT_ROW && cur_loc.col == MAZE_EXIT_COL)
		if (pass_len == ORIG_PASSWORD_LEN)
			return 0;
	return 1;
}

static inline void ask_user_password(char *user_pass_buf, size_t size)
{
	print("Gimme ur password : ");
	read(user_pass_buf, size);
	print("\n");
}

int try_get_flag(void)
{
	char user_input[MAX_PASSWORD_LEN];
	memset(user_input, 0, MAX_PASSWORD_LEN);
	ask_user_password(user_input, MAX_PASSWORD_LEN - 1);
	return check_the_password(user_input);
}

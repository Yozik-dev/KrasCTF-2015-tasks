#include <common.h>

#include <asm/graphic/graphic.h>
#include <display.h>

void move_active_video_page_up(const video_page_t *video_page)
{
	short top_left  = (video_page->top_left_row  << 8) |
		video_page->top_left_col;
	short bot_right = (video_page->bot_right_row << 8) |
		video_page->bot_right_col;

	arch_move_active_video_page_up(top_left, bot_right,
				       video_page->shift_rows_once);
}

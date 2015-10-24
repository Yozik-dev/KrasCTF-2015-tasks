#ifndef ASM_GRAPHIC_H
#define ASM_GRAPHIC_H

void arch_move_cursor(int x, int y);
int arch_get_cursor_pos(void);

int arch_init_display(short mode);
void arch_move_active_video_page_up(short top_left, short bot_right,
				    unsigned char cnt);

#endif

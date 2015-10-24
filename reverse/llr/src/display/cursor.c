#include <common.h>

#include <asm/graphic/graphic.h>

void set_cursor_position(int x_pos, int y_pos)
{
	arch_move_cursor(x_pos, y_pos);
	return;
}

int get_cursor_position(void)
{
	return arch_get_cursor_pos();
}

#include <common.h>

#include <asm/graphic/graphic.h>
#include <io.h>
#include <string.h>

int init_display(short mode)
{
	return arch_init_display(mode);
}

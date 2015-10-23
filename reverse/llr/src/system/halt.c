#include <common.h>

#include <asm/system/system.h>

/**
* Halt the system
*/
void halt(void)
{
	arch_halt();
}

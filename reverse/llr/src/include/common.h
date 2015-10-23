#ifndef COMMON_H
#define COMMON_H

/**
 * This compiler directive should always be the first
 * compiler directive in any C source file
 */
__asm__(".code16gcc;");

#ifndef size_t_defined
typedef unsigned int size_t;
#define size_t_defined
#endif

#ifndef NULL
#define NULL 0
#endif

#endif

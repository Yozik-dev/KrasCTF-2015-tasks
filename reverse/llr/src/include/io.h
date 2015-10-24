#ifndef IO_H
#define IO_H

#include <common.h>
#include <display.h>

#ifndef DEFAULT_TEXT_COLOR
#define DEFAULT_TEXT_COLOR GRAY_TEXT_COLOR
#endif

#ifndef DEFAULT_TAB_SIZE
#define DEFAULT_TAB_SIZE 8
#endif

void set_text_color(text_color_t color);
void print(const char *message);
void error(const char *err_msg);
void putc(char);

char getc(void);
int read(char *buffer, size_t len);

#endif

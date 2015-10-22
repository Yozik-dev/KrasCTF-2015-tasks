#include <QCoreApplication>
#include <QDebug>
#include "server.h"

bool createConnection();

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if (argc != 3)
    {
        qDebug() << "USAGE: radio <server_ip> <client_ip>";
        return 0;
    }

    QString server_ip = argv[1];
    QString client_ip = argv[2];


    MyTcpServer server(server_ip, client_ip);
    server.start();


    return a.exec();
}


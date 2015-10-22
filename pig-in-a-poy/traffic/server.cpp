#include "server.h"
#include <QDebug>
#include "time.h"


MyTcpServer::MyTcpServer(QObject *parent) :
    QObject(parent)
{
    server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));
    qsrand(time(0));
}


void MyTcpServer::start()
{
    qDebug() << "Server start";
    if (!server->listen(QHostAddress("193.218.139.39"), 31337))
    {
        qDebug() << "Server bind error";
        return;
    }
    else
    {
        qDebug() << "Server binded";
    }
}

void MyTcpServer::newConnection()
{
    qDebug() << "new connection";
    QTcpSocket *socket = server->nextPendingConnection();
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));

    this->sockets_to_stages[socket] = 1;
    this->sockets_to_times[socket] = QDateTime::currentMSecsSinceEpoch();
    this->send_question(socket);
}

void MyTcpServer::disconnected()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(QObject::sender());
    if (!socket)
        return;

    qDebug() << "socket disconnected";
    this->sockets_to_stages.remove(socket);
    this->sockets_to_digits.remove(socket);
    this->sockets_to_times.remove(socket);
}

void MyTcpServer::readyRead()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(QObject::sender());
    if (!socket)
        return;

    qint64 curtime = QDateTime::currentMSecsSinceEpoch();
    if (curtime - this->sockets_to_times[socket] > 1000)
    {
        this->error_answer(socket, "You take too match time!");
        return;
    }

    QString client_answer = socket->readAll();
    client_answer.replace("\n", "");
    client_answer.replace("\r", "");
    int stage = this->sockets_to_stages.value(socket, 0);
    qDebug() << "reded " << client_answer.length() << " bytes, stage=" << stage;

    switch (stage)
    {
        case 1:
            if (client_answer.compare("John Smith") != 0)
            {
                this->error_answer(socket, "Get out of here, KGB's dog!");
            }
            else
            {
                this->sockets_to_stages[socket]++;
                this->send_question(socket);
            }
            return;

        case 2:
            if (client_answer.toInt() != this->sockets_to_digits[socket])
            {
                this->error_answer(socket, "Get out of here, KGB's dog!");
            }
            else
            {
                this->sockets_to_stages[socket]++;
                this->send_question(socket);
            }
            return;

        case 3:
            if (client_answer.compare("USSR IS SO GULAG") != 0)
            {
                this->error_answer(socket, "Get out of here, KGB's dog!");
            }
            else
            {
                this->sockets_to_stages[socket]++;
                this->send_question(socket);
                socket->close();
            }
            return;

        default:
            this->error_answer(socket, "Get out of here, KGB's dog!");
    }
}

void MyTcpServer::error_answer(QTcpSocket *socket, const char* message)
{
    socket->write(message);
    socket->close();
}

void MyTcpServer::send_question(QTcpSocket *socket)
{
    int stage = this->sockets_to_stages.value(socket, 0);
    QString message;
    switch (stage)
    {
        case 1:
            message = "Stay on your place! What's your name?";
            break;

        case 2:
        {
            int x = qrand() % 300;
            int y = qrand() % 300;
            this->sockets_to_digits[socket] = x+y;
            message = QString("Okay, John, solve the simple task: %1 + %2 = ?").arg(x).arg(y);
            break;
        }

        case 3:
            message = "We almost here... Now tell me the keyword";
            break;

        case 4:
            message = "Correct, you can pass! The key is <flag>";
            break;
    }

    socket->write(message.toUtf8());
    socket->flush();
}

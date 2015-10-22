#include "server.h"
#include <QDebug>
#include "time.h"


MyTcpServer::MyTcpServer(QString server_ip, QString client_ip, QObject *parent) :
    QObject(parent)
{
    this->server_ip = server_ip;
    this->client_ip = client_ip;
    server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));
    qsrand(time(0));
}


void MyTcpServer::start()
{
    qDebug() << "Server start";
    if (!server->listen(QHostAddress(this->server_ip), 31337))
    {
        qDebug() << "Server bind error";
        return;
    }
    else
    {
        this->new_round();
        qDebug() << "Server binded";
    }
}

void MyTcpServer::new_round()
{
    this->session = QString(this->getRandomHex(8) + "_" + this->getRandomHex(8) + "_" + this->getRandomHex(8)).split("_");
    foreach(QTcpSocket* socket, this->sockets_to_stages.keys())
        socket->close();
}

QString MyTcpServer::getRandomHex(const int &length)
{
    QString randomHex;

    for(int i = 0; i < length; i++)
    {
        int n = qrand() % 16;
        randomHex.append(QString::number(n,16));
    }

    return randomHex;
}

void MyTcpServer::newConnection()
{
    qDebug() << "new connection";
    QTcpSocket *socket = server->nextPendingConnection();
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));

    this->sockets_to_stages[socket] = 0;
    this->send_question(socket);
}

void MyTcpServer::disconnected()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(QObject::sender());
    if (!socket)
        return;

    qDebug() << "socket disconnected";
    this->sockets_to_stages.remove(socket);
}

void MyTcpServer::readyRead()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(QObject::sender());
    if (!socket)
        return;

    QString client_answer = socket->readAll();
    client_answer = client_answer.replace("\r", "");
    client_answer = client_answer.replace("\n", "");
    qDebug() << "readed: " << client_answer;
    QStringList parts = client_answer.split(" ");
    if (parts[0] == "GET_SESSION")
    {
        if (!this->check_ip(socket))
        {
            socket->write("INSIDER!");
            socket->close();

        }
        else
        {
            socket->write(this->session[this->sockets_to_stages[socket]].toUtf8());
            socket->flush();
            this->sockets_to_stages[socket] = (this->sockets_to_stages[socket] + 1) % 3;
        }

        return;
    }

    if (parts[0] == "GET_FLAG")
    {
        QString full_session = this->session.join("");
        if (parts[1] == full_session)
            socket->write(FLAG);

        socket->close();
    }
}

bool MyTcpServer::check_ip(QTcpSocket *socket)
{
    return this->client_ip == socket->peerAddress().toString();
}

void MyTcpServer::error_answer(QTcpSocket *socket, const char* message)
{
    socket->write(message);
    socket->close();
}

void MyTcpServer::send_question(QTcpSocket *socket)
{
    QString message = "Commands:\nGET_SESSION -- returns part of session\nGET_FLAG <session> -- returns flag\n";
    socket->write(message.toUtf8());
    socket->flush();
}

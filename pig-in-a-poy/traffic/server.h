#ifndef SERVER_H
#define SERVER_H

#include <QTcpSocket>
#include <QTcpServer>
#include <QtNetwork>
#include <QMap>



class MyTcpServer : public QObject
{
    Q_OBJECT
public:
    explicit MyTcpServer(QObject *parent = 0);
    void start();

public slots:
    void newConnection();
    void readyRead();
    void disconnected();

private:
    QTcpServer *server;

    void error_answer(QTcpSocket* socket, const char* message);
    void send_question(QTcpSocket* socket);

    QMap<QTcpSocket*, int> sockets_to_stages;
    QMap<QTcpSocket*, int> sockets_to_digits;
    QMap<QTcpSocket*, quint64> sockets_to_times;
};

#endif // SERVER_H

#ifndef SERVER_H
#define SERVER_H

#include <QTcpSocket>
#include <QTcpServer>
#include <QtNetwork>
#include <QMap>


#define FLAG "r@d10_@ac1vE_1m@g1ne_dr@g0ns"

class MyTcpServer : public QObject
{
    Q_OBJECT
public:
    explicit MyTcpServer(QString server_ip, QString client_ip, QObject *parent = 0);
    void start();

public slots:
    void new_round();

private slots:
    void newConnection();
    void readyRead();
    void disconnected();

private:
    QTcpServer *server;
    QString server_ip;
    QString client_ip;

    bool check_ip(QTcpSocket* socket);
    void error_answer(QTcpSocket* socket, const char* message);
    void send_question(QTcpSocket* socket);
    QString getRandomHex(const int &length);

    QMap<QTcpSocket*, int> sockets_to_stages;
    QStringList session;

};

#endif // SERVER_H

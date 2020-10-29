#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>


class Server : public QObject {

    Q_OBJECT

    public:
        explicit Server(QWidget *parent = nullptr);

    private slots:
        void sendFortune();

    private:
        void initServer();

        int status = 1;
        QTcpServer *tcpServer = nullptr;
        QVector<QString> fortunes;
};

#endif // SERVER_H

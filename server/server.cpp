#include "server.h"

#include <QMessageLogger>


Server::Server(QWidget* parent) {

    initServer();
}

void Server::initServer()
{
    tcpServer = new QTcpServer();
    if (!tcpServer->listen()) {
        logger.critical("Unable to start the server: .");
        close();
        return;
    }
//! [0]
    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address()) {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    // if we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    statusLabel->setText(tr("The server is running on\n\nIP: %1\nport: %2\n\n"
                            "Run the Fortune Client example now.")
                         .arg(ipAddress).arg(tcpServer->serverPort()));
//! [1]
}

//! [4]
void Server::sendFortune()
{
//! [5]
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_10);

    out << fortunes[QRandomGenerator::global()->bounded(fortunes.size())];
//! [4] //! [7]

    QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
    connect(clientConnection, &QAbstractSocket::disconnected,
            clientConnection, &QObject::deleteLater);
//! [7] //! [8]

    clientConnection->write(block);
    clientConnection->disconnectFromHost();
//! [5]
}

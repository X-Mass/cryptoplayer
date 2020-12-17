#include "player.h"
#include "ui_player.h"


Player::Player(QWidget *parent) : QWidget(parent), ui(new Ui::Player) {
    ui->setupUi(this);
}

Player::~Player() {
    delete ui;
}

Player::Player(QTcpSocket *tcpSocket, CryptoPP::SecByteBlock key, QWidget *parent) : QWidget(parent), ui(new Ui::Player) {
    ui->setupUi(this);
    this->tcpSocket = tcpSocket;
    this->key = key;
    updateTrackList();
}

void Player::updateTrackList() {
    QJsonObject message;
    message["action"] = "getTrackList";
    QJsonDocument document(message);
    QByteArray bytes = document.toJson();
    tcpSocket->write(bytes);
    if(tcpSocket->waitForReadyRead(5000)) {
        QJsonDocument recievedDocument = QJsonDocument::fromJson(tcpSocket->readAll());
        QJsonObject recieved = recievedDocument.object();
        if (recieved["action"].toString() == "getTrackList") {
            QJsonArray tracks = recieved["trackList"].toArray();
            for (auto track : tracks) {
                qDebug() << track;
            }
        }
    }
}

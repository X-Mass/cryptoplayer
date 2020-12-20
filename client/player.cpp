#include "player.h"
#include "ui_player.h"

#include <QMediaPlayer>
#include <experimental/filesystem>
#include "cryptlib.h"
#include "filters.h"
#include "base64.h"
#include "idea.h"
#include "modes.h"
#include "files.h"
#include "unistd.h"

#include <iostream>
#include <fstream>

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
    player = new QMediaPlayer;
    std::string ivs = "n27TDNB1syY=\n";
    std::string decoded;
    CryptoPP::StringSource (ivs, true,
                        new CryptoPP::Base64Decoder(
                                       new CryptoPP::StringSink(decoded)
                                       )
                        ); // StringSource
    CryptoPP::SecByteBlock block((const CryptoPP::byte*)decoded.data(), decoded.size());
    iv = block;
    updateTrackList();
    updateTrackListView();
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
            trackList.clear();
            QJsonArray tracks = recieved["trackList"].toArray();
            for (auto trackObject : tracks) {
                QJsonObject track = trackObject.toObject();
                trackList[track["name"].toString()] = track["id"].toInt();
                qDebug() << track;
            }
        }
    }
}

void Player::updateTrackListView() {
    ui->trackListWidget->clear();
    for (auto&& track : trackList.keys()) {
        ui->trackListWidget->addItem(track);
    }
}

void Player::decrypt(const CryptoPP::SecByteBlock &key, const CryptoPP::SecByteBlock &iv,
                         const std::string &filename_in, const std::string &filename_out) {
   CryptoPP::CBC_Mode<CryptoPP::IDEA>::Decryption cipher;
    cipher.SetKeyWithIV(key.data(), key.size(), iv.data());

    std::ifstream in{filename_in, std::ios::binary};
    std::ofstream out{filename_out, std::ios::binary | std::ios::trunc};

    CryptoPP::FileSource{in, /*pumpAll=*/true,
                       new CryptoPP::StreamTransformationFilter{
                           cipher, new CryptoPP::FileSink{out}}};
    in.close();
    out.close();

  //  chmod(filename, S_IRWXU|S_IRGRP|S_IRWXGRP|S_IRWXOTH);
}

void Player::on_trackListWidget_itemDoubleClicked(QListWidgetItem *item)
{
    unsigned int fileID(trackList[item->text()]);
    QString fileName = QString::number(fileID);

    if (!QFileInfo::exists(fileName)) {
        qDebug() << "1";
        getTrack(fileID);
    }
    decrypt(key, iv, fileName.toStdString(), (fileName + ".mp3").toStdString());
    std::string ivs = std::string(reinterpret_cast<const char*>(iv.data()), iv.size());
    std::string skey = std::string(reinterpret_cast<const char*>(key.data()), key.size());
    qDebug() << QString::fromStdString(skey);
    qDebug() << QString::fromStdString(ivs);
    player->setMedia(QUrl::fromLocalFile(QDir::currentPath() + "/" + fileName + ".mp3"));
    player->setVolume(50);
    player->play();
}

void Player::getTrack(unsigned int id) {
    QJsonObject message;
    message["action"] = "getTrack";
    message["trackID"] = (int) id;
    QJsonDocument document(message);
    QByteArray bytes = document.toJson();
    tcpSocket->write(bytes);
    tcpSocket->waitForReadyRead();
    QJsonDocument recievedDocument = QJsonDocument::fromJson(tcpSocket->readAll());
    QJsonObject recieved = recievedDocument.object();
    if (recieved["action"].toString() == "getTrack") {
        int size = recieved["size"].toInt();
        tcpSocket->write(bytes);
        QByteArray fileByteArray;
        while (fileByteArray.size() != size) {
            if(tcpSocket->waitForReadyRead(5000)) {
                fileByteArray.append(tcpSocket->readAll());
            }
        }
        qDebug() << "Size: " << fileByteArray.size();
        QSaveFile file(QString::number(id));
        file.open(QIODevice::WriteOnly);
        file.write(fileByteArray);
        file.commit();
    }
}

void Player::on_pushButton_clicked()
{
    player->pause();
}

void Player::on_pushButton_2_clicked()
{
    player->play();
}

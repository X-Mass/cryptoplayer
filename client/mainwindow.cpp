#include <QtCrypto>
#include <iostream>

#include "cryptlib.h"
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include "sha.h"
#include "filters.h"
#include "base64.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"


#define PORT 1337

std::string SHA256HashString(std::string aString){
    std::string digest;
    CryptoPP::SHA256 hash;

    CryptoPP::StringSource foo(aString, true,
    new CryptoPP::HashFilter(hash,
      new CryptoPP::Base64Encoder (
         new CryptoPP::StringSink(digest))));

    return digest;
}


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->verticalLayout->setAlignment(Qt::AlignHCenter);
    connectToServer();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent* event) {
    // code here to make all stuff always centered
    QMainWindow::resizeEvent(event);
}

void MainWindow::connectToServer() {
// Also have these signals to be connected:
//    connect(tcpSocket, SIGNAL(readyRead())...
//    connect(tcpSocket, SIGNAL(connected())...
//    connect(tcpSocket, SIGNAL(disconnected())...
//    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError))...
    tcpSocket = new QTcpSocket(this);
    tcpSocket->connectToHost("127.0.0.1", PORT);
    if (tcpSocket->waitForConnected(5000)) {
        // OK
        ui->label_2->setText(QString::fromUtf8("Connected to server"));
    } else {
        //TODO
        ui->label_2->setText(QString::fromUtf8("Can't connect to server"));
        delete tcpSocket;
    }
}

void MainWindow::on_signInButton_clicked() { // Sign in
    QJsonObject message;
    QString pass = ui->lineEdit_2->text();
    std::string hash = SHA256HashString(pass.toStdString());

    message["action"] = "login";
    message["login"] = ui->lineEdit->text();
//     QCA::Hash hash1(QStringLiteral("md5"));
    message["pass"] = QString::fromStdString(hash).left(43); // must be hash here, not pure string
    QJsonDocument document(message);
    QByteArray bytes = document.toJson();
    tcpSocket->write(bytes);
    if(tcpSocket->waitForReadyRead(5000)) {
        QJsonDocument recievedDocument = QJsonDocument::fromJson(tcpSocket->readAll());
        QJsonObject recieved = recievedDocument.object();
        if (recieved["action"].toString() == "login") {
            if (recieved["isValid"].toBool() == true) {
                std::string key;
                std::string encoded = recieved["key"].toString().toStdString();

                CryptoPP::StringSource (encoded, true,
                                    new CryptoPP::Base64Decoder(
                                                   new CryptoPP::StringSink(key)
                                                   )
                                    );
                CryptoPP::SecByteBlock keyBlock((const CryptoPP::byte*)key.data(), key.size());

                qDebug() << "Login/Password pair is correct";

                hide();
                Player* player = new Player(tcpSocket, keyBlock);
                player->resize(320, 240);
                player->show();
                // TODO
            } else {
                qDebug() << "Login/Password pair is incorrect";
                // TODO
            }
        } else {
            qDebug() << "Communication error";
        }
    } else {
        qDebug() << "Server not responding";
    }
}

void MainWindow::on_signUpButton_clicked() { // Sign up
    QJsonObject message;
    QString pass = ui->lineEdit_2->text();
    std::string hash = SHA256HashString(pass.toStdString());
    message["action"] = "register";
    message["login"] = ui->lineEdit->text();
    message["pass"] = QString::fromStdString(hash).left(43);

    QJsonDocument document(message);
    QByteArray bytes = document.toJson();
    tcpSocket->write(bytes);
    if(tcpSocket->waitForReadyRead(5000)) {
        QJsonDocument recievedDocument = QJsonDocument::fromJson(tcpSocket->readAll());
        QJsonObject recieved = recievedDocument.object();
        if (recieved["action"].toString() == "register") {
            // result 0 == success, 1 == user already in base
            if (recieved["result"].toString() == "0") {
                qDebug() << "New user signed up";
                // TODO
            } else if (recieved["result"].toString() == "1") {
                qDebug() << "User has already signed up";
            }
        } else {
            qDebug() << "Communication error";
        }
    } else {
        qDebug() << "Server not responding";
    }
}





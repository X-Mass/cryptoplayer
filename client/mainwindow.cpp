//#include <QtCrypto>

#include "mainwindow.h"
#include "ui_mainwindow.h"


#define PORT 1337

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
    message["action"] = "login";
    message["login"] = ui->lineEdit->text();
//     QCA::Hash hash1(QStringLiteral("md5"));
    message["pass"] = ui->lineEdit_2->text(); // must be hash here, not pure string
    QJsonDocument document(message);
    QByteArray bytes = document.toJson();
    tcpSocket->write(bytes);
    if(tcpSocket->waitForReadyRead(5000)) {
        QJsonDocument recievedDocument = QJsonDocument::fromJson(tcpSocket->readAll());
        QJsonObject recieved = recievedDocument.object();
        if (recieved["action"].toString() == "login") {
            if (recieved["isValid"].toBool() == true) {
                qDebug() << "Login/Password pair is correct";

                Player* player = new Player();
                player->setParent(this);
                player->resize(320, 240);
                player->show();
                hide();
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
    message["action"] = "register";
    message["login"] = ui->lineEdit->text();
    message["pass"] = ui->lineEdit_2->text(); // must be hash here, not pure string
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

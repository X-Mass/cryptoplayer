#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->verticalLayout->setAlignment(Qt::AlignHCenter);
    connectToServer();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent* event) {
    // code here to make all stuff centered everytime
    QMainWindow::resizeEvent(event);
}

void MainWindow::connectToServer() {
// Also have these signals to be connected:
//    connect(tcpSocket, SIGNAL(readyRead())...
//    connect(tcpSocket, SIGNAL(connected())...
//    connect(tcpSocket, SIGNAL(disconnected())...
//    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError))...
    tcpSocket = new QTcpSocket(this);
    tcpSocket->connectToHost("127.0.0.1", 13371);
    if (tcpSocket->waitForConnected(5000)) {
        // OK
        ui->label_2->setText(QString::fromUtf8("Connected to server"));
    } else {
        //TODO
        ui->label_2->setText(QString::fromUtf8("Can't connect to server"));
        delete tcpSocket;
    }
}

void MainWindow::on_pushButton_clicked() {
    QJsonObject message;
    message["action"] = "login";
    message["login"] = ui->lineEdit->text();
    message["pass"] = ui->lineEdit_2->text(); // must be hash here, not pure string
    QJsonDocument document(message);
    QByteArray bytes = document.toJson();
    tcpSocket->write(bytes);
    if(tcpSocket->waitForReadyRead(5000)) {
        QJsonDocument recievedDocument = QJsonDocument::fromJson(tcpSocket->readAll());
        QJsonObject recieved = recievedDocument.object();
        if (recieved["action"].toString() == "login" && recieved["isValid"].toBool() == true) {
            // OK
            qDebug() << "Login/Password pair is correct";
        } else {
            // NOT OK
            qDebug() << "Login/Password pair is incorrect";
        }
    } else {
        qDebug() << "Something wrong with the server";
    }
}

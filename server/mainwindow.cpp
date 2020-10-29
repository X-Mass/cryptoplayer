#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->verticalLayout->setAlignment(Qt::AlignHCenter);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_pushButton_clicked() {
    if (serverStatus == 0) {
        tcpServer = new QTcpServer(this);
        connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newUser()));
        if (!tcpServer->listen(QHostAddress::Any, 13371) && serverStatus == 0) {
            qDebug() << QObject::tr("Unable to start the server: %1.").arg(tcpServer->errorString());
            ui->label_2->setText(tcpServer->errorString());
        } else {
            serverStatus = 1;
            qDebug() << "TCPSocket listen on port 13371";
            ui->label_2->setText(QString::fromUtf8("Up"));
            ui->pushButton->setText(QString::fromUtf8("Stop server"));
            qDebug() << "Server up";
        }
    } else {
        serverStatus=0;
        foreach(int i, SClients.keys()) {
            QTextStream os(SClients[i]);
            os.setAutoDetectUnicode(true);
            os << QDateTime::currentDateTime().toString() << "\n";
            SClients[i]->close();
            SClients.remove(i);
        }
        tcpServer->close();
        ui->label_2->setText(QString::fromUtf8("Down"));
        ui->pushButton->setText(QString::fromUtf8("Start server"));
        qDebug() << "Server down";
    }
}

void MainWindow::newUser() {
    if(serverStatus == 1){
        qDebug() << "New connection";
        QTcpSocket* clientSocket=tcpServer->nextPendingConnection();
        int idusersocs=clientSocket->socketDescriptor();
        SClients[idusersocs]=clientSocket;
        ui->label_4->setText(QObject::tr("%1").arg(SClients.size()));
        connect(SClients[idusersocs], SIGNAL(readyRead()), this, SLOT(response()));
    }
}

void MainWindow::response() {
    QTcpSocket* clientSocket = (QTcpSocket*)sender();
//    int idusersocs=clientSocket->socketDescriptor();
    QJsonDocument recievedDocument = QJsonDocument::fromJson(clientSocket->readAll());
    QJsonObject recieved = recievedDocument.object();
    QJsonObject response;
    qDebug() << "Recieved a " << recieved["action"].toString() << " message";
    if (recieved["action"].toString() == "login") {
        response["action"] = "login";
        response["isValid"] = userInBase(recieved["login"].toString(), recieved["pass"].toString());
    }
    QJsonDocument responseDocument(response);
    QByteArray bytes = responseDocument.toJson();
    clientSocket->write(bytes);
//    clientSocket->write(bytes);
//    QTextStream os(clientSocket);
//    os.setAutoDetectUnicode(true);
//    os << "Fuck you\n"; // this will be sent to user
// if we need to close it
//    clientSocket->close();
//    SClients.remove(idusersocs);
}

bool MainWindow::userInBase(QString login, QString hash) {
    // TODO: DB stuff
    return true;
}

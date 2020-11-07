#include "mainwindow.h"
#include "ui_mainwindow.h"


#define PORT 1337

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->verticalLayout->setAlignment(Qt::AlignHCenter);
    // DB opening (or initialization, if there is no one)
    sdb = QSqlDatabase::addDatabase("QSQLITE");
    bool isInitiated = QFile::exists("player.sqlite");
    sdb.setDatabaseName("player.sqlite");
    if (!sdb.open())
        qDebug() << "Unable to start database drivers";
    if (!isInitiated) {
        QSqlQuery query;
        query.exec("create table users "
                  "(id integer primary key, "
                  "login varchar(20), " // size of strings?
                  "hash varchar(30), "  // size of strings?
                  "expiration date)");
    }
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_changeServerState_clicked() {
    if (serverStatus == 0) {
        tcpServer = new QTcpServer(this);
        connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newUser()));
        if (!tcpServer->listen(QHostAddress::Any, PORT) && serverStatus == 0) {
            qDebug() << QObject::tr("Unable to start the server: %1.").arg(tcpServer->errorString());
            ui->serverStatus->setText(tcpServer->errorString());
        } else {
            serverStatus = 1;
            qDebug() << "TCPSocket listen on port " << PORT;
            ui->serverStatus->setText(QString::fromUtf8("Up"));
            ui->changeServerState->setText(QString::fromUtf8("Stop server"));
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
        ui->serverStatus->setText(QString::fromUtf8("Down"));
        ui->changeServerState->setText(QString::fromUtf8("Start server"));
        qDebug() << "Server down";
    }
}

void MainWindow::newUser() {
    if(serverStatus == 1){
        qDebug() << "New connection";
        QTcpSocket* clientSocket=tcpServer->nextPendingConnection();
        int idusersocs=clientSocket->socketDescriptor();
        SClients[idusersocs]=clientSocket;
        ui->usersOnline->setText(QString(SClients.size()));
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
    QString action = recieved["action"].toString();
    if (action == "login") {
        response["action"] = "login";
        response["isValid"] = signIn(recieved["login"].toString(), recieved["pass"].toString());
    } else if (action == "register") {
        response["action"] = "register";
        response["result"] = signUp(recieved["login"].toString(), recieved["pass"].toString());
    }
    QJsonDocument responseDocument(response);
    QByteArray bytes = responseDocument.toJson();
    clientSocket->write(bytes);
// if we need to close it
//    clientSocket->close();
//    SClients.remove(idusersocs);
}

bool MainWindow::signIn(QString login, QString hash) {
    // TODO: DB stuff
    return true;
}

QString MainWindow::signUp(QString login, QString hash) {
    // TODO: DB stuff
    return QString::fromUtf8("success");
}

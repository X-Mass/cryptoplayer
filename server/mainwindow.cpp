#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMimeData>
#include <QDragEnterEvent>
#include "cryptlib.h"
#include "osrng.h"
#include "filters.h"
#include "base64.h"


#define PORT 1337

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
   /* std::string encoded, encoded2, encoded3;
    CryptoPP::SecByteBlock iv(16), userAccess(16);
    CryptoPP::OS_GenerateRandomBlock(false, iv, iv.size());
    CryptoPP::OS_GenerateRandomBlock(false, userAccess, userAccess.size());
    std::string ivs = std::string(reinterpret_cast<const char*>(iv.data()), iv.size());
    CryptoPP::StringSource (ivs, true,
                        new CryptoPP::Base64Encoder(
                                       new CryptoPP::StringSink(encoded)
                                       )
                        ); // StringSource
   //

    ivs = encoded;
    CryptoPP::StringSource (ivs, true,
                        new CryptoPP::Base64Decoder(
                                       new CryptoPP::StringSink(encoded2)
                                       )
                        ); // StringSource
    CryptoPP::SecByteBlock block((const CryptoPP::byte*)encoded2.data(), encoded2.size());
    if (block == iv) {
        qDebug() << "LOL";
    } else {
        qDebug() << "Not LOL";
    }


    CryptoPP::StringSource (encoded2, true,
                        new CryptoPP::Base64Encoder(
                                       new CryptoPP::StringSink(encoded3)
                                       )
                        );

    qDebug() << QString::fromStdString(encoded);
    qDebug() << QString::fromStdString(encoded2);
    qDebug() << QString::fromStdString(encoded3);*/

    ui->setupUi(this);
    ui->verticalLayout->setAlignment(Qt::AlignHCenter);
    setAcceptDrops(true);
    // DB opening (or initialization, if there is no one)
    sdb = QSqlDatabase::addDatabase("QSQLITE");
    bool isInitiated = QFile::exists("player.sqlite");
    sdb.setDatabaseName(qApp->applicationDirPath() + "/player.sqlite");
    if (!sdb.open())
        qDebug() << "Unable to start database drivers: " << sdb.lastError();
    QSqlQuery query;
    if (!isInitiated) {
        if (!query.exec("CREATE TABLE users "
                  "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
                  "login VARCHAR(20) UNIQUE, " // size of strings?
                  "hash VARCHAR(43), "  // size of strings?
                  "expiration DATE, "
                   "key VARCHAR(26))"
                        ))
            qDebug() << "ERROR: " << query.lastError().text();
        if (!query.exec("CREATE TABLE tracks "
                  "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
//                  "name VARCHAR(20), " // size of strings?
//                  "artist VARCHAR(20), "  // size of strings?
//                  "album VARCHAR(20))"))
                  "path VARCHAR(255) UNIQUE)"))
            qDebug() << "ERROR: " << query.lastError().text();
    }
    updateTrackList();
    updateTrackListView();
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
        ui->usersOnline->setText(QString("%1").arg(SClients.size()));
        connect(SClients[idusersocs], SIGNAL(readyRead()), this, SLOT(response()));
    }
}

void MainWindow::response() {
    QTcpSocket* clientSocket = (QTcpSocket*)sender();
//    int idusersocs=clientSocket->socketDescriptor();
    QJsonDocument recievedDocument = QJsonDocument::fromJson(clientSocket->readAll());
    QJsonObject recieved = recievedDocument.object();
    QJsonObject response;
    QString action = recieved["action"].toString();

    response["action"] = action;
    qDebug() << action << " action initiated";
    if (action == "login") {
        QString key = signIn(recieved["login"].toString(), recieved["pass"].toString());
        if (key.isEmpty()) {
            qDebug() << "3";
            response["isValid"] = false;
        } else {
            qDebug() << "4";
            response["isValid"] = true;
            response["key"] = key;
        }
    } else if (action == "register") {
        response["result"] = signUp(recieved["login"].toString(), recieved["pass"].toString());
    } else if (action == "getTrackList") {
        insertTrackListToJson(response);
    }
    QJsonDocument responseDocument(response);
    QByteArray bytes = responseDocument.toJson();
    clientSocket->write(bytes);
// if we need to close it
//    clientSocket->close();
//    SClients.remove(idusersocs);
}

QString MainWindow::signIn(QString login, QString hash) {
    QSqlQuery query;
    if (!query.exec(QString("SELECT hash,key FROM users WHERE login='%1'").arg(login)))
        qDebug() << "ERROR: " << query.lastError().text();
    if (query.first() && query.value(0).toString() == hash) {
        qDebug() << "1";
        return query.value(1).toString();
    }
    qDebug() << "2";
    return QString();
}

int MainWindow::signUp(QString login, QString hash) {
    QSqlQuery query;
    std::string keyString;
    CryptoPP::SecByteBlock key(16);
    CryptoPP::OS_GenerateRandomBlock(false, key, key.size());
    std::string keyRaw = std::string(reinterpret_cast<const char*>(key.data()), key.size());
    CryptoPP::StringSource (keyRaw, true,
                        new CryptoPP::Base64Encoder(
                                       new CryptoPP::StringSink(keyString)
                                       )
                        ); // StringSource

    if (!query.exec(QString("INSERT INTO users (login, hash, key) VALUES ('%1', '%2', '%3')").arg(login, hash, QString::fromStdString(keyString)))) {
        if (query.lastError().nativeErrorCode() == "19") {
            qDebug() << "SQLite error code:" << query.lastError().nativeErrorCode();
            return 1;
        }
    }
    return 0;
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *event) {
    foreach (const QUrl &url, event->mimeData()->urls()) {
        QString fileName = url.toLocalFile();
        // "/Users/iosif/Desktop/a.txt"
        if (addFile(fileName) != 0) {
            // Error handler
        }
    }
}

int MainWindow::addFile(QString fileName) {
    QSqlQuery query;
    if (!query.exec(QString("INSERT INTO tracks (path) VALUES ('%1')").arg(fileName))) {
        if (query.lastError().nativeErrorCode() == "19")
            qDebug() << "Something wring with track adding SQLite error code:" << query.lastError().nativeErrorCode();
    }
    updateTrackList();
    updateTrackListView();
    return 0;
}

void MainWindow::updateTrackList() {
    QSqlQuery query;
    if (!query.exec(QString("SELECT * FROM tracks")))
        qDebug() << "ERROR: " << query.lastError().text();
    trackList.clear();
    while (query.next()) {
        trackList[query.value(0).toUInt()] = query.value(1).toString();
    }
}

void MainWindow::updateTrackListView() {
    ui->trackListWidget->clear();
    for (auto&& track : trackList) {
        ui->trackListWidget->addItem(track);
    }
}

void MainWindow::insertTrackListToJson(QJsonObject& json) {
    QMapIterator<unsigned int, QString> i(trackList);
    QJsonArray jsonTrackList;
    while (i.hasNext()) {
        i.next();
        QJsonArray track;
        track.append((int)i.key());
        track.append(i.value());
        jsonTrackList.append (track);
    }
    json["trackList"] = jsonTrackList;
}

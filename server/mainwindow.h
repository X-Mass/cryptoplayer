#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QJsonObject>

#include <QtSql>
#include <QList>

#include "filters.h"
#include "cryptlib.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {

    Q_OBJECT

    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

    private slots:
        void on_changeServerState_clicked();
        void newUser();
        void response();
        void dragEnterEvent(QDragEnterEvent *event);
        void dropEvent(QDropEvent *event);

    private:
        Ui::MainWindow *ui;
        QTcpServer *tcpServer;
        bool serverStatus = 0;
        QMap<int, QTcpSocket *> SClients;
        QMap<int, CryptoPP::SecByteBlock> clientsKeys;
        CryptoPP::SecByteBlock iv;
        QString signIn(QString login, QString hash, int userID);
        int signUp(QString login, QString hash); // make enum for return value. Atm 0 == success, 1 == user already in base
        QSqlDatabase sdb;
        int addFile(QString fileName);
        QMap<unsigned int, QString> trackList;
        void updateTrackList();
        void updateTrackListView();
        void insertTrackListToJson(QJsonObject& json);
        void sendTrack(unsigned int trackID, QTcpSocket* clientSocket);
        void encrypt(const CryptoPP::SecByteBlock &key, const CryptoPP::SecByteBlock &iv,
                          const std::string &filename_in, const std::string &filename_out);
};
#endif // MAINWINDOW_H

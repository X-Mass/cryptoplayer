#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QJsonObject>
#include <QtCrypto>
#include <QtSql>


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

    private:
        Ui::MainWindow *ui;
        QTcpServer *tcpServer;
        bool serverStatus;
        QMap<int, QTcpSocket *> SClients;
        QMap<int, QCA::SymmetricKey *> clientsKeys;
        QMap<int, QCA::InitializationVector *> clientsIV;
        bool signIn(QString login, QString hash);
        QString signUp(QString login, QString hash);
        QSqlDatabase sdb;
};
#endif // MAINWINDOW_H

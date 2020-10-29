#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QJsonObject>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {

    Q_OBJECT

    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

    private slots:
        void on_pushButton_clicked();
        void newUser();
        void response();

    private:
        Ui::MainWindow *ui;
        QTcpServer *tcpServer;
        bool serverStatus;
        QMap<int,QTcpSocket *> SClients;
        bool userInBase(QString login, QString hash); // QString, ByteArray or sth else?
};
#endif // MAINWINDOW_H

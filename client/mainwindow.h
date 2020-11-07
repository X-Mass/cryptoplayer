#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QJsonObject>

#include "player.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {

    Q_OBJECT

    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();
        void resizeEvent(QResizeEvent* event);

    private slots:
        void on_signInButton_clicked();
        void on_signUpButton_clicked();

    private:
        Ui::MainWindow *ui;
        QTcpSocket *tcpSocket;
        void connectToServer();
        Player *player;
};
#endif // MAINWINDOW_H

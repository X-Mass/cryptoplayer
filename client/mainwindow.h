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
        void resizeEvent(QResizeEvent* event);

    private slots:
        void on_pushButton_clicked();

    private:
        Ui::MainWindow *ui;
        QTcpSocket *tcpSocket;
        void connectToServer();
};
#endif // MAINWINDOW_H

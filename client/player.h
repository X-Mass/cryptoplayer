#ifndef PLAYER_H
#define PLAYER_H

#include <QWidget>
#include <QtNetwork>
#include <QJsonObject>


namespace Ui {
class Player;
}

class Player : public QWidget {

    Q_OBJECT

    public:
        explicit Player(QWidget *parent = nullptr);
        explicit Player(QTcpSocket *tcpSocket, QWidget *parent = nullptr);
        ~Player();

    private:
        QTcpSocket *tcpSocket;
        Ui::Player *ui;
        QMap<unsigned int, QString> trackList;
        void updateTrackList();
        void updateTrackListView();
};

#endif // PLAYER_H

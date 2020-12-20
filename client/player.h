#ifndef PLAYER_H
#define PLAYER_H

#include <QWidget>
#include <QtNetwork>
#include <QJsonObject>
#include <QListWidgetItem>
#include <QMediaPlayer>
#include "cryptlib.h"
#include "secblock.h"

namespace Ui {
class Player;
}

class Player : public QWidget {

    Q_OBJECT

    public:
        explicit Player(QWidget *parent = nullptr);
        explicit Player(QTcpSocket *tcpSocket, CryptoPP::SecByteBlock key, QWidget *parent = nullptr);
        ~Player();

private slots:
    void on_trackListWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
        QMediaPlayer* player;
        QTcpSocket *tcpSocket;
        CryptoPP::SecByteBlock key;
        Ui::Player *ui;
        QMap<QString, unsigned int> trackList;
        CryptoPP::SecByteBlock iv;
        void decrypt (const CryptoPP::SecByteBlock &key, const CryptoPP::SecByteBlock &iv,
                      const std::string &filename_in, const std::string &filename_out);
        void updateTrackList();
        void updateTrackListView();
        void getTrack(unsigned int id);
};

#endif // PLAYER_H

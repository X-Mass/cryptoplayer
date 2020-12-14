#ifndef PLAYER_H
#define PLAYER_H

#include <QWidget>
#include <QStandardItemModel>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QMainWindow>
#include <QDebug>
#include <QFileDialog>
#include <QMediaPlayer>
#include <QMediaMetaData>
#include <QMediaPlaylist>
#include <QTime>

namespace Ui {
class Player;
}

class Player : public QWidget {

    Q_OBJECT

public:
    explicit Player(QWidget *parent = nullptr);
    ~Player();

private slots:
    void on_btn_add_clicked();              // Слот для обработки добавления треков через диалоговое окно
    void on_playButton_clicked();
    void on_pauseButton_clicked();
    void on_stopButton_clicked();
    void on_timeProgress_sliderMoved(int position);
    void on_volume_sliderMoved(int position);
    void on_actionOpenFile_triggered();

private:
    Ui::Player *ui;
    QMediaPlayer* mplayer;
    QMediaPlaylist * mplayList;

};

#endif // PLAYER_H

#include "player.h"
#include "ui_player.h"


Player::Player(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Player)
{

    ui->setupUi(this);

    mplayer = new QMediaPlayer(this);
    mplayList = new QMediaPlaylist(this);
    mplayList->setPlaybackMode(QMediaPlaylist::Loop);
    mplayList->addMedia(QUrl::fromLocalFile("C:/Users/luber/crypto/cryptoplayer/sax.mp3"));
    mplayer->setPlaylist(mplayList);
}

Player::~Player() {
    delete ui;
}

void Player::on_playButton_clicked()
{
    mplayer->play();
}


void Player::on_pauseButton_clicked()
{
    mplayer->pause();
}

void Player::on_stopButton_clicked()
{
    mplayer->stop();
}

void Player::on_timeProgress_sliderMoved(int position)
{
    mplayer->setPosition(position);
}

void Player::on_volume_sliderMoved(int position)
{
    mplayer->setVolume(position);
}

void Player::on_btn_add_clicked()
{
    // С помощью диалога выбора файлов делаем множественный выбор mp3 файлов
    QStringList files = QFileDialog::getOpenFileNames(this,
                                                      tr("Open files"),
                                                      QString(),
                                                      tr("Audio Files (*.mp3)"));

    // Далее устанавливаем данные по именам и пути к файлам
    // в плейлист и таблицу отображающую плейлист
    foreach (QString filePath, files) {
        QList<QStandardItem *> items;
        items.append(new QStandardItem(QDir(filePath).dirName()));
        items.append(new QStandardItem(filePath));
        mplayList->appendRow(items);
        mplayList->addMedia(QUrl(filePath));
    }
}

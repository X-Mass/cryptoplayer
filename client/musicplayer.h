#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <QMainWindow>
#include <QMenuBar>
#include <QToolBar>
#include <QMenu>
#include <QMediaPlayer>
#include <QFileDialog>
#include <QList>
#include <musicfile.h>
#include <QStringListModel>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QMouseEvent>
#include <QMediaMetaData>
#include <QMessageBox>
#include "controlui.h"
#include "centerui.h"
class MusicPlayer : public QMainWindow
{
    Q_OBJECT

public:
    MusicPlayer(QWidget *parent = 0);
    ~MusicPlayer();
    void createToolBar();
    void createMenuBar();
    void createStatusBar();
    //UI
    ControlUI * cui;

    CenterUI * tui;

    QMenu *file;
    QMenu *edit;
    QMenu *help;

    QMediaPlayer *player;

    int count=0;

    QList<MusicFile> playlist;

    QStringListModel * mo;

    QStringList sl;

public slots:
    void openOneFileFunction();
    void openOneFolderFunction();
    void playMusic();
    void puseMusic();
    void stopMusic();
    void nextMusic();
    void beforeMusic();
    void changeVolume();
    void changeLoad();
    void setLoadMaxValue();
    void setMusicPosition();
    void delMusicFile();
    void clearMusicList();
    void handleMusicPlayError();
    void playNextOneMusic(qint64 pox);
    void about();

protected :
    //void mousePressEvent(QMouseEvent *event);
signals:
};

#endif // MUSICPLAYER_H

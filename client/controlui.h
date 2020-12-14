#ifndef CONTROLUI_H
#define CONTROLUI_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QToolButton>
#include <QSlider>
#include <QDial>
#include <QAction>
#include <QFileDialog>
#include <QUrl>
#include <QDir>
#include <QFile>
#include <QDebug>
#include <QFileInfo>
#include <QStatusBar>
class ControlUI : public QWidget
{
    Q_OBJECT
public:
    explicit ControlUI(QWidget *parent = nullptr);

       QToolButton *play;

       QToolButton *pause;

       QToolButton *stop;

       QToolButton *next;

       QToolButton *before;

       QSlider *load;

       QDial *volume;

       QAction *openOneFile;

       QAction *openOneFolder;

       QAction *delOneFile;

       QAction *clearPlayList;

       QAction *about;

       QStatusBar *status;
signals:

public slots:
};

#endif // CONTROLUI_H

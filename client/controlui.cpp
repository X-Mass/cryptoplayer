#include "controlui.h"

ControlUI::ControlUI(QWidget *parent) : QWidget(parent)
{


        play = new QToolButton;

        pause = new QToolButton;

        stop = new QToolButton;

        next = new QToolButton;

        before =  new QToolButton;

        load = new QSlider;


        volume = new QDial;


        play->setIcon(QIcon(":/img/play.png"));
        pause->setIcon(QIcon(":/img/pause.png"));
        stop->setIcon(QIcon(":/img/stop.png"));
        next->setIcon(QIcon(":/img/next.png"));
        before->setIcon(QIcon(":/img/before.png"));

        play->setFixedSize(50,50);
        pause->setFixedSize(50,50);
        stop->setFixedSize(50,50);
        next->setFixedSize(50,50);
        before->setFixedSize(50,50);


        play->setStatusTip("Play");
        pause->setStatusTip("Pause");
        stop->setStatusTip("Stop");
        next->setStatusTip("Next");
        before->setStatusTip("Previous");


        volume->setMaximum(100);
        volume->setMinimum(0);
        volume->setFixedSize(50,50);


        volume->setStatusTip("Volume");


        volume->setValue(80);


        load->setOrientation(Qt::Horizontal);
        load->setFixedHeight(20);
        load->setMinimum(0);


        load->setStatusTip("Load");


        openOneFile = new QAction();
        openOneFile->setText("open file");
        openOneFile->setShortcut(QKeySequence::Open);


        openOneFolder = new QAction();
        openOneFolder->setText("open folder");


        delOneFile = new QAction();
        delOneFile->setText("Delete");


        clearPlayList = new QAction();
        clearPlayList->setText("ClearList");

        about = new QAction();
        about->setIcon(QIcon(":/img/bix.png"));
        about->setText("about");


        status = new QStatusBar;
        status->setStatusTip("Ready..");
        status->showMessage("Ready");
        status->activateWindow();
}

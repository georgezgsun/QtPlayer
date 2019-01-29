#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMediaPlayer>
#include <QVideoWidget>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->resize(1000,700);

    QVideoWidget *vw1 = new QVideoWidget(this);
    QVideoWidget *vw2 = new QVideoWidget(this);
    QVideoWidget *vw3 = new QVideoWidget(this);
    QVideoWidget *vw4 = new QVideoWidget(this);

    QMediaPlayer *mp1 = new QMediaPlayer(this);
    QMediaPlayer *mp2 = new QMediaPlayer(this);
    QMediaPlayer *mp3 = new QMediaPlayer(this);
    QMediaPlayer *mp4 = new QMediaPlayer(this);

    mp1->setMedia(QUrl::fromLocalFile("/home/georges/Videos/10072018172054-Recovered.mp4"));
    mp2->setMedia(QUrl::fromLocalFile("/home/georges/Videos/10072018101949.wmv"));
    mp3->setMedia(QUrl::fromLocalFile("/home/georges/Videos/10072018172054-Recovered.mp4"));
    mp4->setMedia(QUrl::fromLocalFile("/home/georges/Videos/10072018101949.wmv"));
    mp1->setVideoOutput(vw1);
    mp2->setVideoOutput(vw2);
    mp3->setVideoOutput(vw3);
    mp4->setVideoOutput(vw4);
    //this->setCentralWidget(vw1);

    vw1->setGeometry(10,20,480,300);
    vw1->show();
    mp1->play();

    vw2->setGeometry(500,20,480,300);
    vw2->show();
    mp2->play();

    vw3->setGeometry(10,320,480,300);
    vw3->show();
    mp3->play();

    vw4->setGeometry(500,320,480,300);
    vw4->show();
    mp4->play();
}

MainWindow::~MainWindow()
{
    delete ui;
}

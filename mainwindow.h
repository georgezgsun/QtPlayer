#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QMediaPlayer>
#include <QMainWindow>
#include <QLabel>
#include <QTimer>

#include "/home/georges/projects/Services/ServiceUtils.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    bool SetupCameras(int argc, char *argv[]);
    ~MainWindow();

public slots:
    void ChkMsg();
    void SwitchCamera();
    void Rec();
    void Review();
    void TakePhoto();

private:
    Ui::MainWindow *ui;

    QVideoWidget *vw1;
    QVideoWidget *vw2;
    QVideoWidget *vw3;
    QVideoWidget *vw4;
    QVideoWidget *vw5;

    QMediaPlayer *mp1;
    QMediaPlayer *mp2;
    QMediaPlayer *mp3;
    QMediaPlayer *mp4;
    QMediaPlayer *mp5;

    int m_CameraViewer;
    bool m_Recording;
    bool m_RecState;

    QTimer *m_timer;  // timer to check the new message
    ServiceUtils *m_Msg;  // the messenger

    QLabel *m_GPSLabel;
    QLabel *m_TriggerLabel;
    QLabel *m_RadarLabel;

};

#endif // MAINWINDOW_H

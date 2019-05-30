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

private:
    Ui::MainWindow *ui;
    QTimer *m_timer;  // timer to check the new message
    ServiceUtils *m_Msg;  // the messenger

    QLabel *m_GPSLabel;
    QLabel *m_TriggerLabel;
    QLabel *m_RadarLabel;

};

#endif // MAINWINDOW_H

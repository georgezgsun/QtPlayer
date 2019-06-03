#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "/home/georges/projects/Services/ServiceUtils.h"

#include <QMediaPlayer>
#include <QVideoWidget>

static string m_GPSTime;
static string m_GPS;  // the GPS data from GPS service
static string m_Trigger;  // the trigger data from Trigger service
static string m_Radar;  // the Radar data from Radar service
static string m_lastTrigger{};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->resize(1065,615);
}

bool MainWindow::SetupCameras(int argc, char *argv[])
{
    vw1 = new QVideoWidget(this);
    vw2 = new QVideoWidget(this);
    vw3 = new QVideoWidget(this);
    vw4 = new QVideoWidget(this);
    vw5 = new QVideoWidget(this);

    mp1 = new QMediaPlayer(this);
    mp2 = new QMediaPlayer(this);
    mp3 = new QMediaPlayer(this);
    mp4 = new QMediaPlayer(this);
    mp5 = new QMediaPlayer(this);

    m_CameraViewer = 0;
    m_Recording = false;
    m_RecState = false;

    m_Msg = new ServiceUtils(argc, argv);
    int ID = 0;
    int lastID = 0;
    static string CameraPath;
    static string CameraName;
    CameraPath = "rtsp://10.0.9.203:8554/0";

    m_Msg->LocalMap("ID", &ID);
    m_Msg->LocalMap("Camera Name", &CameraName);
    m_Msg->LocalMap("Camera Path", &CameraPath);
    m_Msg->LocalMap("time", &m_GPSTime);
    m_Msg->LocalMap("position", &m_GPS);
    m_Msg->LocalMap("Trigger", &m_Trigger);
    m_Msg->LocalMap("Radar", &m_Radar);

    if (!m_Msg->StartService())
        return -1;

    m_Msg->SubscribeService("GPS");
    m_Msg->SubscribeService("Radar");
    m_Msg->SubscribeService("Trigger");

    ID = 0;
    m_Msg->QueryConfigures();
    while (ID < 4)
    {
        if (!m_Msg->ChkNewMsg())
            continue;

        if (ID == lastID)
            continue;

        fprintf(stderr, "The camera %d gets a path of %s.\n", ID, CameraPath.c_str());
        switch (ID)
        {
        case 1:
            mp1->setMedia(QUrl(QString::fromStdString(CameraPath)));
            break;
        case 2:
            mp2->setMedia(QUrl(QString::fromStdString(CameraPath)));
            break;
        case 3:
            mp3->setMedia(QUrl(QString::fromStdString(CameraPath)));
            break;
        case 4:
            mp4->setMedia(QUrl(QString::fromStdString(CameraPath)));
            break;
        }

        lastID = ID;
    }

    mp1->setVideoOutput(vw1);
    vw1->setGeometry(2,20,480,270);
    vw1->show();
    mp1->play();

    mp2->setVideoOutput(vw2);
    vw2->setGeometry(485,20,480,270);
    vw2->show();
    mp2->play();

    mp3->setVideoOutput(vw3);
    vw3->setGeometry(2,294,480,270);
    vw3->show();
    mp3->play();

    mp4->setVideoOutput(vw4);
    vw4->setGeometry(485,294,480,270);
    vw4->show();
    mp4->play();

    mp5->setMedia(QUrl::fromLocalFile("/home/georges/Videos/10072018172054-Recovered.mp4"));
    mp5->setVideoOutput(vw5);
    vw5->setGeometry(2,20,960,540);
    vw5->hide();
    mp5->pause();

    m_timer = new QTimer(this);
    connect(m_timer,SIGNAL(timeout()), this, SLOT(ChkMsg()));
    m_timer->start(500);

    // The camera button
    connect(ui->Camera, SIGNAL(clicked()), this, SLOT(SwitchCamera()));

    // The record button
    connect(ui->Record, SIGNAL(clicked()), this, SLOT(Rec()));

    // The review button
    connect(ui->Review, SIGNAL(clicked()), this, SLOT(Review()));
    return true;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ChkMsg()
{
    size_t type = 1;
    long GPSChannel = m_Msg->GetServiceChannel("GPS");
    long TriggerChannel = m_Msg->GetServiceChannel("Trigger");
    long RadarChannel = m_Msg->GetServiceChannel("Radar");

    while (type)
    {
        type = m_Msg->ChkNewMsg();
        if (type == CMD_SERVICEDATA)
        {

            if (m_Msg->m_MsgChn == GPSChannel)
            {
                ui->GPS->setText(QString::fromStdString(m_GPS));
                ui->Radar->setText(QString::fromStdString(m_GPSTime));
            }
            else if (m_Msg->m_MsgChn == TriggerChannel)
            {
                ui->Trigger->setText(QString::fromStdString(m_Trigger));
                if (m_Trigger.compare(m_lastTrigger))
                {
                    m_Msg->Log("Triggers updated to " + m_Trigger);
                    m_lastTrigger = m_Trigger;
                }
            }
            else if (m_Msg->m_MsgChn == RadarChannel)
                continue;

            m_Msg->WatchdogFeed();
        }
    }

    if (m_Recording)
    {
        m_RecState = !m_RecState;
        if (m_RecState)
            ui->Record->setStyleSheet("border-image: url(:/Recording-94x94.png);");
        else
            ui->Record->setStyleSheet("border-image: url(:/Record-94x94.png);");
    }
    else
        ui->Record->setStyleSheet("border-image: url(:/Record-94x94.png);");

    return;
}

void MainWindow::SwitchCamera()
{
    m_CameraViewer++;
    mp5->stop();
    vw5->hide();

    switch (m_CameraViewer)
    {
    case 0:
    case 5:
        vw1->setGeometry(2,20,480,270);
        vw1->show();
        mp1->play();

        vw2->setGeometry(485,20,480,270);
        vw2->show();
        mp2->play();

        vw3->setGeometry(2,294,480,270);
        vw3->show();
        mp3->play();

        vw4->setGeometry(485,294,480,270);
        vw4->show();
        mp4->play();

        m_CameraViewer = 0;
        break;

    case 1:
        vw1->setGeometry(2, 20, 960, 540);
        mp1->play();
        vw1->show();

        mp2->pause();
        vw2->hide();

        mp3->pause();
        vw3->hide();

        mp4->pause();
        vw4->hide();
        break;

    case 2:
        mp1->pause();
        vw1->hide();

        vw2->setGeometry(2, 20, 960, 540);
        mp2->play();
        vw2->show();

        mp3->pause();
        vw3->hide();

        mp4->pause();
        vw4->hide();
        break;

    case 3:
        mp1->pause();
        vw1->hide();

        mp2->pause();
        vw2->hide();

        vw3->setGeometry(2, 20, 960, 540);
        mp3->play();
        vw3->show();

        mp4->pause();
        vw4->hide();
        break;

    case 4:
        mp1->pause();
        vw1->hide();

        mp2->pause();
        vw2->hide();

        mp3->pause();
        vw3->hide();

        vw4->setGeometry(2, 20, 960, 540);
        mp4->play();
        vw4->show();
        break;

    default:
        m_CameraViewer = 0;
    }
    return;
}

void MainWindow::Rec()
{
    m_Recording = ! m_Recording;
    if (m_Recording)
    {
        m_Msg->Log("User start a new recording by click on the Record button of the GUI.");
        m_Msg->SndCmd("Record=start", "Recorder");
    }
    else
    {
        m_Msg->Log(("User stop the recording by click on the Record button of the GUI."));
        m_Msg->SndCmd("Record=stop", "Recorder");
    }
    return;
}

void MainWindow::Review()
{
    // Hide all the camera live stream players
    mp1->pause();
    vw1->hide();

    mp2->pause();
    vw2->hide();

    mp3->pause();
    vw3->hide();

    mp4->pause();
    vw4->hide();

    // Play the review video
    mp5->play();
    vw5->show();

    return;
}

void MainWindow::TakePhoto()
{
    m_Msg->Log(("User take an extra photo by click on the Photo button of the GUI."));
    m_Msg->SndCmd("TakeAPhoto", "Recorder");

    return;
}

#ifndef EVENTS_H
#define EVENTS_H

#include <QWidget>
#include <fsmp_board/fsmpCamera.h>
#include <fsmp_board/fsmpTempHum.h>
#include <fsmp_board/fsmpEvents.h>
#include <fsmp_board/fsmpFan.h>
#include <fsmp_board/fsmpBeeper.h>
#include <QTimer>

#include<QtMqtt/qmqttclient.h>
#include<QtMqtt/qmqtttopicfilter.h>

#include<QJsonObject>
#include<QJsonDocument>

QT_BEGIN_NAMESPACE
namespace Ui { class events; }
QT_END_NAMESPACE
//class Widget;
class events : public QWidget
{
    Q_OBJECT

public:
    explicit events(QWidget *parent = nullptr);
    ~events();

private slots:

    void on_connect_clicked();
    void if_connected();
    void my_disconnected();

    void on_eve_clicked();

    void dispaly(QImage pix);
    void temphum_get();
    void people_get(bool detected);
    void fire_get();
    void on_alarm_clicked();
    void on_fan_clicked();
    void key_get(int keynum);
    void ledonFunction();
    void ledoffFunction();






private:
    Ui::events *ui;
    QTimer *t;
    fsmpCamera* cam;
    fsmpTempHum* temphum;
    fsmpEvents* event;
    fsmpFan* fan;
    fsmpBeeper*bee;
    //Widget *widgetObject;
    QMqttClient *client;


};

#endif // EVENTS_H

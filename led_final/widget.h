#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <fsmp_board/fsmpLed.h>
#include <fsmp_board/fsmpLight.h>
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
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:

    Widget(QWidget *parent = nullptr);
    ~Widget();

public slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

    void light_get();
    /*void on_pushButton_3_clicked();
    void dispaly(QImage);

    void on_pushButton_temphum_clicked();
    void temphum_get();

    void on_pushButton_people_clicked();
    void people_get(bool detected);

    void on_pushButton_fire_clicked();
    void fire_get();

    void on_pushButton_fan_clicked();

    void on_pushButton_bee_clicked();

    void on_pushButton_key_clicked();
    void key_get(int keynum);

    void on_pushButton_auto_clicked();
*/
    void on_pushButton_connect_clicked();
    void if_connected();
    void my_disconnected();
    void my_recv(QByteArray ba, QMqttTopicName tn);
    void ledon();
    void ledoff();
private slots:


private:
    Ui::Widget *ui;

    fsmpLeds* led;
    fsmpLight *light;
    QTimer *t;
   /* fsmpCamera* cam;
    fsmpTempHum* temphum;
  fsmpEvents* events;
    fsmpFan* fan;
    fsmpBeeper*bee;
*/

    QMqttClient *client;

};
#endif // WIDGET_H

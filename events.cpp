#include "events.h"
#include "ui_events.h"
#include <QTextEdit>
#include  "widget.h"
events::events(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::events)
{
    ui->setupUi(this);

   t     = new QTimer;

    cam   = new fsmpCamera;  //用了一个线程
    cam->setPixDelay(300);

    temphum =new fsmpTempHum;

    event=new fsmpEvents;

    fan =new fsmpFan;
    fan->setSpeed(100);

    bee=new fsmpBeeper;
    bee->setRate(500);

    client =new QMqttClient;
    // widgetObject = new Widget(this);
    connect(event, SIGNAL(keyPressed(int)), this, SLOT(key_get(int)));
}

events::~events()
{
    delete ui;
}
void events::ledonFunction()
{
    Widget widgetObject;
    // 调用 Widget 类中的槽函数，例如：
    widgetObject.ledon();
    //widgetObject.light_get();
}
void events::ledoffFunction()
{
    Widget widgetObject;
    // 调用 Widget 类中的槽函数，例如：
    widgetObject.ledoff();

}


void events::on_connect_clicked()
{
    client->setHostname("mqtt.yyzlab.com.cn");
    client->setPort(1883);
    client->setClientId("weiweiwei");
    client->connectToHost();
    connect(client,SIGNAL(connected()),this, SLOT(if_connected()));
    connect(client,SIGNAL(disconnected()),this, SLOT(my_disconnected()));
}
void events::if_connected()
{   ui->lineEdit->setText("YES");
    client->subscribe(QMqttTopicFilter("1715342633773/AIOTSIM2Device"));
    connect(client,SIGNAL(messageReceived(QByteArray, QMqttTopicName)),this,SLOT(my_recv(QByteArray,QMqttTopicName)));
}
void events::my_disconnected()
{
    ui->lineEdit->setText("NO");
}
void events::my_recv(QByteArray ba, QMqttTopicName tn)
{
    qDebug() << ba;
    qDebug() << tn;

    // 解析 JSON 数据
    QJsonDocument d = QJsonDocument::fromJson(ba);
    QJsonObject obj = d.object();

    // 根据 lamp 字段的值设置按钮文本
    bool r = obj.value("fan").toBool();
    if (r) {
        fan->start();
        ui->fan->setText("关闭风扇");
    } else {
        fan->stop();
        ui->fan->setText("打开风扇");
    }
    bool a = obj.value("alarm").toBool();
    if (a) {
        bee->start();
        ui->alarm->setText("关闭警报");
    } else {
       bee->stop();
        ui->alarm->setText("打开警报");
    }

}
//fire
void events::fire_get()
{
     ui->lineEdit_4->setText("危险");
     client->publish(QMqttTopicName("1715342633773/Device2AIOTSIM"),
     QByteArray("{\"flame\":true,\"id\":0}"));
     qDebug() << "fire" ;
     bee->start();
     client->publish(QMqttTopicName("1715342633773/Device2AIOTSIM"),
      QByteArray("{\"alarm\":true,\"id\":0}"));
     sleep(1);
     bee->stop();
     client->publish(QMqttTopicName("1715342633773/Device2AIOTSIM"),
      QByteArray("{\"alarm\":false,\"id\":0}"));
}
//人员检测
void events::people_get(bool detected)
    {
    qDebug() << "People Detected: " << detected;
    client->publish(QMqttTopicName("1715342633773/Device2AIOTSIM"),
     QByteArray("{\"infrared\":" + QByteArray::number(detected) + ",\"id\":0}"));

       if(detected==true){
            cam->start();
            connect(cam,SIGNAL(pixReady(QImage)),this, SLOT(dispaly(QImage)));
        }else{
            disconnect(cam,SIGNAL(pixReady(QImage)),this, SLOT(dispaly(QImage)));
            ui->cam->clear();
             }
}
//温湿度
void events::temphum_get()
{
     qDebug()<<"temperature:"<<temphum->temperature();
     qDebug()<<"humidity:"<<temphum->humidity();

     client->publish(QMqttTopicName("1715342633773/Device2AIOTSIM"),
     QByteArray("{\"tem\":" + QByteArray::number(temphum->temperature()) + ",\"hum\":" + QByteArray::number(temphum->humidity()) + ",\"id\":0}"));

     QString temperatureString = QString::number(temphum->temperature());
     ui->lineEdit_2->setText(temperatureString);
     QString shiduString = QString::number(temphum->humidity());
     ui->lineEdit_3->setText(shiduString);
     if(temphum->temperature()>30){
         fan->start();
     }else{
         fan->stop();
     }

}
//摄像
void events::dispaly(QImage pix)
{
    QPixmap p = QPixmap::fromImage(pix);
    ui->cam->setPixmap(p);
}

void events::on_eve_clicked()
{
    if(ui->eve->text() == "打开环境检测")
    {
        ui->eve->setText("退出环境检测");
        t->start(1000);
        connect(event,SIGNAL(flameDetected()),this, SLOT(fire_get()));
        connect(t,SIGNAL(timeout()),this, SLOT(temphum_get()));
        connect(event,SIGNAL(peopleDetected(bool)),this, SLOT(people_get(bool)));
        ui->lineEdit_4->setText("安全");
    } else
    {
        ui->eve->setText("打开环境检测");
        t->stop();

        disconnect(event,SIGNAL(flameDetected()),this, SLOT(fire_get()));
        disconnect(event,SIGNAL(peopleDetected(bool)),this, SLOT(people_get(bool)));
        disconnect(cam,SIGNAL(pixReady(QImage)),this, SLOT(dispaly(QImage)));
        ui->cam->clear();
         ui->lineEdit_4->setText("安全");
    }
}

//fan
void events::on_fan_clicked()
{
    if(ui->fan->text() == "打开风扇")
    {
        ui->fan->setText("关闭风扇");
        fan->start();
        client->publish(QMqttTopicName("1715342633773/Device2AIOTSIM"),
         QByteArray("{\"fan\":true,\"id\":0}"));
    }
    else
    {
        fan->stop();
        ui->fan->setText("打开风扇");
        client->publish(QMqttTopicName("1715342633773/Device2AIOTSIM"),
         QByteArray("{\"fan\":false,\"id\":0}"));
    }
}

//警报
void events::on_alarm_clicked()
{
    if(ui->alarm->text() == "打开警报")
    {
        ui->alarm->setText("关闭警报");
        bee->start();
        client->publish(QMqttTopicName("1715342633773/Device2AIOTSIM"),
         QByteArray("{\"alarm\":true,\"id\":0}"));
    }
    else
    {
        bee->stop();
        ui->alarm->setText("打开警报");
        client->publish(QMqttTopicName("1715342633773/Device2AIOTSIM"),
         QByteArray("{\"alarm\":false,\"id\":0}"));
    }
}
//按键
void events::key_get(int keynum){
    //key1
    if(keynum==1){
        t->start(1000);
        connect(event,SIGNAL(flameDetected()),this, SLOT(fire_get()));
        connect(t,SIGNAL(timeout()),this, SLOT(temphum_get()));
        connect(event,SIGNAL(peopleDetected(bool)),this, SLOT(people_get(bool)));
    }

    //key2
    if(keynum==2){
       ledonFunction();

    }

   //key3
    if(keynum==3){
        ledoffFunction();
        t->stop();

        disconnect(event,SIGNAL(flameDetected()),this, SLOT(fire_get()));
        disconnect(event,SIGNAL(peopleDetected(bool)),this, SLOT(people_get(bool)));
        disconnect(cam,SIGNAL(pixReady(QImage)),this, SLOT(dispaly(QImage)));
        ui->cam->clear();
        ui->lineEdit_2->clear();
        ui->lineEdit_3->clear();
        ui->lineEdit_4->clear();
    }
}






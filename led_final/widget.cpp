#include "widget.h"
#include "ui_widget.h"
#include <QTextEdit>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    led = new fsmpLeds;  //只能new一次
    light = new fsmpLight;
    t     = new QTimer;

  /*  cam   = new fsmpCamera;  //用了一个线程
    cam->setPixDelay(300);

    temphum =new fsmpTempHum;

    events=new fsmpEvents;

    fan =new fsmpFan;
    fan->setSpeed(100);

    bee=new fsmpBeeper;
    bee->setRate(500);*/

    client =new QMqttClient;
}

Widget::~Widget()
{
    delete ui;
}
void Widget::on_pushButton_connect_clicked()
{
    client->setHostname("mqtt.yyzlab.com.cn");
    client->setPort(1883);
    client->setClientId("weiweiwei");
    client->connectToHost();
    connect(client,SIGNAL(connected()),this, SLOT(if_connected()));
    connect(client,SIGNAL(disconnected()),this, SLOT(my_disconnected()));
}
void Widget::if_connected()
{   ui->lineEdit_result->setText("YES");
    client->subscribe(QMqttTopicFilter("1714956307509/AIOTSIM2Device"));
    connect(client,SIGNAL(messageReceived(QByteArray, QMqttTopicName)),this,SLOT(my_recv(QByteArray,QMqttTopicName)));
}
void Widget::my_disconnected()
{
    ui->lineEdit_result->setText("NO");
}
void Widget::my_recv(QByteArray ba, QMqttTopicName tn)
{
    qDebug() << ba;
    qDebug() << tn;

    // 解析 JSON 数据
    QJsonDocument d = QJsonDocument::fromJson(ba);
    QJsonObject obj = d.object();

    // 根据 lamp 字段的值设置按钮文本
    bool r = obj.value("lamp").toBool();
    if (r) {
        ui->pushButton->setText("关灯");
    } else {
        ui->pushButton->setText("开灯");
    }
}

//开灯
void Widget::on_pushButton_clicked()
{
    if(ui->pushButton->text() == "开灯")
    {
        led->on(fsmpLeds::LED1);
        ui->pushButton->setText("关灯");
        client->publish(QMqttTopicName("1714956307509/Device2AIOTSIM"),
         QByteArray("{\"lamp\":true,\"id\":0}"));

    }
    else
    {
         led->off(fsmpLeds::LED1);
         ui->pushButton->setText("开灯");
         client->publish(QMqttTopicName("1714956307509/Device2AIOTSIM"),
          QByteArray("{\"lamp\":false,\"id\":0}"));

    }
}
void Widget:: ledon(){
    led->on(fsmpLeds::LED2);
}
void Widget:: ledoff(){
    led->off(fsmpLeds::LED2);
}
//光感
void Widget::on_pushButton_2_clicked()
{
   if(ui->pushButton_2->text() == "光源检测")
   {
       ui->pushButton_2->setText("关闭检测");
       t->start(1000);
       connect(t,SIGNAL(timeout()),this, SLOT(light_get()));
   }
   else
   {
       ui->pushButton_2->setText("光源检测");

       t->stop();
   }
}

void Widget::light_get()
{
    qDebug()<<"light:"<<light->getValue();

    client->publish(QMqttTopicName("1714956307509/Device2AIOTSIM"),
     QByteArray("{\"light\":" + QByteArray::number(light->getValue()) + ",\"id\":0}"));
    if(light->getValue()<3){
        led->on(fsmpLeds::LED1);
        ui->result->setText("当前亮度较暗");

    }else{
        led->off(fsmpLeds::LED1);
        ui->result->setText("当前亮度适中");
    }
}
//摄像头
/*void Widget::on_pushButton_3_clicked()
{
    if(ui->pushButton_3->text() == "打开摄像头")
    {
        ui->pushButton_3->setText("关闭摄像头");
        cam->start();
       connect(cam,SIGNAL(pixReady(QImage)),this, SLOT(dispaly(QImage)));

    }
    else
    {
        ui->pushButton_3->setText("关闭摄像头");
        disconnect(cam,SIGNAL(pixReady(QImage)),this, SLOT(dispaly(QImage)));
    }
   // cam->start(); //开启摄像头
   // connect(cam,SIGNAL(pixReady(QImage)),this, SLOT(dispaly(QImage)));
}

void Widget::dispaly(QImage pix)
{
    QPixmap p = QPixmap::fromImage(pix);
    ui->label_2->setPixmap(p);
}



//温湿度
void Widget::on_pushButton_temphum_clicked()
{
    if(ui->pushButton_temphum->text() == "温湿度检测")
    {
        ui->pushButton_temphum->setText("关闭温湿度检测");
        t->start(1000);
        connect(t,SIGNAL(timeout()),this, SLOT(temphum_get()));

    }
    else
    {
        ui->pushButton_temphum->setText("温湿度检测");
        t->stop();
    }
}

void Widget::temphum_get()
{
     qDebug()<<"temperature:"<<temphum->temperature();
     qDebug()<<"humidity:"<<temphum->humidity();
     client->publish(QMqttTopicName("1714956307509/Device2AIOTSIM"),
      QByteArray("{\"tem\":" + QByteArray::number(temphum->temperature()) + ",\"hum\":" + QByteArray::number(temphum->humidity()) + ",\"id\":0}"));
     if(temphum->temperature()>30){
         fan->start();
     }else{
         fan->stop();
     }

}




//人员检测
void Widget::on_pushButton_people_clicked()
{
    if(ui->pushButton_people->text() == "人员检测")
    {
        ui->pushButton_people->setText("关闭人员检测");
        connect(events,SIGNAL(peopleDetected(bool)),this, SLOT(people_get(bool)));

    }
    else
    {
        disconnect(events, SIGNAL(peopleDetected(bool)), this, SLOT(people_get(bool)));
        ui->pushButton_people->setText("人员检测");
    }
}

void Widget::people_get(bool detected)
    {
    qDebug() << "People Detected: " << detected;
    client->publish(QMqttTopicName("1714956307509/Device2AIOTSIM"),
     QByteArray("{\"infrared\":" + QByteArray::number(detected) + ",\"id\":0}"));

       if(detected==true){
            cam->start();
            connect(cam,SIGNAL(pixReady(QImage)),this, SLOT(dispaly(QImage)));
        }else{
            disconnect(cam,SIGNAL(pixReady(QImage)),this, SLOT(dispaly(QImage)));
             }

}

//火焰检测
void Widget::on_pushButton_fire_clicked()
{
    if(ui->pushButton_fire->text() == "火焰检测")
    {
        ui->pushButton_fire->setText("停止火焰检测");

        connect(events,SIGNAL(flameDetected()),this, SLOT(fire_get()));

    }
    else
    {
        disconnect(events,SIGNAL(flameDetected()),this, SLOT(fire_get()));
        ui->pushButton_fire->setText("火焰检测");
    }
}
void Widget::fire_get(){
    client->publish(QMqttTopicName("1714956307509/Device2AIOTSIM"),
     QByteArray("{\"flame\":true,\"id\":0}"));
     qDebug() << "fire" ;
     bee->start();
     client->publish(QMqttTopicName("1714956307509/Device2AIOTSIM"),
      QByteArray("{\"alarm\":true,\"id\":0}"));
     sleep(1);
     bee->stop();
     client->publish(QMqttTopicName("1714956307509/Device2AIOTSIM"),
      QByteArray("{\"alarm\":false,\"id\":0}"));
}






//fan
void Widget::on_pushButton_fan_clicked()
{
    if(ui->pushButton_fan->text() == "打开风扇")
    {
        ui->pushButton_fan->setText("关闭风扇");
        fan->start();
        client->publish(QMqttTopicName("1714956307509/Device2AIOTSIM"),
         QByteArray("{\"fan\":true,\"id\":0}"));
    }
    else
    {
        fan->stop();
        ui->pushButton_fan->setText("打开风扇");
        client->publish(QMqttTopicName("1714956307509/Device2AIOTSIM"),
         QByteArray("{\"fan\":false,\"id\":0}"));
    }
}

//警报
void Widget::on_pushButton_bee_clicked()
{
    if(ui->pushButton_bee->text() == "警报")
    {
        ui->pushButton_bee->setText("关闭警报");
        bee->start();
        client->publish(QMqttTopicName("1714956307509/Device2AIOTSIM"),
         QByteArray("{\"alarm\":true,\"id\":0}"));
    }
    else
    {
        bee->stop();
        ui->pushButton_bee->setText("警报");
        client->publish(QMqttTopicName("1714956307509/Device2AIOTSIM"),
         QByteArray("{\"alarm\":false,\"id\":0}"));
    }
}

//按键

void Widget::on_pushButton_key_clicked()
{
    if(ui->pushButton_key->text() == "按键")
    {
     ui->pushButton_key->setText("关闭按键");
     connect(events,SIGNAL(keyPressed(int)),this, SLOT(key_get(int)));
    }
    else
    {
        disconnect(events,SIGNAL(keyPressed(int)),this, SLOT(key_get(int)));
        ui->pushButton_key->setText("关闭按键");
    }
}

void Widget::key_get(int keynum){
    //key1
    if(keynum==1){
        t->start(1000);
        connect(t,SIGNAL(timeout()),this, SLOT(light_get()));
        connect(events,SIGNAL(flameDetected()),this, SLOT(fire_get()));
        connect(t,SIGNAL(timeout()),this, SLOT(temphum_get()));
        connect(events,SIGNAL(peopleDetected(bool)),this, SLOT(people_get(bool)));
    }

    //key2
    if(keynum==2){
        led->on(fsmpLeds::LED2);
    }

   //key3
    if(keynum==3){
        t->stop();
        disconnect(events,SIGNAL(flameDetected()),this, SLOT(fire_get()));
        disconnect(events,SIGNAL(peopleDetected(bool)),this, SLOT(people_get(bool)));

    }
}

//自动

void Widget::on_pushButton_auto_clicked()
{
    if(ui->pushButton_auto->text() == "自动模式")
    {
        ui->pushButton_auto->setText("退出自动模式");
        connect(events,SIGNAL(keyPressed(int)),this, SLOT(key_get(int)));
        t->start(1000);
        connect(t,SIGNAL(timeout()),this, SLOT(light_get()));
        connect(events,SIGNAL(flameDetected()),this, SLOT(fire_get()));
        connect(t,SIGNAL(timeout()),this, SLOT(temphum_get()));
        connect(events,SIGNAL(peopleDetected(bool)),this, SLOT(people_get(bool)));
    } else
    {
        ui->pushButton_auto->setText("自动模式");
        t->stop();
        disconnect(events,SIGNAL(flameDetected()),this, SLOT(fire_get()));
        disconnect(events,SIGNAL(peopleDetected(bool)),this, SLOT(people_get(bool)));
    }
}
*/






#include "dialog.h"
#include "ui_dialog.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <string>
#include <QDebug>
#include <QMessageBox>


Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    ui->angle_lcdNumber->display("-------");
    ui->angle_lcdNumber_6->display("-------");
    ui->angle_lcdNumber_7->display("-------");
    ui->angle_lcdNumber_8->display("-------");
    ui->angle_lcdNumber_9->display("-------");
    arduino = new QSerialPort(this);
    serialBuffer = "";
    string_x = "";
    string_y = "";
    string_z = "";
    angle_value_x = 0.0;
    angle_value_y = 0.0;
    angle_value_z = 0.0;

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    scene2 = new QGraphicsScene(this);
    ui->graphicsView_6->setScene(scene2);

    scene3 = new QGraphicsScene(this);
    ui->graphicsView_7->setScene(scene3);

    QString image_filename = ":/img/oZ.png";
    QPixmap pxmap(image_filename);
    MyPix = scene->addPixmap(pxmap);

    QString image_filename2 = ":/img/oX.png";
    QPixmap pxmap2(image_filename2);
    MyPix2 = scene2->addPixmap(pxmap2);

    QString image_filename3 = ":/img/oY.png";
    QPixmap pxmap3(image_filename3);
    MyPix3 = scene3->addPixmap(pxmap3);


    bool arduino_is_available = false;
    QString arduino_uno_port_name;

    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){

        if(serialPortInfo.hasProductIdentifier() && serialPortInfo.hasVendorIdentifier()){

            if((serialPortInfo.productIdentifier() == arduino_uno_product_id)
                    && (serialPortInfo.vendorIdentifier() == arduino_uno_vendor_id)){
                arduino_is_available = true;
                arduino_uno_port_name = serialPortInfo.portName();
            }
        }
    }


    if(arduino_is_available){
        qDebug() << "Found the arduino port...\n";
        arduino->setPortName(arduino_uno_port_name);
        arduino->open(QSerialPort::ReadOnly);
        arduino->setBaudRate(QSerialPort::Baud38400);
        arduino->setDataBits(QSerialPort::Data8);
        arduino->setFlowControl(QSerialPort::NoFlowControl);
        arduino->setParity(QSerialPort::NoParity);
        arduino->setStopBits(QSerialPort::OneStop);
        QObject::connect(arduino, SIGNAL(readyRead()), this, SLOT(readSerial()));
    }
    else
    {
        qDebug() << "Невозможно найти корректный порт, с платой Arduino\n";
        QMessageBox::information(this, "Serial Port Error", "Couldn't open serial port to arduino.");
    }
}

Dialog::~Dialog()
{
    if(arduino->isOpen()){
        arduino->close();
    }
    delete ui;
}

void Dialog::readSerial()
{

    QStringList buffer_split = serialBuffer.split("\n");

    if(buffer_split.length() < 1){

        serialData = arduino->readAll();
        serialBuffer = serialBuffer + QString::fromStdString(serialData.toStdString());
        serialData.clear();
    }else{

        serialBuffer = "";
        parser = buffer_split[1].split(",");
        string_x = parser[0];
        string_y = parser[1];
        string_z = parser[2];
        string_hum = parser[3];
        string_temp = parser[4];
        angle_value_x = parser[0].toDouble();
        angle_value_y = parser[1].toDouble();
        angle_value_z = parser[2].toDouble();
        updateAngle_x(string_x, angle_value_x);
        updateAngle_y(string_y, angle_value_y);
        updateAngle_z(string_z, angle_value_z);
        update_temp(string_temp);
        update_hum(string_hum);
    }

}

void Dialog::updateAngle_z(QString sensor_reading, double temp)
{
    ui->graphicsView->resetTransform();
    ui->graphicsView->rotate(-temp);

    ui->angle_lcdNumber->display(sensor_reading);
}

void Dialog::updateAngle_x(QString sensor_reading, double temp)
{
    ui->graphicsView_6->resetTransform();
    ui->graphicsView_6->rotate(-temp);

    ui->angle_lcdNumber_6->display(sensor_reading);
}

void Dialog::updateAngle_y(QString sensor_reading, double temp)
{
    ui->graphicsView_7->resetTransform();
    ui->graphicsView_7->rotate(-temp);

    ui->angle_lcdNumber_7->display(sensor_reading);
}

void Dialog::update_temp(QString sensor_reading)
{
    ui->angle_lcdNumber_8->display(sensor_reading);
}

void Dialog::update_hum(QString sensor_reading)
{
    ui->angle_lcdNumber_9->display(sensor_reading);
}



#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QSerialPort>
#include <QByteArray>
#include <QGraphicsView>
#include <QGraphicsItem>


namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:

    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:
    void readSerial();
    void updateAngle_x(QString, double);
    void updateAngle_y(QString, double);
    void updateAngle_z(QString, double);
    void update_temp(QString);
    void update_hum(QString);

private:
    Ui::Dialog *ui;

    QGraphicsScene *scene;
    QGraphicsPixmapItem *MyPix;
    QGraphicsScene *scene2;
    QGraphicsPixmapItem *MyPix2;
    QGraphicsScene *scene3;
    QGraphicsPixmapItem *MyPix3;

    QSerialPort *arduino;
    static const quint16 arduino_uno_vendor_id = 9025;
    static const quint16 arduino_uno_product_id = 67;
    QByteArray serialData;
    QString serialBuffer;
    QString string_x;
    QString string_y;
    QString string_z;
    QString string_temp;
    QString string_hum;
    QStringList parser;
    double angle_value_x;
    double angle_value_y;
    double angle_value_z;
};

#endif // DIALOG_H

#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QDebug>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QString>
#include <QDialog>
#include <QStringList>
#include <QByteArray>
#include <QObject>
#include <QTimer>
#include "message.h"

class SerialPort:public QObject
{
    Q_OBJECT
public:
    SerialPort();
    ~SerialPort();
    bool CloseSerial();
    bool SendCmd(QByteArray);
    QStringList getSerialNames();
    bool EnterTestMode();
public slots:
    void ReceiveInfo();
    void SerialError(QSerialPort::SerialPortError error);
    void UpdateStatus();
signals:
    void UpdateSerialData(QByteArray);
    void SerialOpened();
private:
    QStringList serialNames;
    QSerialPort *m_port;
    QTimer* timer;
    QByteArray recvBuf;
    bool PortIsAvailable(QString);
    QString SerialScan();
    bool OpenSerial(QString);
    QByteArray GetOnePacket(int timeout);
    void CleanRecvBuf();
    const QString cv2SerialName= "nRF52 USB CDC BLE Demo";
//    const QString cv2SerialName="Qualcomm HS-USB Android GPS 9018";

    message* msg;
};

#endif // SERIALPORT_H

#include "serialport.h"
//#include <synchapi.h>
//#include <windows.h>
#include <QTime>
#include <QCoreApplication>

#define SERIALDELAY 100

SerialPort::SerialPort()
{
    msg = new message();
    m_port = new QSerialPort();
    timer = new QTimer(this);
    connect(m_port, SIGNAL(readyRead()), this, SLOT(ReceiveInfo()));
    connect(m_port, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
            this, &SerialPort::SerialError);
    connect(timer, SIGNAL(timeout()), this, SLOT(UpdateStatus()));
    timer->start(1000);
}

SerialPort::~SerialPort()
{

}
QString SerialPort::SerialScan()
{
    qDebug() << "SerialScan begin";

    serialNames.clear();
    foreach (const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        qDebug() << "get serial port:" << serial.portName() << "info:"
                 << info.description();
        if(info.description() == cv2SerialName)
        {
            serialNames.append(serial.portName());
            return serial.portName();
        }
    }
    return NULL;
}

bool SerialPort::PortIsAvailable(QString port)
{
    foreach (const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
//        qDebug() << "serial.portName():" << serial.portName();
        if(serial.portName() == port)
            return true;
    }
    qDebug() << "port " << port << "is not available";
    return false;
}

bool SerialPort::OpenSerial(QString port)
{
    if(!serialNames.contains(port))
    {
        qDebug() << port << " is not in serialList";
        return false;
    }
    if(!PortIsAvailable(port))
    {
        qDebug() << port << " is not available";
        return false;
    }
    m_port->setPortName(port);
    qDebug() << "set port name" << port;
    if(!m_port->open(QIODevice::ReadWrite))
    {
        qDebug() << "open " << port << "fail";
        return false;
    }
    if(!m_port->setBaudRate(QSerialPort::Baud115200))
    {
        qDebug() << "setBaudRate " << port << "fail";
        return false;
    }
    if(!m_port->setDataBits(QSerialPort::Data8))
    {
        qDebug() << "setDataBits " << port << "fail";
        return false;
    }
    if(!m_port->setParity(QSerialPort::NoParity))
    {
        qDebug() << "setParity " << port << "fail";
        return false;
    }
    if(!m_port->setStopBits(QSerialPort::OneStop))
    {
        qDebug() << "setStopBits " << port << "fail";
        return false;
    }
    if(!m_port->setFlowControl(QSerialPort::NoFlowControl))
    {
        qDebug() << "setFlowControl " << port << "fail";
        return false;
    }
    qDebug() << "open " << port << "successful";

    return true;
}

bool SerialPort::CloseSerial()
{
    if(serialNames.contains(m_port->portName()) && PortIsAvailable(m_port->portName()))
    {
        m_port->clear();
        m_port->close();
//        m_port->deleteLater();
        qDebug() << "close " << m_port->portName() << "successful";
        return true;
    }
    else
    {
        qDebug() << "close " << m_port->portName() << "fail";
        return false;
    }
}

bool SerialPort::SendCmd(QByteArray data)
{
    qDebug() << "send cmd";
//    for(int i = 0; i < data.size(); i++)
//    {
//        qDebug("%x,", (unsigned char)data[i]);
//    }
//    for(int i = 0; i < data.size(); i++)
//    {
//        data[i] = (unsigned char)(data[i]&0xff);
//    }
//    for(int i = 0; i < data.size(); i++)
//    {
//        qDebug("%x,", data[i]);
//    }
    qDebug() << data.toHex();
    int n = m_port->write(data);
    if(n == data.size())
        return true;
    else
    {
        qDebug() << "serial send data fail" << "need write:" << data.size()
                 << "actually write:" << n;
        return false;
    }

}

void SerialPort::ReceiveInfo()
{
    recvBuf = m_port->readAll();
//    QByteArray data = m_port->readAll();
//    recvBuf.append(data);
    qDebug() << "recv:" << recvBuf;
//    emit UpdateSerialData(recvBuf);

}

void SerialPort::CleanRecvBuf()
{
    recvBuf.clear();
}

void SerialPort::SerialError(QSerialPort::SerialPortError error)
{
    qDebug() << "SerialError";
    if (error == QSerialPort::ResourceError) {
         qDebug() << "ResourceError";
         m_port->close();
         timer->start(1000);
       }
}

void SerialPort::UpdateStatus()
{
    QString name = SerialScan();
    if(name != NULL)
    {
        qDebug() << "gete serial port:" << name;
        OpenSerial(name);
        timer->stop();
        emit SerialOpened();
    }
}

//QByteArray SerialPort::GetOnePacket(int timeout)
//{
//    int t = 0;
//    while(1)
//    {
//        Sleep(50);
//        t += 50;
//        if(t > timeout)
//            break;
//        if(msg->IsReplyPacket(recvBuf))
//        {
//            return recvBuf;
//        }
//    }
//    return NULL;
//}

//char ConvertHexChar(char ch)
//{
//    if((ch >= '0') && (ch <= '9'))
//        return ch-0x30;
//    else if((ch >= 'A') && (ch <= 'F'))
//        return ch-'A'+10;
//    else if((ch >= 'a') && (ch <= 'f'))
//        return ch-'a'+10;
//    else return ch-ch;//不在0-f范围内的会发送成0
//}
//bool StringToHex(QString str, QByteArray &data)
//{
//    int hexdata,lowhexdata;
//    int hexdatalen = 0;
//    int len = str.length();
//    data.resize(len/2);
//    char lstr,hstr;

//    for(int i=0; i<len; )
//    {
//        //char lstr,
//        hstr=str[i].toLatin1();
//        if(hstr == ' ')
//        {
//            i++;
//            continue;
//        }
//        i++;
//        if(i >= len)
//            break;
//        lstr = str[i].toLatin1();
//        hexdata = ConvertHexChar(hstr);
//        lowhexdata = ConvertHexChar(lstr);
//        if((hexdata == 16) || (lowhexdata == 16))
//            break;
//        else
//            hexdata = hexdata*16+lowhexdata;
//        i++;
//        data[hexdatalen] = (char)hexdata;
//        hexdatalen++;
//        qDebug("data:%x,", hexdata);
//    }
//    data.resize(hexdatalen);
//    if(hexdatalen == 0)
//        return false;
//    else
//        return true;
//}

void sleep(unsigned int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

bool SerialPort::EnterTestMode()
{
    if(!SendCmd(msg->PackEnterTestMode()))
    {
        return false;
    }
    sleep(SERIALDELAY);
//    if(!msg->IsReplyPacket(recvBuf))
//    {
//        return false;
//    }
    if(msg->IsEnterTestMode(recvBuf))
    {
        qDebug() << "EnterTestMode ok";
        CleanRecvBuf();
        return true;
    }
    else
    {
        qDebug() << "EnterTestMode fail";
        CleanRecvBuf();
        return false;
    }
}

QString SerialPort::GetNordicSoftVer()
{
    QString Ver;
    if(!SendCmd(msg->PackGetNordicSoftVer()))
    {
        return NULL;
    }
    sleep(SERIALDELAY);
    if(msg->GetNordicSoftVerFromData(recvBuf, Ver))
    {
        qDebug() << "GetNordicSoftVer ok";
        CleanRecvBuf();
        return Ver;
    }
    else
    {
        qDebug() << "GetNordicSoftVer fail";
        CleanRecvBuf();
        return NULL;
    }
}

QString SerialPort::GetNordicsSN()
{
    QString SN;
    if(!SendCmd(msg->PackGetNordicSN()))
    {
        return NULL;
    }
    sleep(SERIALDELAY);
    if(msg->GetNordicSNFromData(recvBuf, SN))
    {
        qDebug() << "GetNordicSN ok";
        CleanRecvBuf();
        return SN;
    }
    else
    {
        qDebug() << "GetNordicSN fail";
        CleanRecvBuf();
        return NULL;
    }
}

QStringList SerialPort::getSerialNames()
{
    return serialNames;
}

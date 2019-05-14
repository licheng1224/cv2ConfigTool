#include <QRegExp>
#include <QRegExpValidator>
#include "dialog.h"
#include "ui_dialog.h"


Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    this->setFixedSize(800, 400);

    mainVLayout = new QVBoxLayout(this);
    part1HLayout = new QHBoxLayout(this);
    part2HLayout = new QHBoxLayout(this);
    part3HLayout = new QHBoxLayout(this);
    serialStatusLayout = new QVBoxLayout(this);
    testModeLayout = new QHBoxLayout(this);
    realTimeLayout = new QVBoxLayout(this);


    part1HLayout->addLayout(serialStatusLayout);
    part1HLayout->setStretchFactor(serialStatusLayout, 1);
    part1HLayout->addLayout(realTimeLayout);
    part1HLayout->setStretchFactor(realTimeLayout, 4);
    mainVLayout->addLayout(part1HLayout);
    mainVLayout->addLayout(part2HLayout);
    mainVLayout->addLayout(part3HLayout);
    mainVLayout->setStretchFactor(part1HLayout, 1);
    mainVLayout->setStretchFactor(part2HLayout, 3);
    mainVLayout->setStretchFactor(part3HLayout, 2);
    this->setLayout(mainVLayout);


    SerialPortStatusLabel = new QLabel("not connect", this);
    SerialPortStatusLabel->setStyleSheet("font-size:20px");
    versionLabel = new QLabel("V0.1", this);
    enterTestModeBtn = new QPushButton("Test mode", this);
    enterTestModeBtn->setEnabled(false);
    testModeLabel = new QLabel("", this);

    testModeLayout->addWidget(enterTestModeBtn);
    testModeLayout->addWidget(testModeLabel);
    serialStatusLayout->addWidget(versionLabel);
    serialStatusLayout->addWidget(SerialPortStatusLabel);
    serialStatusLayout->addLayout(testModeLayout);

    SPort = new SerialPort();
    connect(SPort, SIGNAL(UpdateSerialData(QByteArray)), this, SLOT(UpdateEdit(QByteArray)));
    connect(SPort, SIGNAL(SerialOpened()), this, SLOT(CV2SerialOpened()));
    connect(enterTestModeBtn, SIGNAL(clicked()), this, SLOT(enterTestModeBtnClicked()));
}

bool Dialog::StringToHex(QString str, QByteArray &data)
{
    int hexdata,lowhexdata;
    int hexdatalen = 0;
    int len = str.length();
    data.resize(len/2);
    char lstr,hstr;

    for(int i=0; i<len; )
    {
        //char lstr,
        hstr=str[i].toLatin1();
        if(hstr == ' ')
        {
            i++;
            continue;
        }
        i++;
        if(i >= len)
            break;
        lstr = str[i].toLatin1();
        hexdata = ConvertHexChar(hstr);
        lowhexdata = ConvertHexChar(lstr);
        if((hexdata == 16) || (lowhexdata == 16))
            break;
        else
            hexdata = hexdata*16+lowhexdata;
        i++;
        data[hexdatalen] = (char)hexdata;
        hexdatalen++;
        qDebug("data:%x,", hexdata);
    }
    data.resize(hexdatalen);
    if(hexdatalen == 0)
        return false;
    else
        return true;
}

char Dialog::ConvertHexChar(char ch)
{
    if((ch >= '0') && (ch <= '9'))
        return ch-0x30;
    else if((ch >= 'A') && (ch <= 'F'))
        return ch-'A'+10;
    else if((ch >= 'a') && (ch <= 'f'))
        return ch-'a'+10;
    else return ch-ch;//不在0-f范围内的会发送成0
}
QString Dialog::HexToString(char *in, int len)
{
    int i;
    unsigned char inChar,hi,lo;
    QString s;

    for (i=0; i<len; i++)
    {
        inChar = in[i];
        hi = (inChar & 0xF0) >> 4;
        if (hi > 9)
            hi = 'A' + (hi - 0x0A);
        else
            hi += 0x30;
        s.append(hi);
        lo = inChar & 0x0F;
        if (lo > 9)
            lo = 'A' + (lo - 0x0A);
        else
            lo += 0x30;
        s.append(lo);
        s.append(0x20);
    }
    return s;
}

QString Dialog::ArrayToString(QByteArray array)
{
    char *buf = array.data();
    int len = array.size();
    return HexToString(buf, len);
}

void Dialog::UpdateEdit(QByteArray data)
{
    qDebug() << "update edit:" << data;
}

void Dialog::CV2SerialOpened()
{
    qDebug() << "serial port is opened:" << SPort->getSerialNames();
    enterTestModeBtn->setEnabled(true);
    SerialPortStatusLabel->setText("connect!");
//    SerialPortStatusLabel->setStyleSheet("background-color:rgb(0,255,0)");
    SerialPortStatusLabel->setStyleSheet("color:rgb(0,0,255);font-size:20px");
}

void Dialog::enterTestModeBtnClicked()
{
//    QByteArray testBa;
//     StringToHex(QString("aa 81 00 00 00 00 00 00 00 00 00 00 00 00 00 00"
//                         "00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00"), testBa);
//    SPort->SendCmd(testBa);
    if(SPort->EnterTestMode())
    {
        enterTestModeBtn->setEnabled(false);
        testModeLabel->setText("ok");
    }
    else
    {
        enterTestModeBtn->setEnabled(true);
        testModeLabel->setText("fail");
    }
}

Dialog::~Dialog()
{
    delete ui;
}

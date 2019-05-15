#ifndef MESSAGE_H
#define MESSAGE_H
#include <QByteArray>

class message
{
public:
    message();
    QByteArray PackEnterTestMode();
    QByteArray PackGetNordicSoftVer();
    QByteArray PackGetNordicSN();
    bool IsReplyPacket(QByteArray);
    bool IsEnterTestMode(QByteArray);
    bool GetNordicSoftVerFromData(QByteArray reply, QString& s);
    bool GetNordicSNFromData(QByteArray reply, QString& s);
private:
    char CRC8(const char *p, int counter);
    QByteArray GetMessageByType(const char* type, int len);
    QByteArray CRC8TableArray;
};

#endif // MESSAGE_H

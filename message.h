#ifndef MESSAGE_H
#define MESSAGE_H
#include <QByteArray>

class message
{
public:
    message();
    QByteArray PackEnterTestMode();
    bool IsReplyPacket(QByteArray);
    bool IsEnterTestMode(QByteArray);
private:
    char CRC8(const char *p, int counter);
    QByteArray GetMessageByType(const char* type, int len);
    QByteArray CRC8TableArray;
};

#endif // MESSAGE_H

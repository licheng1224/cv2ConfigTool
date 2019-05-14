#include "dialog.h"
#include <QTextCodec>
#include <QApplication>
//#pragma execution_character_set("utf-8")
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    QTextCodec *codec = QTextCodec::codecForName("utf8");
//    QTextCodec::setCodecForTr(codec);
//    QTextCodec::setCodecForLocale(codec);
//    QTextCodec::setCodecForCStrings(codec);
    Dialog w;
    w.show();

    return a.exec();
}

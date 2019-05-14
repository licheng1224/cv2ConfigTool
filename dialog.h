#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QComboBox>
#include <QByteArray>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "serialport.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    SerialPort *SPort;
public slots:
    void UpdateEdit(QByteArray);
    void CV2SerialOpened();
    void enterTestModeBtnClicked();
private:
    Ui::Dialog *ui;
    QVBoxLayout *mainVLayout;
    QHBoxLayout *part1HLayout;
    QHBoxLayout *part2HLayout;
    QHBoxLayout *part3HLayout;
    QVBoxLayout *serialStatusLayout;
    QHBoxLayout *testModeLayout;
    QVBoxLayout *realTimeLayout;
    QVBoxLayout *nordicStatusLayout;
    QVBoxLayout *nordicControlLayout;
    QHBoxLayout *keyLayout;
    QHBoxLayout *trigerLayout;
    QHBoxLayout *rockerLayout;


    QLabel *SerialPortStatusLabel;
    QLabel *versionLabel;
    QPushButton *enterTestModeBtn;
    QLabel *testModeLabel;
    QLabel *MCUSoftVerLabel;
    QLabel *MCUHardVerLabel;
    QLabel *BTAddrLabel;
    QLabel *IMUChipIDLabel;
    bool StringToHex(QString str, QByteArray &data);
    char ConvertHexChar(char ch);
    QString HexToString(char *in, int len);
    QString ArrayToString(QByteArray);

};

#endif // DIALOG_H

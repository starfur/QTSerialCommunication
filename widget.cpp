#include "widget.h"
#include "ui_widget.h"
#include <sstream>
#include <QObject>
#include <QDateTime>
#include <QDebug>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::displayErr(QString tag, QString message)
{
    tag = "<font color=\"Red\">" + tag + "</font>";
    message = "<font color=\"Red\">" + message + "</font>";
    qDebug()<<" displayErr ";
    ui->UpText_2->append(tag + " " + message);
    ui->UpText_1->append(tag + " " + message);
}

void Widget::displayWar(QString tag, QString message)
{
    tag = "<font color=\"Orange\">" + tag + "</font>";
    message = "<font color=\"Orange\">" + message + "</font>";
qDebug()<<" displayErr ";
    ui->UpText_2->append(tag + " " + message);
    ui->UpText_1->append(tag + " " + message);
}

void Widget::takeResponse(QByteArray bArr)
{
    qDebug()<<" displayErr ";
//    ui->textEdit->append(bArr.toPercentEncoding(bArr,bArr,'-'));
    QString resHex;
    for(int i=0;i<bArr.size();i++)
        resHex.append(QString::asprintf("%02X ",static_cast<unsigned char>(bArr.at(i))));

    resHex.replace(resHex.size()-1,1,"\n");
    ui->UpText_2->append(resHex);

    ui->UpText_1->append(bArr);
}

void Widget::takeCmdResponse(QByteArray bArr)
{
    qDebug()<<" displayErr ";
    int index=0;
    QByteArray cmdResArr;

    cmdResArr = QByteArray(bArr);

    for(index=0;index<cmdResArr.size();index++){
        if( static_cast<unsigned char>(cmdResArr[index]) == 0xE0){
            QString cmdRes;
            QString cmdResAscii;
            unsigned int cmdCRC=0;
            unsigned char cmdResLen = static_cast<unsigned char>(cmdResArr[index+1]);
            unsigned char cmdMsgID = static_cast<unsigned char>(cmdResArr[index+2]);
            if( cmdResLen + index <= cmdResArr.size() ){
                for(int j=0;j<cmdResLen;j++){
                    cmdCRC += static_cast<unsigned char>(cmdResArr[index+j]);
                    cmdRes.append(QString::asprintf("%02X ",static_cast<unsigned char>(cmdResArr[index+j])));
                    cmdResAscii.append(QString::asprintf(" %c ",static_cast<unsigned char>(cmdResArr[index+j])));
                }
                cmdCRC = 0xFF - static_cast<unsigned char>(cmdCRC);

                if(cmdCRC == 0x00){
                    QDateTime dateTime = QDateTime::currentDateTime();

                    ui->Down_Text_1->append("<font color=\"Blue\"><b> [ "+dateTime.time().toString("HH:mm:ss.zzz")+" ] Recv </b></font>");
                    ui->Down_Text_2->append("<font color=\"Blue\"><b> [ "+dateTime.time().toString("HH:mm:ss.zzz")+" ] Recv </b></font>");

                    ui->Down_Text_2->append("<font color=\"Blue\">"+cmdRes+"</font>");
                    ui->Down_Text_1->append("<font color=\"Blue\">"+cmdResAscii+"</font>");
                }
                else{
                    ui->Down_Text_1->append("<font color=\"DarkBlue\">[ERR][Recv]Cmd response CRC</font>");
                    ui->Down_Text_2->append("<font color=\"DarkBlue\">[ERR][Recv]Cmd response CRC</font>");
                    ui->Down_Text_2->append(QString::asprintf("ByteArrSize=%02X MsgID/ErrStat=%02X Len=%02X calcCRC=%02X CRCbyte=%02X"
                                                             ,cmdResArr.size(),cmdMsgID,cmdResLen,static_cast<unsigned char>(cmdCRC),static_cast<unsigned char>(cmdResArr[index+cmdResLen-1])));
                    continue;
                }
            }
            else{
                ui->Down_Text_1->append("<font color=\"DarkBlue\">[ERR][Recv]Response byte array lenght is short.</font>");
                ui->Down_Text_2->append("<font color=\"DarkBlue\">[ERR][Recv]Response byte array lenght is short.</font>");
                return;
            }
        }
    }
    return;
}

void Widget::takeSentDataSet(QByteArray bArr){
qDebug()<<" displayErr ";
    QString hexArr;
    QString asciiArr;

    for(int i=0;i<bArr.size();i++){
        hexArr.append(QString::asprintf("%02X ",static_cast<unsigned char>(bArr[i])));
        asciiArr.append(QString::asprintf(" %c ",static_cast<unsigned char>(bArr[i])));
    }

    QDateTime dateTime = QDateTime::currentDateTime();

    ui->Down_Text_1->append("<font color=\"Red\"><b> [ "+dateTime.time().toString("HH:mm:ss.zzz")+" ] Send </b></font>");
    ui->Down_Text_2->append("<font color=\"Red\"><b> [ "+dateTime.time().toString("HH:mm:ss.zzz")+" ] Send </b></font>");

    ui->Down_Text_2->append("<font color=\"Red\">"+hexArr+"</font>");
    ui->Down_Text_1->append("<font color=\"Red\">"+asciiArr+"</font>");
}

////////////////////////////////////////
void Widget::on_ConnectButton_clicked()
{
    qDebug()<<" on_ConnectButton_clicked ";
    TVSerialCommunicationConfig stConnectTVMessage;
    stConnectTVMessage.m_strPortName = ui->BR_edit->text();
    stConnectTVMessage.m_eBaudRate = static_cast<QSerialPort::BaudRate>(ui->BR_edit->text().toInt());

    emit signalConnectTV(static_cast<void*>(&stConnectTVMessage));
}

void Widget::on_DisconnectButton_clicked()
{
    qDebug()<<" on_DisconnectButton_clicked ";
    emit signalDisconnectTV();
}

void Widget::on_SendCMDButton_clicked()
{
    qDebug()<<" on_SendCMDButton_clicked ";
    QStringList commands = ui->CMD_Edit->text().split(" ");
    QString ComID = commands.at(0);
    QByteArray CRC;
    bool ok;

    for(int i = 1; i< commands.size(); i++)
    {
        bool ok;
        CRC.append(static_cast<char>(commands.at(i).toUInt(&ok,16)));
    }

    emit signalSendCommand(static_cast<quint8>(ComID.toUInt(&ok, 16)),CRC);
}

void Widget::on_ClearButton_clicked()
{
    qDebug()<<" on_clearButton_clicked ";
    ui->UpText_1->clear();
    ui->UpText_2->clear();
    ui->Down_Text_1->clear();
    ui->Down_Text_2->clear();
}

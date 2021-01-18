#include "widget.h"

#include <QApplication>
#include "tvSerialCommData.h"
#include "tvSerialCommDef.h"
#include "tvserialcomm.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();

    tvSerialComm objSerialComm;

    QObject::connect(&objSerialComm,SIGNAL(SendResponse(QByteArray)),&w,SLOT(takeResponse(QByteArray)));
    QObject::connect(&objSerialComm,SIGNAL(SendResponse(QByteArray)),&w,SLOT(takeCmdResponse(QByteArray)));
    QObject::connect(&objSerialComm,SIGNAL(SendError(QString,QString)),&w,SLOT(displayErr(QString,QString)));
    QObject::connect(&objSerialComm,SIGNAL(SendWarning(QString,QString)),&w,SLOT(displayWar(QString,QString)));
    QObject::connect(&objSerialComm,SIGNAL(SendDataSentSet(QByteArray)),&w,SLOT(takeSentDataSet(QByteArray)));

    QObject::connect(&w,SIGNAL(signalConnectTV(void*)),&objSerialComm,SLOT(Connect(void*)));
    QObject::connect(&w,SIGNAL(signalDisconnectTV()),&objSerialComm,SLOT(Disconnect()));
    QObject::connect(&w,SIGNAL(signalSendCommand(quint8,QByteArray)),&objSerialComm,SLOT(SendCommand(quint8,QByteArray)));

    return a.exec();
}

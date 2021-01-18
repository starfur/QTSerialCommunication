#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "tvSerialCommData.h"
#include "tvSerialCommDef.h"
#include "tvserialcomm.h"
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void SendCommand();

 signals:
     void signalConnectTV(void*);

     void signalDisconnectTV();

     void signalSendCommand(quint8 eCommand, QByteArray cAdditionalCommandData);

 public slots:
     void displayErr(QString tag, QString message);
     void displayWar(QString tag, QString message);

     void takeResponse(QByteArray bArr);

     void takeCmdResponse(QByteArray bArr);

     void takeSentDataSet(QByteArray bArr);



private slots:
     void on_ConnectButton_clicked();

     void on_DisconnectButton_clicked();

     void on_SendCMDButton_clicked();

     void on_ClearButton_clicked();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H

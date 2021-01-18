#ifndef TVSERIALCOMM_H
#define TVSERIALCOMM_H

#include <QObject>
#include <QSerialPort>
#include "tvSerialCommDef.h"
#include "tvSerialCommData.h"
#include <QtCore/qglobal.h>

class tvSerialComm : public QObject
{
    Q_OBJECT

    // Methods
private:
    quint8 CalculateCheckSum(QByteArray cCommandArray);

public:
    explicit tvSerialComm(QObject *parent = nullptr);
    virtual ~tvSerialComm();

    virtual bool GetConnectionStatus();

public slots:
    virtual bool Connect(void* pvConfig);
    virtual void Disconnect();
    void GetSerialPortError(QSerialPort::SerialPortError eSerialPortError);
    virtual QByteArray ReadCommand();
    virtual bool SendCommand(quint8 eCommand, QByteArray cAdditionalCommandData);

signals:
    void SendResponse(QByteArray bArr);

    void SendError(QString strTag, QString strMessage);
    void SendWarning(QString strTag, QString strMessage);
    void SendDataSentSet(QByteArray bArr);
    void SendDataSentStatus(bool bStatus);
    void SendFACommand(QByteArray);
    // Attributes
private:
    QSerialPort m_cSerialPort;
};

#endif // TVSERIALCOMM_H

#ifndef TVSERIALCOMMDATA_H
#define TVSERIALCOMMDATA_H

#include <QSerialPort>

#define TV_DELAY_WAIT_FOR_BYTES_WRITTEN_IN_SEC 1000
#define TV_DELAY_WAIT_FOR_BYTES_READ_IN_SEC 1000

typedef struct
{
    QString m_strPortName;
    QSerialPort::BaudRate m_eBaudRate;

}TVSerialCommunicationConfig;
Q_DECLARE_METATYPE(TVSerialCommunicationConfig)

#endif // TVSERIALCOMMDATA_H

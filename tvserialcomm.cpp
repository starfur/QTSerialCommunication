#include <QDebug>
#include <QMetaEnum>
#include "tvserialcomm.h"

tvSerialComm::tvSerialComm(QObject *parent) : QObject(parent)
  {
      qRegisterMetaType<TVSerialCommunicationConfig>();
  }


tvSerialComm::~tvSerialComm()
{
    if(GetConnectionStatus())
        Disconnect();
}


bool tvSerialComm::Connect(void* pvConfig)
{
    if(GetConnectionStatus())
    {
        PRINT_DEBUG_MESSAGE_WARNING(MESSAGE_TV_TAG, WARNING_ALREADY_CONNECTED);
        emit SendWarning(QString(MESSAGE_TV_TAG), WARNING_ALREADY_CONNECTED);
        return true;
    }

    if(pvConfig == nullptr)
    {
        PRINT_DEBUG_MESSAGE_CRITICAL(MESSAGE_TV_TAG, ERROR_INVALID_INPUT_PARAMETER);
        return false;
    }

    TVSerialCommunicationConfig* stSerialCommConfig = static_cast<TVSerialCommunicationConfig*>(pvConfig);

    m_cSerialPort.setPortName(stSerialCommConfig->m_strPortName);
    m_cSerialPort.setBaudRate(stSerialCommConfig->m_eBaudRate);

    if (!m_cSerialPort.open(QIODevice::ReadWrite))
    {
        qDebug() << "in Connect Test 1";
        emit SendError(MESSAGE_TV_TAG, ERROR_CANNOT_OPEN_SERIAL_PORT);
        return false;
    }
    else
    {
        qDebug() << "in Connect Test 2";
        connect(&m_cSerialPort, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(GetSerialPortError(QSerialPort::SerialPortError)));
        connect(&m_cSerialPort, SIGNAL(readyRead()),this,SLOT(ReadCommand()));
        return true;
    }
}

void tvSerialComm::Disconnect()
{
    if(GetConnectionStatus() == true)
    {
        disconnect(&m_cSerialPort, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(GetSerialPortError(QSerialPort::SerialPortError)));
        m_cSerialPort.close();
    }
    else
    {
        PRINT_DEBUG_MESSAGE_WARNING(MESSAGE_TV_TAG,WARNING_ALREADY_DISCONNECTED);
        emit SendWarning(MESSAGE_TV_TAG, WARNING_ALREADY_DISCONNECTED);
    }
}

bool tvSerialComm::SendCommand(quint8 eCommand, QByteArray cAdditionalCommandData)
{
    if(!GetConnectionStatus())
    {
        PRINT_DEBUG_MESSAGE_CRITICAL(MESSAGE_TV_TAG, ERROR_CHECK_CONNECTION);
        SendError(MESSAGE_TV_TAG, ERROR_CHECK_CONNECTION);
        return false;
    }

//    if(eCommand >= FACTORY_MSG_END)
//    {
//        PRINT_DEBUG_MESSAGE_CRITICAL(MESSAGE_TV_TAG, ERROR_INVALID_INPUT_PARAMETER);
//        SendError(MESSAGE_TV_TAG, ERROR_INVALID_INPUT_PARAMETER);
//        return false;
//    }

    QByteArray cCommandArray;
    cCommandArray.append(static_cast<char>(START_HEADER));
    cCommandArray.append(static_cast<char>(cAdditionalCommandData.count()+4));
    cCommandArray.append(static_cast<char>(eCommand));
    if(cAdditionalCommandData.count() != 0)
        cCommandArray.append(cAdditionalCommandData);
    cCommandArray.append(static_cast<char>(CalculateCheckSum(cCommandArray)));

    bool bDataSent = false;

    PRINT_DEBUG_MESSAGE_WARNING(MESSAGE_TV_TAG, WARNING_ALREADY_CONNECTED);

    fprintf(stderr,"%d ",eCommand);
    fflush(stderr);


    if(m_cSerialPort.write(cCommandArray) != -1)
    {
        if(m_cSerialPort.waitForBytesWritten(TV_DELAY_WAIT_FOR_BYTES_WRITTEN_IN_SEC))
        {
            bDataSent = true;
        }
        else
            PRINT_DEBUG_MESSAGE_CRITICAL(MESSAGE_TV_TAG, ERROR_WRITE_REQUEST_TIMEOUT);
    }

    emit SendDataSentSet(cCommandArray);
    emit SendDataSentStatus(bDataSent);

    return bDataSent;
}

bool tvSerialComm::GetConnectionStatus()
{
    return m_cSerialPort.isOpen();
}

quint8 tvSerialComm::CalculateCheckSum(QByteArray cCommandArray)
{
    quint8 sum = 0;

    for(int i = 0;i< cCommandArray.count();i++)  //Count from beginning to length - 1(CRC)
    {
         sum += cCommandArray.at(i);
    }
    return (CRC_BIT - sum);
}

QByteArray tvSerialComm::ReadCommand()
{
    QByteArray responseData;
    m_cSerialPort.waitForReadyRead(200);
    responseData = m_cSerialPort.readAll();
//    for(int i=0;i<responseData.size();i++)
//        qDebug() << QString::asprintf("%02X ",responseData.at(i));

    emit SendResponse(responseData);
    return responseData;
}

void tvSerialComm::GetSerialPortError(QSerialPort::SerialPortError eSerialPortError)
{
    QString strError = QMetaEnum::fromType<QSerialPort::SerialPortError>().valueToKey(eSerialPortError);
    if(eSerialPortError != QSerialPort::NoError && eSerialPortError != QSerialPort::ReadError && eSerialPortError != QSerialPort::TimeoutError)
    {
        disconnect(&m_cSerialPort, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(GetSerialPortError(QSerialPort::SerialPortError)));

        if(GetConnectionStatus())
            m_cSerialPort.close();


        PRINT_DEBUG_MESSAGE_CRITICAL(MESSAGE_TV_TAG, strError);

        emit SendError(MESSAGE_TV_TAG, strError);
    }
    else
    {
        PRINT_DEBUG_MESSAGE_CRITICAL(MESSAGE_TV_TAG, strError);
    }
}

#ifndef TVSERIALCOMMDEF_H
#define TVSERIALCOMMDEF_H



#define CRC_BIT 0xFF
#define START_HEADER 0xE0

#define UNUSED_PARAM(x) (void)x

#define DEBUG_MESSAGE_CRITICAL_TAG "[CRITICAL]"
#define DEBUG_MESSAGE_WARNING_TAG "[WARNING]"
#define DEBUG_MESSAGE_DEBUG_TAG "[DEBUG]"
#define DEBUG_MESSAGE_INFO_TAG "[INFO]"

#define MESSAGE_MINOLTA_TAG "[MINOLTA]"
#define MESSAGE_TV_TAG "[TV]"
#define MESSAGE_WBA_TAG "[WBA]"
#define MESSAGE_PCMESSENGER_TAG "[PCMESSENGER]"
#define MESSAGE_PC_TAG "[PC]"

#define PRINT_DEBUG_MESSAGE_CRITICAL(strTag, strMessage) qCritical() << strTag  << " " << strMessage //<< "  FILE : " << __FILE__ << "  LINE : " << __LINE__
#define PRINT_DEBUG_MESSAGE_WARNING(strTag, strMessage) qWarning() << strTag  << " " << strMessage //<< "  FILE : " << __FILE__ << "  LINE : " << __LINE__
#define PRINT_DEBUG_MESSAGE_DEBUG(strTag, strMessage) qDebug() << strTag << " " << strMessage //<< "  FILE : " << __FILE__ << "  LINE : " << __LINE__
#define PRINT_DEBUG_MESSAGE_INFO(strTag, strMessage) qDebug() << strTag << " " << strMessage //<< "  FILE : " << __FILE__ << "  LINE : " << __LINE__

#define ERROR_INVALID_INPUT_PARAMETER "Check input parameter"
#define ERROR_CANNOT_OPEN_SERIAL_PORT "Serial Port cannot be opened"
#define ERROR_WRITE_REQUEST_TIMEOUT "Write request timeout"
#define ERROR_READ_REQUEST_TIMEOUT "Read request timeout"
#define ERROR_CHECK_CONNECTION "Check connection"
#define ERROR_WRONG_SERIAL_PORT "Wrong Serial Port"
#define ERROR_WBA_REACH_RGB_GAIN_OFFSET_LIMIT "Reach RGB Gain Offset Limit"
#define ERROR_WBA_TIMEOUT "WBA Timeout"
#define ERROR_SOCKET_NULL "Socket is NULL"
#define ERROR_SOCKET_SEND "Unable to send data via socket"
#define ERROR_UNDEFINED "Undefined Error"
#define ERROR_ALLOCATION_ERROR "Allocation Error"
#define ERROR_MINOLTA_READ_ERROR "Minolta Read Error"
#define ERROR_UNDEFINED_JSON_MESSAGE "Undefined JSON Message "
#define ERROR_JSON_DOCUMENT_EMPTY "Empty JSON Document"


#define WARNING_ALREADY_CONNECTED "Already connected"
#define WARNING_ALREADY_DISCONNECTED "Already disconnected"

#define INFO_CONNECTED "Connected"

#define UNIT_CONVERSION_SEC_TO_MSEC 1000

#endif // TVSERIALCOMMDEF_H

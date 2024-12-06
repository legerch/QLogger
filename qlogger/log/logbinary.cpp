#include "logbinary.h"

/*****************************/
/* Class documentations      */
/*****************************/

/*****************************/
/* Enum documentations       */
/*****************************/

/*****************************/
/* Structure documentations  */
/*****************************/

/*****************************/
/* Signals documentations    */
/*****************************/

/*****************************/
/* Macro definitions         */
/*****************************/

/*****************************/
/* Start namespace           */
/*****************************/

namespace QLogger{

/*****************************/
/* Constants definitions     */
/*****************************/

/*****************************/
/* Functions implementation  */
/*         Class             */
/*****************************/

LogBinary::LogBinary(QtMsgType idType, const QByteArray &data)
    : m_idType(idType), m_data(data)
{
    /* Nothing to do */
}

QtMsgType LogBinary::getType() const
{
    return m_idType;
}

qsizetype LogBinary::getSizeBytes() const
{
    return m_data.size();
}

const QByteArray &LogBinary::getData() const
{
    return m_data;
}

QTextStream& operator<<(QTextStream &stream, const LogBinary &logBinary)
{
    stream << logBinary.m_data;
    return stream;
}

/*****************************/
/* Functions implementation  */
/*        Structures         */
/*****************************/

/*****************************/
/* End namespace             */
/*****************************/

} // namespace QLogger

/*****************************/
/* End file                  */
/*****************************/

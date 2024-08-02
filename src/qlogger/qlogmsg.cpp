#include "qlogmsg.h"

#include <QDate>
#include <QFileInfo>

/*****************************/
/* Class documentations      */
/*****************************/

/*!
 * \class QLogger::QLogMsg
 * \brief Class use to manage a log message.
 *
 * This class is used to properly format a log message and perform
 * multiple operations (like calculating log message size in bytes for example).
 *
 * Is is important to note that log message format will differ according to
 * type of build used:
 * - \b Release: <tt>[utc-date][type-log] log-msg</tt>
 * - \b Debug: <tt>[utc-date][type-log] log-msg (cpp-file:cpp-line, cpp-function)</tt>
 * 
 * If source file informations must appears on logs even on \b release mode, please use
 * the associated macro: \c QT_MESSAGELOGCONTEXT \n
 * See https://doc.qt.io/qt-6/qmessagelogcontext.html for more details
 */

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
/* Functions implementation  */
/*****************************/

/*!
 * \brief Use to create a log message
 * \details
 * This will format log message according to available infos
 * and calculated estimated size in bytes.
 * 
 * \param[in] idType
 * Type of log message
 * \param[in] context
 * Log context informations
 * \param[in] msg
 * Log message to create
 */
QLogMsg::QLogMsg(QtMsgType idType, const QMessageLogContext &context, const QString &msg)
{
    format(idType, context, msg);
    calcSizeInBytes();
}

/*!
 * \brief Format message according to available log informations
 * \details
 * This method will format log message according to available
 * context informations. \n
 * Please refer to this class documentation for more details on this
 * behaviour
 *
 * \param[in] idType
 * Type of log message
 * \param[in] context
 * Log context informations
 * \param[in] msg
 * Log message to create
 */
void QLogMsg::format(QtMsgType idType, const QMessageLogContext &context, const QString &msg)
{
    /* Add message log */
    m_msg = QString("[%1][%2] %3")
        .arg(QDateTime::currentDateTimeUtc().toString(Qt::ISODate), qtMsgTypeToString(idType), msg);

    /* Do we have context informations ? */
    if(context.file){
        m_msg += QString(" (%1:%2, %3)")
            .arg(QFileInfo(context.file).fileName())
            .arg(context.line)
            .arg(context.function);
    }

    m_msg += '\n';
}

/*!
 * \brief Use to calculate size in bytes
 * of the formatted log message
 */
void QLogMsg::calcSizeInBytes()
{
    m_sizeBytes = m_msg.toUtf8().size();
}

QTextStream& operator<<(QTextStream &stream, const QLogMsg &logMsg)
{
    stream << logMsg.m_msg;
    return stream;
}

/*!
 * \brief Use to convert Qt log message type
 * to string.
 *
 * \param[in] idType
 * Qt log message type to convert.
 *
 * \return
 * Return lower case string equivalent of
 * message type.
 */
QString QLogMsg::qtMsgTypeToString(QtMsgType idType)
{
    static const QMap<QtMsgType, QString> MAP_MSG_TYPE_TO_STR =
    {
        {QtDebugMsg, "debug"},
        {QtInfoMsg, "info"},
        {QtWarningMsg, "warning"},
        {QtCriticalMsg, "critical"},
        {QtFatalMsg, "fatal"},
        {QtSystemMsg, "system"}
    };

    return MAP_MSG_TYPE_TO_STR.value(idType, "unknown");
}

/*!
 * \brief Use to get log formatted message
 * string.
 *
 * \return
 * Return log formatted message string.
 *
 * \sa getSizeInBytes()
 */
const QString& QLogMsg::getString() const
{
    return m_msg;
}

/*!
 * \brief Use to get size in bytes of
 * the log formatted messages.
 *
 * \note
 * Method calcSizeInBytes() must has been
 * used, otherwise this value will be unacurate.
 *
 * \return
 * Return log message size in bytes.
 *
 * \sa getString()
 * \sa calcSizeInBytes()
 */
qsizetype QLogMsg::getSizeInBytes() const
{
    return m_sizeBytes;
}

/*****************************/
/* End namespace             */
/*****************************/

} // Namespace QLogger

/*****************************/
/* End of file               */
/*****************************/

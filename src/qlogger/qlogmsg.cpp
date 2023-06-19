#include "qlogmsg.h"

#include <QDate>
#include <QFileInfo>

QLogMsg::QLogMsg(QtMsgType idType, const QMessageLogContext &context, const QString &msg)
{
    format(idType, context, msg);
    calcSizeInBytes();
}

//TODO: doc (use to manage case for logcontext not complete in release mode if macro is undefined)
void QLogMsg::format(QtMsgType idType, const QMessageLogContext &context, const QString &msg)
{
#if defined(QT_MESSAGELOGCONTEXT)
    static const QString logPattern = QString("[%1][%2] %3 (%4:%5, %6)\n");

    m_msg = logPattern
        .arg(QDateTime::currentDateTimeUtc().toString(Qt::ISODate))
        .arg(qtMsgTypeToString(idType), msg)
        .arg(QFileInfo(context.file).fileName())
        .arg(context.line)
        .arg(context.function);
#else
    static const QString logPattern = QString("[%1][%2] %3\n");

    m_msg = logPattern
        .arg(QDateTime::currentDateTimeUtc().toString(Qt::ISODate))
        .arg(qtMsgTypeToString(idType), msg);
#endif
}

void QLogMsg::calcSizeInBytes()
{
    m_sizeBytes = m_msg.toUtf8().size();
}

QTextStream& operator<<(QTextStream &stream, const QLogMsg &logMsg)
{
    stream << logMsg.m_msg;
    return stream;
}

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

const QString& QLogMsg::getString() const
{
    return m_msg;
}

qsizetype QLogMsg::getSizeInBytes() const
{
    return m_sizeBytes;
}

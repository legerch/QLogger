#ifndef QLOGMSG_H
#define QLOGMSG_H

#include <QDebug>
#include <QString>

namespace QLogger{

class QLogMsg
{

public:
    explicit QLogMsg(QtMsgType idType, const QMessageLogContext &context, const QString &msg);

public:
    const QString& getString() const;
    qsizetype getSizeInBytes() const;

public:
    friend QTextStream& operator<<(QTextStream &stream, const QLogMsg &logMsg);

private:
    void format(QtMsgType idType, const QMessageLogContext &context, const QString &msg);
    void calcSizeInBytes();

private:
    static QString qtMsgTypeToString(QtMsgType idType);

private:
    QString m_msg;
    qsizetype m_sizeBytes;
};

} // Namespace QLogger

#endif // QLOGMSG_H

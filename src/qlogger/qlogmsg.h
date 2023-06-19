#ifndef QLOGMSG_H
#define QLOGMSG_H

#include <QDebug>
#include <QString>

class QLogMsg
{

public:
    explicit QLogMsg(QtMsgType idType, const QMessageLogContext &context, const QString &msg);

public:
    friend QTextStream& operator<<(QTextStream &stream, const QLogMsg &logMsg);

private:
    void format(QtMsgType idType, const QMessageLogContext &context, const QString &msg);

private:
    static QString qtMsgTypeToString(QtMsgType idType);

private:
    QString m_msg;
};

#endif // QLOGMSG_H

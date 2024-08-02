#ifndef LOGBINARY_H
#define LOGBINARY_H

#include <QByteArray>
#include <QTextStream>

namespace QLogger
{

class LogBinary
{

public:
    LogBinary(QtMsgType idType, const QByteArray &data);

public:
    QtMsgType getType() const;
    qsizetype getSizeBytes() const;

    const QByteArray& getData() const;

public:
    friend QTextStream& operator<<(QTextStream &stream, const LogBinary &logBinary);

private:
    QtMsgType m_idType;
    QByteArray m_data;
};

} // Namespace QLogger

#endif // LOGBINARY_H

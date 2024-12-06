#ifndef QLOGGER_LOGBINARY_H
#define QLOGGER_LOGBINARY_H

#include "qlogger/qloggerglobal.h"

#include <QByteArray>
#include <QTextStream>

namespace QLogger
{

class QLOGGER_EXPORT LogBinary
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

#endif // QLOGGER_LOGBINARY_H

#ifndef QLOGGER_LOGFORMATTER_H
#define QLOGGER_LOGFORMATTER_H

#include "logentry.h"
#include "logbinary.h"

namespace QLogger
{

class LogFormatter
{

public:
    using CbFormatter = std::function<QString(const LogEntry &log)>;

public:
    static LogBinary format(const LogEntry &log);
    static void setCustomFormat(CbFormatter formatter);

private:
    static QString defaultFormatter(const LogEntry &log);

private:
    static CbFormatter m_formatter;
};

} // Namespace QLogger

#endif // QLOGGER_LOGFORMATTER_H

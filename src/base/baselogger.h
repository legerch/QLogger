#ifndef BASELOGGER_H
#define BASELOGGER_H

#include "log/logbinary.h"

namespace QLogger
{

class BaseLogger
{

public:
    BaseLogger(bool enableConsole);
    virtual ~BaseLogger() = default;

public:
    void setLevel(QtMsgType idType);
    void writeLog(const LogBinary &log);

protected:
    virtual void write(const LogBinary &log) = 0;
    virtual void flush() = 0;

private:
    void printToConsole(const LogBinary &log);

private:
    QtMsgType m_level = QtDebugMsg;
    bool m_enableConsole;
};

} // Namespace QLogger

#endif // BASELOGGER_H

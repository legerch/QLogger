#ifndef BASELOGGER_H
#define BASELOGGER_H

#include "qlogger/qlogmsg.h"

namespace QLogger
{

class BaseLogger
{

public:
    BaseLogger();
    virtual ~BaseLogger() = default;

public:
    void setLevel(QtMsgType idType);
    void writeLog(const QLogMsg &msg);

protected:
    virtual void proceedEntry(const QLogMsg &msg) = 0;
    virtual void flush() = 0;

private:
    QtMsgType m_level = QtDebugMsg;
};

} // Namespace QLogger

#endif // BASELOGGER_H

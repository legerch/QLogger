#ifndef QLOGGER_BASELOGGER_H
#define QLOGGER_BASELOGGER_H

#include <QHash>
#include <QObject>

#include "qlogger/log/logbinary.h"

namespace QLogger
{

class BaseLogger : public QObject
{
    Q_OBJECT

public:
    BaseLogger(bool enableConsole, QObject *parent = nullptr);
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
    int m_minPriority = 0;
    bool m_enableConsole;

private:
    static const QHash<QtMsgType, int> MAP_LEVEL_PRIORITY;
};

} // Namespace QLogger

#endif // QLOGGER_BASELOGGER_H

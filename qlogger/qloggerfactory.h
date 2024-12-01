#ifndef QLOGGER_QLOGGERFACTORY_H
#define QLOGGER_QLOGGERFACTORY_H

#include "qloggerglobal.h"
#include "base/baselogger.h"
#include "log/logformatter.h"

#include <memory>

#include <QFileInfo>
#include <QMutex>
#include <QVersionNumber>

namespace QLogger
{

class QLOGGER_EXPORT QLoggerFactory final
{
    QLOGGER_DISABLE_COPY_MOVE(QLoggerFactory)

private:
    typedef std::unique_ptr<BaseLogger> LoggerPtr;

public:
    void initLoggerDaily(const QFileInfo &file, const QTime &time, uint maxFiles, bool enableConsole);
    void initLoggerRotating(const QFileInfo &file, int maxFiles, qint64 maxSize, bool enableConsole);
    void desinit();

public:
    void setLevel(QtMsgType idType);

public:
    static QLoggerFactory& instance();
    static const QVersionNumber& getLibraryVersion();

private:
    void initGeneric();
    void proceedMessage(QtMsgType idType, const QMessageLogContext &context, const QString &msg);

private:
    QLoggerFactory();

private:
    static void messageHandler(QtMsgType idType, const QMessageLogContext &context, const QString &msg);

private:
    LoggerPtr m_logger;
    QMutex m_mutex;
};

} // namespace QLogger

#endif // QLOGGER_QLOGGERFACTORY_H

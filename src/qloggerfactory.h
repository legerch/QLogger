#ifndef QLOGGERFACTORY_H
#define QLOGGERFACTORY_H

#include "qloggerglobal.h"
#include "base/baselogger.h"
#include "log/logformatter.h"

#include <memory>

#include <QFileInfo>
#include <QMutex>

namespace QLogger
{

class QLoggerFactory final
{
    QLOGGER_DISABLE_COPY_MOVE(QLoggerFactory)

private:
    typedef std::unique_ptr<BaseLogger> LoggerPtr;

public:
    void initLoggerRotating(const QFileInfo &file, int maxFiles, qint64 maxSize, bool enableConsole);
    void desinit();

public:
    void setLevel(QtMsgType idType);

public:
    static QLoggerFactory &instance();

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

#endif // QLOGGERFACTORY_H

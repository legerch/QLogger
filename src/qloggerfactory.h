#ifndef QLOGGERFACTORY_H
#define QLOGGERFACTORY_H

#include "qloggerglobal.h"
#include "base/baselogger.h"

#include <memory>

#include <QFileInfo>

namespace QLogger
{

class QLoggerFactory
{
    QLOGGER_DISABLE_COPY_MOVE(QLoggerFactory)

public:
    typedef std::unique_ptr<BaseLogger> LoggerPtr;

public:
    void initLoggerRotating(const QFileInfo &file, int maxFiles, qint64 maxSize);
    void desinit();

private:
    QLoggerFactory();
    ~QLoggerFactory();

private:
    LoggerPtr m_logger;
};

} // namespace QLogger

#endif // QLOGGERFACTORY_H

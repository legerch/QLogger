#include "qloggerfactory.h"

#include "sinks/filerotatinglogger.h"

/*****************************/
/* Class documentations      */
/*****************************/

/*****************************/
/* Enum documentations       */
/*****************************/

/*****************************/
/* Structure documentations  */
/*****************************/

/*****************************/
/* Signals documentations    */
/*****************************/

/*****************************/
/* Macro definitions         */
/*****************************/

/*****************************/
/* Start namespace           */
/*****************************/

namespace QLogger{

QLoggerFactory::QLoggerFactory()
{
    /* Nothing to do */
}

void QLoggerFactory::initLoggerRotating(const QFileInfo &file, int maxFiles, qint64 maxSize)
{
    m_logger = std::make_unique<FileRotatingLogger>(file, maxFiles, maxSize);
    initGeneric();
}

void QLoggerFactory::desinit()
{
    m_logger.reset();
    qInstallMessageHandler(0);
}

QLoggerFactory &QLoggerFactory::instance()
{
    static QLoggerFactory instance;
    return instance;
}

void QLoggerFactory::initGeneric()
{
    qInstallMessageHandler(messageHandler);
}

void QLoggerFactory::proceedMessage(QtMsgType idType, const QMessageLogContext &context, const QString &msg)
{
    QMutexLocker locker(&m_mutex);

    /* Prepare log message */
    const LogEntry logEntry(idType, context, msg);
    const LogBinary logBin = LogFormatter::format(logEntry);

    /* Write log message */
    m_logger->writeLog(logBin);
}

void QLoggerFactory::messageHandler(QtMsgType idType, const QMessageLogContext &context, const QString &msg)
{
    QLoggerFactory &logHandler = QLoggerFactory::instance();
    logHandler.proceedMessage(idType, context, msg);
}

/*****************************/
/* Constants definitions     */
/*****************************/

/*****************************/
/* Functions implementation  */
/*         Class             */
/*****************************/



/*****************************/
/* Functions implementation  */
/*        Structures         */
/*****************************/

/*****************************/
/* End namespace             */
/*****************************/

} // namespace QLogger

/*****************************/
/* End file                  */
/*****************************/

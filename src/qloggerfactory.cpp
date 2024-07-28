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

QLoggerFactory::~QLoggerFactory()
{
    desinit();
}

void QLoggerFactory::initLoggerRotating(const QFileInfo &file, int maxFiles, qint64 maxSize)
{
    m_logger = std::make_unique<FileRotatingLogger>(file, maxFiles, maxSize);
    initGeneric();
}

void QLoggerFactory::desinit()
{
    //TODO: maybe close used files here ?
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
    const QLogMsg fmtMsg(idType, context, msg);
    m_logger->writeLog(fmtMsg);

    //TODO: manage log level
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

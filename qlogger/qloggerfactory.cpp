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

/*!
 * \brief Use to configure rotating file log sink
 *
 * \param[in] file
 * Path of log file to use. \n
 * \em Examples:
 * - <tt>customlog.txt</tt>: Will create log messages
 * in current directory and use \c customlog as pattern
 * to use for the rotation
 * - <tt>logs/customlog.txt: Will create custom log messages
 * in \c logs directory and use \c customlog as pattern
 * to use for the rotation
 * \param[in] maxFiles
 * Number of file to use for rotation. \n
 * For example, if set to \c 3, no more than 3
 * files will be used for rotation.
 * \param[in] maxFileSize
 * Maximum log file size in bytes.
 * \param[in] enableConsole
 * Set to \c true to enable console/terminal printing (helpful
 * during development, useless in \em release mode)
 *
 * \note
 * If any sinks already exist, it will be properly deleted.
 *
 * \sa desinit()
 */
void QLoggerFactory::initLoggerRotating(const QFileInfo &file, int maxFiles, qint64 maxSize, bool enableConsole)
{
    m_logger = std::make_unique<FileRotatingLogger>(file, maxFiles, maxSize, enableConsole);
    initGeneric();
}

void QLoggerFactory::desinit()
{
    m_logger.reset();
    qInstallMessageHandler(0);
}

void QLoggerFactory::setLevel(QtMsgType idType)
{
    QMutexLocker locker(&m_mutex);
    m_logger->setLevel(idType);
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

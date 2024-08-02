#include "logformatter.h"

#include <QDateTime>

/*****************************/
/* Class documentations      */
/*****************************/

/*!
 * \class QLogger::LogFormatter
 * \brief Use to format log
 * \details
 * This class allow to format \c QLogger::LogEntry to a
 * \c QLogger::LogBinary using a custom formatter. \n
 *
 * By default, log entries will be formatted to:
 * - Without context informations: <tt>[utc-date][type-log] log-msg</tt>
 * - With context informations: <tt>[utc-date][type-log] log-msg (cpp-file:cpp-line, cpp-function)</tt>
 *
 * For more details about context informations, please refer to \c QLogger::LogEntry
 *
 * To use a custom formatter, use \c setCustomFormat()
 */

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

/*****************************/
/* Constants definitions     */
/*****************************/
LogFormatter::CbFormatter LogFormatter::m_formatter = defaultFormatter;

/*****************************/
/* Functions implementation  */
/*         Class             */
/*****************************/

LogBinary LogFormatter::format(const LogEntry &log)
{
    const QString fmtLog = m_formatter(log);
    return LogBinary(log.getType(), fmtLog.toUtf8());
}

/*!
 * \brief Register a custom formatter
 * \param formatter
 * Formatter to use.
 */
void LogFormatter::setCustomFormat(CbFormatter formatter)
{
    m_formatter = formatter;
}

QString LogFormatter::defaultFormatter(const LogEntry &log)
{
    /* Add log main informations */
    QString fmt = QString("[%1][%2] %3").arg(
        QDateTime::currentDateTimeUtc().toString(Qt::ISODate),
        log.getTypeString(),
        log.getMsg()
    );

    /* Can we add context informations ? */
    if(log.contextIsAvailable()){
        fmt += QString(" (%1:%2, %3)")
                   .arg(log.getCtxFile().fileName())
                   .arg(log.getCtxLine())
                   .arg(log.getCtxFctSig());
    }

    /* Terminate log line */
    fmt += '\n';

    return fmt;
}

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

#include "logentry.h"

#include <QMap>
#include <QRegularExpression>

/*****************************/
/* Class documentations      */
/*****************************/

/*!
 * \class QLogger::LogEntry
 * \brief Used to store all log entry informations
 * \details
 * This class is used to store all informations related to a log entry, including
 * his context, if available.
 *
 * By default, context informations are only available in \b debug builds. \n
 * If context informations are needed even on \c release mode, please use associated
 * macro: \c QT_MESSAGELOGCONTEXT \n
 * See https://doc.qt.io/qt-6/qmessagelogcontext.html for more details
 *
 * \sa QLogger::LogFormatter
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

/*****************************/
/* Functions implementation  */
/*         Class             */
/*****************************/

LogEntry::LogEntry(QtMsgType idType, const QMessageLogContext &context, const QString &msg)
{
    /* Copy log entry */
    m_idType = idType;
    m_msg = msg;

    /* Do we have context informations */
    m_haveContext = (context.file == nullptr ? false : true);
    if(m_haveContext){
        m_ctxFile = QFileInfo(context.file);
        m_ctxLine = context.line;
        m_ctxFctSig = context.function;
    }
}

QtMsgType LogEntry::getType() const
{
    return m_idType;
}

/*!
 * \brief Get string type
 *
 * \return
 * Return type of log entry in string format. \n
 * String will be in \em lowercase.
 */
QString LogEntry::getTypeString() const
{
    /* Create associated type map */
    static const QMap<QtMsgType, QString> MAP_MSG_TYPE_TO_STR =
    {
        {QtDebugMsg, "debug"},
        {QtInfoMsg, "info"},
        {QtWarningMsg, "warning"},
        {QtCriticalMsg, "critical"},
        {QtFatalMsg, "fatal"},
        {QtSystemMsg, "system"}
    };

    /* Return associated type string */
    return MAP_MSG_TYPE_TO_STR.value(m_idType, "unknown");
}

/*!
 * \brief Use to know if context informations are available
 *
 * \return
 * Return \c true if context informations are available
 */
bool LogEntry::contextIsAvailable() const
{
    return m_haveContext;
}

/*!
 * \brief Get file of source code associated
 * to log entry
 *
 * \note
 * Context informations may not be available, please use
 * \c contextIsAvailable() before using this method
 *
 * \return
 * Returns file information related to log entry.
 *
 * \sa contextIsAvailable()
 */
const QFileInfo& LogEntry::getCtxFile() const
{
    return m_ctxFile;
}

/*!
 * \brief Get line of source code associated
 * to log entry
 *
 * \note
 * Context informations may not be available, please use
 * \c contextIsAvailable() before using this method
 *
 * \return
 * Returns line number.
 *
 * \sa contextIsAvailable()
 */
int LogEntry::getCtxLine() const
{
    return m_ctxLine;
}

/*!
 * \brief Get function signature
 * \details
 * Function signature will contains type of returned value and
 * all expected arguments types.
 *
 * \note
 * Context informations may not be available, please use
 * \c contextIsAvailable() before using this method
 *
 * \return
 * Return function signature
 *
 * \sa getCtxFctName()
 * \sa contextIsAvailable()
 */
const QString& LogEntry::getCtxFctSig() const
{
    return m_ctxFctSig;
}

/*!
 * \brief Get function name
 * \details
 * Since Qt doesn't provide function name context, we searched it
 * using regex. Function signature will be equals to: \n
 * \c MyNamespace::MyClass::myFunction(int, double)
 *
 * Details of our regex:
 * - Matched by <tt>(\w+::)*</tt>: <tt>MyNamespace::</tt>, <tt>MyClass::</tt>
 * - Matched by <tt>(\w+)</tt>: <tt>myFunction</tt>
 * - Matched by <tt>s*\(.*\)</tt>: <tt>(int, double)</tt>
 *
 * So, function name will be in the 2nd group. Then, since
 * index 0 of QRegularExpressionMatch is the enire substring captured
 * by the pattern, index to use will be 2 (0 + group 2). \n
 * For more details, see:
 *  https://doc.qt.io/qt-6/qregularexpressionmatch.html#captured
 *
 * \note
 * Context informations may not be available, please use
 * \c contextIsAvailable() before using this method
 *
 * \return
 * Returns function name
 *
 * \sa getCtxFctSig()
 * \sa contextIsAvailable()
 */
QString LogEntry::getCtxFctName() const
{
    /* Qt doesn't provide function name, so "manually" find it using regex */
    static const QRegularExpression regexFctName(R"((\w+::)*(\w+)\s*\(.*\))");

    /* Do we find matches ? */
    QRegularExpressionMatch match = regexFctName.match(m_ctxFctSig);
    if(!match.hasMatch()){
        return m_ctxFctSig; // No match found, return function signature
    }

    /* Name of function is in second index */
    return match.captured(2);
}

const QString& LogEntry::getMsg() const
{
    return m_msg;
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

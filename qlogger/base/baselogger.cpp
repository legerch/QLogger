#include "baselogger.h"

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

/*****************************/
/* Constants definitions     */
/*****************************/
const QHash<QtMsgType, int> BaseLogger::MAP_LEVEL_PRIORITY =
{
    {QtDebugMsg, 0},
    {QtInfoMsg, 1},
    {QtWarningMsg, 2},
    {QtCriticalMsg, 3},
    {QtFatalMsg, 4}
};

/*****************************/
/* Functions implementation  */
/*         Class             */
/*****************************/

BaseLogger::BaseLogger(bool enableConsole)
{
    m_enableConsole = enableConsole;
}

/*!
 * \brief Use to set minimum level of log message
 * to be managed.
 * \details
 * Since Qt log level doesn't use an enum ordered by priority, we use
 * a custom one. Order of priority will be (from low to high): \n
 * - QtDebugMsg
 * - QtInfoMsg
 * - QtWarningMsg
 * - QtCriticalMsg,
 * - QtFatalMsg
 *
 * So, for example, when minimum level is set to \c QtWarningMsg,
 * only <tt>WarningMsg, QtCriticalMsg and QtFatalMsg</tt> will be managed
 *
 * \param[in] idType
 * Minimum log level to use
 */
void BaseLogger::setLevel(QtMsgType idType)
{
    m_minPriority = MAP_LEVEL_PRIORITY.value(idType, 0);
}

void BaseLogger::writeLog(const LogBinary &log)
{
    /* Do minimum level of log is reached ? */
    const int currentPriority = MAP_LEVEL_PRIORITY.value(log.getType(), 0);
    if(currentPriority < m_minPriority){
        return;
    }

    /* Write logs */
    write(log);
    flush();

    /* Print to console if needed */
    if(m_enableConsole){
        printToConsole(log);
    }
}

void BaseLogger::printToConsole(const LogBinary &log)
{
    switch(log.getType())
    {
        case QtDebugMsg:
        case QtInfoMsg:{
            QTextStream out(stdout);
            out << log;
        }break;

        default:{
            QTextStream out(stderr);
            out << log;
        }break;
    }
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

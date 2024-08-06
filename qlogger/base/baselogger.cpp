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

/*****************************/
/* Functions implementation  */
/*         Class             */
/*****************************/

BaseLogger::BaseLogger(bool enableConsole)
{
    m_enableConsole = enableConsole;
}

void BaseLogger::setLevel(QtMsgType idType)
{
    m_level = idType;
}

void BaseLogger::writeLog(const LogBinary &log)
{
    /* Do minimum level of log is reached ? */
    if(log.getType() < m_level){
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

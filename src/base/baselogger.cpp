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

BaseLogger::BaseLogger()
{
    /* Nothing to do */
}

void BaseLogger::setLevel(QtMsgType idType)
{
    m_level = idType;
}

void BaseLogger::writeLog(const LogBinary &log)
{
    //TODO: manage log level

    write(log);
    flush();

    //TODO: manage console
    QTextStream out(stderr);
    out << log;
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

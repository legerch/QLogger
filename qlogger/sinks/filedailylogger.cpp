#include "filedailylogger.h"

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

FileDailyLogger::FileDailyLogger(const QFileInfo &file, const QTime &time, bool enableConsole, QObject *parent)
    : FileLogger(file, enableConsole, parent)
{
    /* Set properties */
    m_timeDaily = time;
}

QString FileDailyLogger::generateFmtBasename(const QVariant &arg) const
{
    QString fmtBasename = QString("%1_%2").arg(getBasename());

    /* Convert arg to date */
    const QDate date = arg.toDate();
    if(!date.isValid()){
        return fmtBasename.arg("date_invalid");
    }

    /* Build basename by adding date information */
    const QString fmtDate = date.toString("yyyy-MM-dd");
    return fmtBasename.arg(fmtDate);
}

void FileDailyLogger::start()
{
    /* Open log file to use */
    openFile(generateFmtBasename(QDate::currentDate()), false);
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

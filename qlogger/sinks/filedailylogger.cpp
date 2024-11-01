#include "filedailylogger.h"

/*****************************/
/* Class documentations      */
/*****************************/

//TODO: doc
/*!
 * Example with daily rotation set at 09:30 AM:
 * Application started at 11:00, filename
 * at current date, next rotation for tomorrow
 *
 * Application started at 2024-12-01 8:00 AM, filename
 * at current date (2024-12-01), next rotation in 1h30m
 * which will use the same file (since current day
 * is the same).
 * Then application kept running until 2024-12-02 09:30,
 * filename used will be changed at the rotation to
 * the current day 2024-12-02.
 *
 * TL;DR :
 * When started: always open CURRENT DATE filename.
 * Will rotate when next daily time is reached with the
 * current day.
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

FileDailyLogger::FileDailyLogger(const QFileInfo &file, const QTime &time, bool enableConsole, QObject *parent)
    : FileLogger(file, enableConsole, parent)
{
    /* Set properties */
    m_timeDaily = time;
}

QString FileDailyLogger::generateFmtBasename(const QVariant &arg) const
{
    QString fmtBasename = QString("%1_%2_%3").arg(getBasename());

    /* Convert arg to date */
    const QDate date = arg.toDate();
    if(!date.isValid()){
        return fmtBasename.arg("date", "invalid");
    }

    /* Build basename by adding date information */
    const QString fmtDate = date.toString("yyyy-MM-dd");
    const QString fmtTime = QString("%1-%2").arg(m_timeDaily.hour()).arg(m_timeDaily.minute());
    return fmtBasename.arg(fmtDate, fmtTime);
}

void FileDailyLogger::start()
{
    /* Start our first rotation */
    rotationPerform();
}

void FileDailyLogger::write(const LogBinary &log)
{
    /* Do we need to perform our daily rotation ? */
    if(QDateTime::currentDateTime() > m_nextRotation){
        rotationPerform();
    }

    /* Write log message */
    FileLogger::write(log);
}

void FileDailyLogger::rotationProgram()
{
    /* Set rotation time for current day */
    const QDateTime currentDateTime = QDateTime::currentDateTime();
    m_nextRotation = QDateTime(currentDateTime.date(), m_timeDaily);

    /* If daily time has passed for today, schedule for tomorrow */
    if(currentDateTime > m_nextRotation){
        m_nextRotation = m_nextRotation.addDays(1);
    }
}

void FileDailyLogger::rotationPerform()
{
    /* Perform rotation by opening new log file to use */
    closeFile();
    openFile(generateFmtBasename(QDate::currentDate()), false);

    /* Program the next rotation */
    rotationProgram();
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

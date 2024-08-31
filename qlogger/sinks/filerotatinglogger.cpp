#include "filerotatinglogger.h"

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

FileRotatingLogger::FileRotatingLogger(const QFileInfo &file, int maxFiles, qint64 maxSize, bool enableConsole, QObject *parent)
    : FileLogger(file, enableConsole, parent)
{
    /* Set properties */
    m_maxFileNb = maxFiles;
    m_maxFileSize = maxSize;
}

QString FileRotatingLogger::generateFmtBasename(const QVariant &arg) const
{
    QString fmtBasename = getBasename();

    /* Convert arg to integer */
    bool succeed = false;
    const int indexFile = arg.toInt(&succeed);
    if(!succeed){
        return fmtBasename;
    }

    /* Index is base file ? */
    if(indexFile == 0){
        return fmtBasename;
    }

    /* Build basename by using index */
    return fmtBasename.append(QString::number(indexFile));
}

void FileRotatingLogger::start()
{
    /* Open log file to use */
    openFile(generateFmtBasename(0), false);
}

void FileRotatingLogger::write(const LogBinary &log)
{
    /* Do file can contains this message ? */
    if(getFileSizeNext(log) > m_maxFileSize){
        rotateFiles();
    }

    /* Write log message */
    FileLogger::write(log);
}

bool FileRotatingLogger::rotateFiles()
{
    /* Close current file */
    closeFile();

    /* Rotate all files by renaming */
    bool succeed = true;
    for(int i = m_maxFileNb -1; i > 0; --i){
        const QString src = generateFilePath(i - 1);

        if(QFile::exists(src)){
            const QString target = generateFilePath(i);

            succeed &= renameFile(src, target);
            if(!succeed){
                break;
            }
        }
    }

    /* Open base file and truncate any existing data */
    succeed &= openFile(generateFmtBasename(0), true);
    return succeed;
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

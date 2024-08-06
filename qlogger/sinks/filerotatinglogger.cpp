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

FileRotatingLogger::FileRotatingLogger(const QFileInfo &file, int maxFiles, qint64 maxSize, bool enableConsole)
    : FileLogger(file.absoluteDir(), file.suffix(), enableConsole)
{
    /* Set properties */
    m_fileBasename = file.baseName();

    m_maxFileNb = maxFiles;
    m_maxFileSize = maxSize;

    /* Open log file to use */
    openFile(generateBasename(0), false);
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
        const QString src = generateFilepath(i - 1);

        if(QFile::exists(src)){
            const QString target = generateFilepath(i);

            succeed &= renameFile(src, target);
            if(!succeed){
                break;
            }
        }
    }

    /* Open base file and truncate any existing data */
    succeed &= openFile(generateBasename(0), true);
    return succeed;
}

QString FileRotatingLogger::generateBasename(int index) const
{
    /* Index is base file ? */
    if(index == 0){
        return m_fileBasename;
    }

    /* Build basename by using index */
    QString basename = m_fileBasename;
    return basename.append(QString::number(index));
}

QString FileRotatingLogger::generateFilepath(int index) const
{
    return getFilePath(generateBasename(index));
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

#include "filelogger.h"

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

FileLogger::FileLogger(const QDir &dir, const QString &extension, bool enableConsole)
    : BaseLogger(enableConsole)
{
    /* Set properties */
    m_dir = dir;
    m_extension = extension;

    /* Create directory if needed */
    if(!m_dir.exists()){
        m_dir.mkpath(".");
    }
}

bool FileLogger::openFile(const QString &basename, bool truncate)
{
    /* Assign file */
    const QFileInfo filePath(getFilePath(basename));
    m_file.setFileName(filePath.absoluteFilePath());

    /* Set open flags */
    QFile::OpenMode openFlags = QIODevice::WriteOnly | QIODevice::Text;
    if(truncate){
        openFlags |= QIODevice::Truncate;
    }else{
        openFlags |= QIODevice::Append;
    }

    /* Open it and prepare stream */
    bool succeed = m_file.open(openFlags);
    if(!succeed){
        m_fileSize = 0;
        return false;
    }

    m_fileSize = m_file.size();
    m_fileStream.setDevice(&m_file);

    return true;
}

void FileLogger::closeFile()
{
    m_file.close();
    m_fileSize = 0;
}

QString FileLogger::getFilePath(const QString &basename) const
{
    return m_dir.absoluteFilePath(basename + '.' + m_extension);
}

qsizetype FileLogger::getFileSize() const
{
    return m_fileSize;
}

qsizetype FileLogger::getFileSizeNext(const LogBinary &log) const
{
    return m_fileSize + log.getSizeBytes();
}

void FileLogger::write(const LogBinary &log)
{
    m_fileStream << log;
    m_fileSize = getFileSizeNext(log);
}

void FileLogger::flush()
{
    m_fileStream.flush();
}

bool FileLogger::renameFile(const QString &oldName, const QString &newName)
{
    /* Remove target file is exists */
    if(QFile::exists(newName)){
        QFile::remove(newName);
    }

    /* Rename file */
    return QFile::rename(oldName, newName);
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

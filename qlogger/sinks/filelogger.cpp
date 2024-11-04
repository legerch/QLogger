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

FileLogger::FileLogger(const QFileInfo &filepath, bool enableConsole, QObject *parent)
    : BaseLogger(enableConsole, parent)
{
    /* Set properties */
    m_dir = filepath.absoluteDir();
    m_basename = filepath.baseName();
    m_extension = filepath.suffix();

    /* Create directory if needed */
    if(!m_dir.exists()){
        m_dir.mkpath(".");
    }
}

bool FileLogger::openFile(const QString &fmtBasename, bool truncate)
{
    /* Assign file */
    const QFileInfo filePath(generateFilePathStr(fmtBasename));
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

const QDir &FileLogger::getDir() const
{
    return m_dir;
}

const QString& FileLogger::getBasename() const
{
    return m_basename;
}

const QString &FileLogger::getExtension() const
{
    return m_extension;
}

qsizetype FileLogger::getFileSize() const
{
    return m_fileSize;
}

qsizetype FileLogger::getFileSizeNext(const LogBinary &log) const
{
    return m_fileSize + log.getSizeBytes();
}

QString FileLogger::generateFilePathStr(const QString &fmtBasename) const
{
    return m_dir.absoluteFilePath(fmtBasename + '.' + m_extension);
}

QString FileLogger::generateFilePathVar(const QVariant &argBasename) const
{
    return generateFilePathStr(generateFmtBasename(argBasename));
}

QStringList FileLogger::listFilesEntries(QDir::SortFlags sort) const
{
    const QStringList namesPattern = {
        QString("*%1*.%2").arg(m_basename, m_extension)
    };

    return m_dir.entryList(
        namesPattern,
        QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot | QDir::Readable,
        sort
    );
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

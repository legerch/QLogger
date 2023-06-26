#include "qloghandler.h"

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

namespace QLogger
{

/*****************************/
/* Constants defintitions    */
/*****************************/

/*****************************/
/* Functions implementation  */
/*         Class             */
/*****************************/

QLogHandler::QLogHandler()
{
    /* Nothing to do */
}

QLogHandler::~QLogHandler()
{
    closeFile();
}

bool QLogHandler::init(const QString &logFilename, int maxFiles, qint64 maxFileSize, bool enableConsole)
{
    /* Fill logger basic informations */
    QFileInfo infoPattern(logFilename);

    m_enableConsole = enableConsole;

    m_maxFileSize = maxFileSize;
    m_maxFiles = maxFiles;
    m_fileBasename = infoPattern.baseName();
    m_fileExt = infoPattern.suffix();

    /* Create logs folder if needed */
    m_currentDir = infoPattern.dir();
    if(!m_currentDir.exists()){
        m_currentDir.mkdir(".");
    }

    /* Open file log to use */
    bool succeed = openFile(generateFilepath(0), false);
    if(!succeed){
        return false;
    }

    /* Install custom message handler */
    qInstallMessageHandler(messageHandler);
    return true;
}

void QLogHandler::desinit()
{
    closeFile();
}

QLogHandler &QLogHandler::instance()
{
    static QLogHandler instance;
    return instance;
}

bool QLogHandler::isInitialized() const
{
    return m_currentFile.isOpen();
}

QString QLogHandler::generateFilepath(int index) const
{
    QString filename = m_fileBasename;

    /* Index different from base ? */
    if(index != 0){
        filename.append(QString::number(index));
    }

    /* Return complete filepath */
    return m_currentDir.absoluteFilePath(filename + "." + m_fileExt);
}

bool QLogHandler::openFile(const QString &filepath, bool truncate)
{
    /* Assign file */
    m_currentFile.setFileName(filepath);

    /* Set open flags */
    QFile::OpenMode openFlags = QIODevice::WriteOnly | QIODevice::Text;
    if(truncate){
        openFlags |= QIODevice::Truncate;
    }else{
        openFlags |= QIODevice::Append;
    }

    /* Open it and prepare stream */
    bool succeed = m_currentFile.open(openFlags);
    if(!succeed){
        m_currentFileSize = 0;
        return false;
    }

    m_currentFileSize = m_currentFile.size();
    m_stream.setDevice(&m_currentFile);

    return true;
}

void QLogHandler::closeFile()
{
    if(m_currentFile.isOpen()){
        m_currentFile.close();
        m_currentFileSize = 0;
    }
}

bool QLogHandler::sizeFileLimitIsReached(const QLogMsg &logMsg) const
{
    const qsizetype nextFileSize = m_currentFileSize + logMsg.getSizeInBytes();

    /* Do log file limit is reached ? */
    if(nextFileSize > m_maxFileSize){
        return true;
    }

    return false;
}

bool QLogHandler::rotateFiles()
{
    /* Close current file */
    closeFile();

    /* Rotate all files by renaming */
    bool succeed = true;
    for(int i=m_maxFiles -1; i>0; --i){
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
    succeed &= openFile(generateFilepath(0), true);

    return succeed;
}

//TODO: doc (oldName must exist)
bool QLogHandler::renameFile(const QString &oldName, const QString &newName)
{
    /* Remove target file is exists */
    if(QFile::exists(newName)){
        QFile::remove(newName);
    }

    /* Rename file */
    return QFile::rename(oldName, newName);
}

void QLogHandler::proceedMessage(QtMsgType idType, const QMessageLogContext &context, const QString &msg)
{
    QMutexLocker locker(&m_mutex);

    /* Prepare log message */
    const QLogMsg fmtMsg(idType, context, msg);

    /* Do file can contains this message ? */
    if(sizeFileLimitIsReached(fmtMsg)){
        rotateFiles();
    }

    /* Log message */
    m_stream << fmtMsg;
    if(m_enableConsole){
        QLogHandler::messageToConsole(idType, fmtMsg);
    }

    /* Update current file size */
    m_currentFileSize += fmtMsg.getSizeInBytes();
}

void QLogHandler::messageHandler(QtMsgType idType, const QMessageLogContext &context, const QString &msg)
{
    QLogHandler &logHandler = QLogHandler::instance();
    logHandler.proceedMessage(idType, context, msg);
}

void QLogHandler::messageToConsole(QtMsgType idType, const QLogMsg &fmtMsg)
{
    switch(idType)
    {
        case QtDebugMsg:
        case QtInfoMsg:{
            QTextStream out(stdout);
            out << fmtMsg;
        }break;

        default:{
            QTextStream out(stderr);
            out << fmtMsg;
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

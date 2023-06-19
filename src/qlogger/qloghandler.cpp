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
        return false;
    }

    m_stream.setDevice(&m_currentFile);

    return true;
}

void QLogHandler::closeFile()
{
    if(m_currentFile.isOpen()){
        m_currentFile.close();
    }
}

bool QLogHandler::sizeFileIsUnderLimit() const
{
    return m_currentFile.size() < m_maxFileSize;
}

bool QLogHandler::rotateFiles()
{
    /* Close current file */
    closeFile();

    /* Rotate all files by renaming */
    bool succeed = true;
    for(int i=m_maxFiles; i>0; --i){
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

void QLogHandler::messageHandler(QtMsgType idType, const QMessageLogContext &context, const QString &msg)
{
    QLogHandler &logHandler = QLogHandler::instance();
    QMutexLocker locker(&logHandler.m_mutex);

    /* Write to file */
    const QString fmtMsg = messageFormat(idType, context, msg);
    logHandler.m_stream << fmtMsg;

    /* Do file size is still valid */
    if(!logHandler.sizeFileIsUnderLimit()){
        logHandler.rotateFiles();
    }

    /* Display log message to console if enable */
    if(logHandler.m_enableConsole){
        messageToConsole(idType, fmtMsg);
    }
}

void QLogHandler::messageToConsole(QtMsgType idType, const QString &fmtMsg)
{
    switch(idType)
    {
        case QtDebugMsg:
        case QtInfoMsg:{
            QTextStream(stdout) << fmtMsg;
        }break;

        default:{
            QTextStream(stderr) << fmtMsg;
        }break;
    }
}

//TODO: doc (use to manage case for logcontext not complete in release mode if macro is undefined)
QString QLogHandler::messageFormat(QtMsgType idType, const QMessageLogContext &context, const QString &msg)
{
#if defined(QT_MESSAGELOGCONTEXT)
    static const QString logPattern = QString("[%1][%2] %3 (%4:%5, %6)\n");
    return logPattern
        .arg(QDateTime::currentDateTimeUtc().toString(Qt::ISODate))
        .arg(qtMsgTypeToString(idType), msg)
        .arg(QFileInfo(context.file).fileName())
        .arg(context.line)
        .arg(context.function);
#else
    static const QString logPattern = QString("[%1][%2] %3\n");
    return logPattern
        .arg(QDateTime::currentDateTimeUtc().toString(Qt::ISODate))
        .arg(qtMsgTypeToString(idType), msg);
#endif
}

QString QLogHandler::qtMsgTypeToString(QtMsgType idType)
{
    static const QMap<QtMsgType, QString> MAP_MSG_TYPE_TO_STR =
    {
        {QtDebugMsg, "debug"},
        {QtInfoMsg, "info"},
        {QtWarningMsg, "warning"},
        {QtCriticalMsg, "critical"},
        {QtFatalMsg, "fatal"},
        {QtSystemMsg, "system"}
    };

    return MAP_MSG_TYPE_TO_STR.value(idType, "unknown");
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

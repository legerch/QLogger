#include "qloghandler.h"

/*****************************/
/* Class documentations      */
/*****************************/

/*!
 * \class QLogger::QLogHandler
 * \brief Class use to handle all log messages.
 *
 * This class is used to properly handle log files rotation.
 *
 * \sa init()
 * \sa QLogger::QLogMsg
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

namespace QLogger
{

/*****************************/
/* Constants defintitions    */
/*****************************/

/*****************************/
/* Functions implementation  */
/*         Class             */
/*****************************/

/*!
 * \brief Construct a QLogHandler object
 * \details
 * This method perform \b nothing
 */
QLogHandler::QLogHandler()
{
    /* Nothing to do */
}

/*!
 * \brief Destructor of QLogHandler instance
 * \details
 * This method will close current log file
 * (if any!) *
 */
QLogHandler::~QLogHandler()
{
    closeFile();
}

/*!
 * \brief Use to configure log handler
 *
 * \param[in] logFilename
 * Path of log file to use. \n
 * \em Examples:
 * - <tt>customlog.txt</tt>: Will create log messages
 * in current directory and use \c customlog as pattern
 * to use for the rotation
 * - <tt>logs/customlog.txt: Will create custom log messages
 * in \c logs directory and use \c customlog as pattern
 * to use for the rotation
 * \param[in] maxFiles
 * Number of file to use for rotation. \n
 * For example, if set to \c 3, no more than 3
 * files will be used for rotation.
 * \param[in] maxFileSize
 * Maximum log file size in bytes.
 * \param[in] enableConsole
 * Set to \c true to enable console/terminal printing (helpful
 * during development, useless in \em release mode)
 *
 * \return
 * Return \c true if initialization succeed. \n
 * This method can failed if unable to open first log message
 * (right access, hard drive full, etc...)
 *
 * \sa desinit()
 */
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

/*!
 * \brief Use to release (and close) all used ressources
 * \details
 * This method will close all used ressources and restore
 * default log behaviour.
 *
 * \sa init()
 */
void QLogHandler::desinit()
{
    closeFile();
}

/*!
 * \brief Use to get current unique instance
 * of QLogHandler
 *
 * \return
 * Return current unique instance
 * of QLogHandler
 */
QLogHandler &QLogHandler::instance()
{
    static QLogHandler instance;
    return instance;
}

/*!
 * \brief Use to verify than log handler
 * is initialized
 * \details
 * To know initialization status, this method will
 * verify if log file is currently open or not.
 *
 * \return
 * Return \c true if log handler is initialized
 */
bool QLogHandler::isInitialized() const
{
    return m_currentFile.isOpen();
}

/*!
 * \brief Use to generate filepath of a log
 * file based on needed index
 * \details
 * An index of 0 will use filepath pattern defined
 * with init() method. \n
 * In case of any other indexes, filepath pattern will
 * still be used but a number will be added before
 * file extension.
 *
 * \param[in] index
 * File index to use
 *
 * \return
 * Return pathfile to use for choosen index
 */
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

/*!
 * \brief Use to open file with write access
 *
 * \param[in] filepath
 * Path to file to open. \n
 * If fill doesn't exist, it will be created.
 * \param[in] truncate
 * Set to \c true to clear any previous data of
 * the file.
 *
 * \note
 * This method do not verify than previous
 * stream file has been closed before opening
 * another.
 *
 * \return
 * Return \c true if operation succeed.
 *
 * \sa closeFile()
 */
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

/*!
 * \brief Use to close current log file
 * \details
 * This method will close (and flush) current log
 * file (only if file is currently open)
 *
 * \sa openFile()
 */
void QLogHandler::closeFile()
{
    if(m_currentFile.isOpen()){
        m_currentFile.close();
        m_currentFileSize = 0;
    }
}

/*!
 * \brief Use to verify if current log
 * file will reach file limit size with this
 * message.
 *
 * \param[in] logMsg
 * Log message to check.
 *
 * \return
 * Return \c true if adding \c logMsg to current log
 * file will reach file limit size.
 *
 * \sa rotateFiles()
 */
bool QLogHandler::sizeFileLimitIsReached(const QLogMsg &logMsg) const
{
    const qsizetype nextFileSize = m_currentFileSize + logMsg.getSizeInBytes();

    /* Do log file limit is reached ? */
    if(nextFileSize > m_maxFileSize){
        return true;
    }

    return false;
}

/*!
 * \brief Use to rotate log files
 * \details
 * This method will:
 * - Close current log file (to flush any pending data)
 * - Rename "logFile N" to "logFile N+1"
 * - Open next log file to write into
 *
 * \return
 * Return \c true if rotation file succeed.
 *
 * \sa sizeFileLimitIsReached()
 */
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

/*!
 * \brief Use this method to rename a file
 *
 * \param[in] oldName
 * Source file to rename. \n
 * This filepath must exist, otherwise renaming
 * will failed.
 * \param[in] newName
 * New name of the file. \n
 * If this file exist, it will be removed before proceeding
 * to rename operation.
 *
 * \return
 * Return \c true if \c oldName has been renamed to
 * \c newName
 */
bool QLogHandler::renameFile(const QString &oldName, const QString &newName)
{
    /* Remove target file is exists */
    if(QFile::exists(newName)){
        QFile::remove(newName);
    }

    /* Rename file */
    return QFile::rename(oldName, newName);
}

/*!
 * \brief Main log handler operation which
 * gonna "proceed" the log message
 * \details
 * This method will:
 * - Check if message can be added to current
 * log file (if not, files will be rotated
 * - Write log to file (and console if enable)
 *
 * \param[in] idType
 * Type of log message
 * \param[in] context
 * Log context informations
 * \param[in] msg
 * Log message to create
 */
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

/*!
 * \brief Static method used to register
 * into qInstallMessageHandler() method.
 * \details
 * For more details, please se Qt documentation at:
 * https://doc.qt.io/qt-5/qtglobal.html#qInstallMessageHandler
 *
 * \param[in] idType
 * Type of log message
 * \param[in] context
 * Log context informations
 * \param[in] msg
 * Log message to create
 */
void QLogHandler::messageHandler(QtMsgType idType, const QMessageLogContext &context, const QString &msg)
{
    QLogHandler &logHandler = QLogHandler::instance();
    logHandler.proceedMessage(idType, context, msg);
}

/*!
 * \brief Use to print log message to console/terminal
 * \details
 * Only \em QtDebugMsg and \c QtInfoMsg will be written
 * on \c stdout output, any other type will be written
 * to \c stderr.
 *
 * \param[in] idType
 * Type of log message
 * \param[in] fmtMsg
 * Formatted message to print.
 */
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

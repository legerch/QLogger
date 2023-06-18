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

bool QLogHandler::init(const QString &logFilename, int maxFiles, qint64 maxFileSize)
{
    /* Fill logger basic informations */
    QFileInfo infoPattern(logFilename);

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
    bool succeed = openFile(generateFilepath(0));
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

bool QLogHandler::isInitialized()
{
    return m_currentFile.isOpen();
}

QString QLogHandler::generateFilepath(int index)
{
    QString filename = m_fileBasename;

    /* Index different from base ? */
    if(index != 0){
        filename.append(QString::number(index));
    }

    /* Return complete filepath */
    return m_currentDir.absoluteFilePath(filename + m_fileExt);
}

bool QLogHandler::openFile(const QString &filepath)
{
    /* Assign file */
    m_currentFile.setFileName(filepath);

    /* Open it and prepare stream */
    bool succeed = m_currentFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
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

//TODO: doc
// Current implemntation is taken directly from Qt doc (https://doc.qt.io/qt-5/qtglobal.html#qInstallMessageHandler)
void QLogHandler::messageHandler(QtMsgType idType, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    const char *file = context.file ? context.file : "";
    const char *function = context.function ? context.function : "";
    switch (idType) {
    case QtDebugMsg:
        fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
        break;
    case QtInfoMsg:
        fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
        break;
    case QtWarningMsg:
        fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
        break;
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

#ifndef QLOGHANDLER
#define QLOGHANDLER

#include "qloggerglobal.h"
#include "qlogmsg.h"

#include <QDir>
#include <QMutex>
#include <QString>

namespace QLogger
{

class QLOGGER_EXPORT QLogHandler
{

public:
    bool init(const QString &logFilename, int maxFiles, qint64 maxFileSize, bool enableConsole);
    void desinit();

public:
    /* Prevent from making singleton copy (or move) */
    QLogHandler(const QLogHandler&) = delete;
    QLogHandler(QLogHandler&&) = delete;
    QLogHandler& operator=(const QLogHandler&) = delete;
    QLogHandler& operator=(QLogHandler&&) = delete;

public:
    static QLogHandler &instance();

private:
    bool isInitialized() const;

    QString generateFilepath(int index) const;
    bool openFile(const QString &filepath, bool truncate);
    void closeFile();

    bool sizeFileLimitIsReached(const QLogMsg &logMsg) const;

    bool rotateFiles();
    bool renameFile(const QString &oldName, const QString &newName);

    void proceedMessage(QtMsgType idType, const QMessageLogContext &context, const QString &msg);

private:
    static void messageHandler(QtMsgType idType, const QMessageLogContext &context, const QString &msg);
    static void messageToConsole(QtMsgType idType, const QLogMsg &fmtMsg);

private:
    explicit QLogHandler();
    ~QLogHandler();

private:
    bool m_enableConsole;

    qint64 m_maxFileSize;
    int m_maxFiles;
    QString m_fileBasename;
    QString m_fileExt;

    QDir m_currentDir;
    QFile m_currentFile;
    qsizetype m_currentFileSize;
    QTextStream m_stream;

    QMutex m_mutex;
};

} // Namespace QLogger

#endif // QLOGHANDLER

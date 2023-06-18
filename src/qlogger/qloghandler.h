#ifndef QLOGHANDLER
#define QLOGHANDLER

#include "qloggerglobal.h"

#include <QDir>
#include <QString>

namespace QLogger
{

class QLogHandler
{

public:
    bool init(const QString &logFilename, int maxFiles, qint64 maxFileSize);
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
    bool isInitialized();

    QString generateFilepath(int index);
    bool openFile(const QString &filepath);
    void closeFile();

private:
    static void messageHandler(QtMsgType idType, const QMessageLogContext &context, const QString &msg);

private:
    explicit QLogHandler();
    ~QLogHandler();

private:
    qint64 m_maxFileSize;
    int m_maxFiles;
    QString m_fileBasename;
    QString m_fileExt;

    QDir m_currentDir;
    QFile m_currentFile;

private:
    static QTextStream m_stream;
};

} // Namespace QLogger

#endif // QLOGHANDLER

#ifndef QLOGGER_FILELOGGER_H
#define QLOGGER_FILELOGGER_H

#include "base/baselogger.h"

#include <QDir>
#include <QTextStream>

namespace QLogger
{

class FileLogger : public BaseLogger
{
    Q_OBJECT

public:
    FileLogger(const QDir &dir, const QString &extension, bool enableConsole, QObject *parent = nullptr);

protected:
    bool openFile(const QString &basename, bool truncate);
    void closeFile();

protected:
    QString getFilePath(const QString &basename) const;
    qsizetype getFileSize() const;
    qsizetype getFileSizeNext(const LogBinary &log) const;

protected:
    virtual void write(const LogBinary &log) override;
    virtual void flush() override;

protected:
    static bool renameFile(const QString &oldName, const QString &newName);

private:
    QDir m_dir;
    QString m_extension;

    QFile m_file;
    qsizetype m_fileSize;
    QTextStream m_fileStream;
};

} // Namespace QLogger

#endif // QLOGGER_FILELOGGER_H

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
    FileLogger(const QFileInfo &filepath, bool enableConsole, QObject *parent = nullptr);

protected:
    bool openFile(const QVariant &argBasename, bool truncate);
    void closeFile();

protected:
    const QString& getBasename() const;
    qsizetype getFileSize() const;
    qsizetype getFileSizeNext(const LogBinary &log) const;

    QString generateFilePath(const QVariant &argBasename) const;

protected:
    virtual QString generateFmtBasename(const QVariant &arg) const = 0;

protected:
    virtual void write(const LogBinary &log) override;
    virtual void flush() override;

protected:
    static bool renameFile(const QString &oldName, const QString &newName);

private:
    QDir m_dir;
    QString m_basename;
    QString m_extension;

    QFile m_file;
    qsizetype m_fileSize;
    QTextStream m_fileStream;
};

} // Namespace QLogger

#endif // QLOGGER_FILELOGGER_H

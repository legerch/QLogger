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
    bool openFile(const QString &fmtBasename, bool truncate);
    void closeFile();

protected:
    const QDir& getDir() const;
    const QString& getBasename() const;
    const QString& getExtension() const;
    qsizetype getFileSize() const;
    qsizetype getFileSizeNext(const LogBinary &log) const;

    QString generateFilePathStr(const QString &fmtBasename) const;
    QString generateFilePathVar(const QVariant &argBasename) const;

    QStringList listFilesEntries(QDir::SortFlags sort) const;

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

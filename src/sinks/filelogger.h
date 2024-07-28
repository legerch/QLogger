#ifndef FILELOGGER_H
#define FILELOGGER_H

#include "base/baselogger.h"

#include <QDir>
#include <QTextStream>

namespace QLogger
{

class FileLogger : public BaseLogger
{

public:
    FileLogger(const QDir &dir, const QString &extension);

protected:
    bool openFile(const QString &basename, bool truncate);
    void closeFile();

protected:
    QString getFilePath(const QString &basename) const;
    qsizetype getFileSize() const;
    qsizetype getFileSizeNext(const QLogMsg &msg) const;

protected:
    virtual void proceedEntry(const QLogMsg &msg) override;
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

#endif // FILELOGGER_H

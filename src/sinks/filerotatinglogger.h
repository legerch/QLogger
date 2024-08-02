#ifndef FILEROTATINGLOGGER_H
#define FILEROTATINGLOGGER_H

#include "filelogger.h"

namespace QLogger
{

class FileRotatingLogger : public FileLogger
{

public:
    FileRotatingLogger(const QFileInfo &file, int maxFiles, qint64 maxSize, bool enableConsole);

protected:
    virtual void write(const LogBinary &log) override;

private:
    bool rotateFiles();

private:
    QString generateBasename(int index) const;
    QString generateFilepath(int index) const;

private:
    int m_maxFileNb;
    qint64 m_maxFileSize;

    QString m_fileBasename;
};

} // Namespace QLogger

#endif // FILEROTATINGLOGGER_H

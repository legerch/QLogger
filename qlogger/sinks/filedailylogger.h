#ifndef QLOGGER_FILEDAILYLOGGER_H
#define QLOGGER_FILEDAILYLOGGER_H

#include "filelogger.h"

#include <QTime>

namespace QLogger
{

class FileDailyLogger : public FileLogger
{
    Q_OBJECT

public:
    FileDailyLogger(const QFileInfo &file, const QTime &time, uint maxFiles, bool enableConsole, QObject *parent = nullptr);

protected:
    virtual QString generateFmtBasename(const QVariant &arg) const override;

protected:
    virtual void start() override;
    virtual void write(const LogBinary &log) override;

private:
    void rotationProgram();
    void rotationPerform();

    void cleanMaxLogs();

private:
    QTime m_timeDaily;
    QDateTime m_nextRotation;
    uint m_nbMaxFiles;
};

} // Namespace QLogger

#endif // FILEDAILYLOGGER_H

#ifndef QLOGGER_LOGENTRY_H
#define QLOGGER_LOGENTRY_H

#include <QFileInfo>
#include <QMessageLogContext>

namespace QLogger
{

class LogEntry
{

public:
    explicit LogEntry(QtMsgType idType, const QMessageLogContext &context, const QString &msg);

public:
    QtMsgType getType() const;
    QString getTypeString() const;

    bool contextIsAvailable() const;
    const QFileInfo& getCtxFile() const;
    int getCtxLine() const;
    const QString& getCtxFctSig() const;
    QString getCtxFctName() const;

    const QString& getMsg() const;

private:
    QtMsgType m_idType;
    QString m_msg;

    bool m_haveContext;
    QFileInfo m_ctxFile;
    int m_ctxLine;
    QString m_ctxFctSig;
};

} // Namespace QLogger

#endif // QLOGGER_LOGENTRY_H

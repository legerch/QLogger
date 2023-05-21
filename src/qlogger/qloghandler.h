#ifndef QLOGHANDLER
#define QLOGHANDLER

#include "qloggerglobal.h"

#include <QString>

namespace QLogger
{

class QLogHandler
{

public:
    static QLogHandler &instance();

public:
    void messageHandler(QtMsgType idType, const QMessageLogContext &context, const QString &msg);

public:
    /* Prevent from making singleton copy (or move) */
    QLogHandler(const QLogHandler&) = delete;
    QLogHandler(QLogHandler&&) = delete;
    QLogHandler& operator=(const QLogHandler&) = delete;
    QLogHandler& operator=(QLogHandler&&) = delete;

private:
    explicit QLogHandler();
    ~QLogHandler();
};

} // Namespace QLogger

#endif // QLOGHANDLER

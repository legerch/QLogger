#include "qlogger/qloggerfactory.h"

/*****************************/
/* Custom log formatter      */
/*****************************/
QString logFormatter(const QLogger::LogEntry &log)
{
    QString fmt = QString("[%1][%2] %3").arg(
        QDateTime::currentDateTimeUtc().toString(Qt::ISODateWithMs),
        log.getTypeString(),
        log.getMsg()
    );

    /* Can we add context informations ? */
    if(log.contextIsAvailable()){
        fmt += QString(" (%1:%2, %3)")
                   .arg(log.getCtxFile().fileName())
                   .arg(log.getCtxLine())
                   .arg(log.getCtxFctName());
    }

    /* Terminate log line */
    fmt += '\n';

    return fmt;
}

/*****************************/
/* Main method               */
/*****************************/
int main(int argc, char *argv[])
{
    /* Set logs */
    QLogger::LogFormatter::setCustomFormat(logFormatter); // Set it before choosing sink
    QLogger::QLoggerFactory::instance().initChoosenSink();

    /* Manage application properties */
    [...]

    return app.exec();
}
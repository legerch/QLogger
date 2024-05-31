#include "qloggerfactory.h"

#include "sinks/filerotatinglogger.h"

/*****************************/
/* Class documentations      */
/*****************************/

/*****************************/
/* Enum documentations       */
/*****************************/

/*****************************/
/* Structure documentations  */
/*****************************/

/*****************************/
/* Signals documentations    */
/*****************************/

/*****************************/
/* Macro definitions         */
/*****************************/

/*****************************/
/* Start namespace           */
/*****************************/

namespace QLogger{

QLoggerFactory::QLoggerFactory()
{
    /* Nothing to do */
}

QLoggerFactory::~QLoggerFactory()
{
    desinit();
}

void QLoggerFactory::initLoggerRotating(const QFileInfo &file, int maxFiles, qint64 maxSize)
{
    m_logger = std::make_unique<FileRotatingLogger>(file, maxFiles, maxSize);
}

void QLoggerFactory::desinit()
{
    //TODO: deregister handler
}

/*****************************/
/* Constants definitions     */
/*****************************/

/*****************************/
/* Functions implementation  */
/*         Class             */
/*****************************/



/*****************************/
/* Functions implementation  */
/*        Structures         */
/*****************************/

/*****************************/
/* End namespace             */
/*****************************/

} // namespace QLogger

/*****************************/
/* End file                  */
/*****************************/

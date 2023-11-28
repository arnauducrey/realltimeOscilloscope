#include <config/xf-config.h>

#if (USE_XF_CLASS_DEFAULT_QT_IMPLEMENTATION != 0)

#include <QCoreApplication>
#include "xf/interface/timeoutmanager.h"
#include "xf/interface/resourcefactory.h"
#include "xf/interface/dispatcher.h"
#include "xf/xf.h"

using interface::XFResourceFactory;

/**
 * In the Qt port we are going to use a QCoreApplication instance
 * which gets executed by the XF::exec() method.
 *
 * The default dispatcher is active in this port implementation.
 * This means, the default dispatcher has its own thread.
 *
 * The XF::execOnce() method is not applicable for this port.
 */

bool XF::isInitialized_ = false;
bool XF::isRunning_ = false;

static QCoreApplication & getApplication(int argc = 0, char * argv[] = nullptr)
{
    static QCoreApplication app(argc, argv);
    return app;
}

void XF::initialize(int timeInterval /* = 10 */, int argc /* = 0 */, char * argv[] /* = nullptr */)
{
    if (!isInitialized_)
    {
        // Call getApplication() to create QT application instance
        ::getApplication(argc, argv);

        // Create and initialize TimeoutManager
        interface::XFTimeoutManager::getInstance()->initialize(timeInterval);
        // Start it
        interface::XFTimeoutManager::getInstance()->start();

        isInitialized_ = true;
    }
}

int XF::exec()
{
    // Start default dispatcher
    XFResourceFactory::getInstance()->getDefaultDispatcher()->start();

    isRunning_ = true;

    // Start Qt event loop
    return ::getApplication().exec();
}

int XF::execOnce()
{
    Q_ASSERT(false);    // Not applicable for this port
    return 0;
}

bool XF::isRunning()
{
    return isRunning_;
}

#endif // USE_XF_CLASS_DEFAULT_QT_IMPLEMENTATION

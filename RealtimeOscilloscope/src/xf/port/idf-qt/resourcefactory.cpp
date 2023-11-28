#include <config/xf-config.h>

#if (USE_XF_PORT_IDF_QT_RESOURCE_FACTORY_IMPLEMENTATION != 0)

#include <QtGlobal>
#include "thread-default.h"
#include "default/dispatcher-active.h"
#include "xf/interface/mutex.h"
#include "resourcefactory.h"

using interface::XFMutex;

//static
interface::XFResourceFactory * interface::XFResourceFactory::getInstance()
{
    return XFResourceFactoryPort::getInstance();
}

// static
interface::XFResourceFactory * XFResourceFactoryPort::getInstance()
{
    static XFResourceFactoryPort theResourceFactory;
    return &theResourceFactory;
}

interface::XFDispatcher * XFResourceFactoryPort::getDefaultDispatcher()
{
    static XFDispatcherActiveDefault mainDispatcher;
    return &mainDispatcher;
}

interface::XFDispatcher * XFResourceFactoryPort::createDispatcher()
{
    // With an IDF implementation only one dispatcher is allowed. This
    // prohibits to create new dispatchers. Return the default dispatcher.
    return getDefaultDispatcher();
}

interface::XFThread * XFResourceFactoryPort::createThread(interface::XFThreadEntryPointProvider * pProvider,
                                                          interface::XFThread::EntryMethodBody entryMethod,
                                                          const char * threadName,
                                                          const uint32_t stackSize /* = 0 */)
{
	// With an IDF implementation normaly only one thread is created and it handles
	// all the XF related stuff. With the Qt based IDF, there is a dedicated thread needed
	// which executes the XF. The primary thread (executing the main function) is
	// needed to execute the QApplication instance (for timers, signals and slots).
	// 
	// The method createThread() is only called once at XF initialization to provide
	// the thread executing the XF.
    return new XFThreadDefault(pProvider, entryMethod, threadName, stackSize);
}

interface::XFMutex * XFResourceFactoryPort::createMutex()
{
    return XFMutex::create();
}

#endif // USE_XF_PORT_IDF_QT_RESOURCE_FACTORY_IMPLEMENTATION

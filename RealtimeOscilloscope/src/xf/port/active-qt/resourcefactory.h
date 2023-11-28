#ifndef XF_PORT_ACTIVE_QT_RESOURCE_FACTORY_H
#define XF_PORT_ACTIVE_QT_RESOURCE_FACTORY_H

#include "config/xf-config.h"

#if (USE_XF_PORT_ACTIVE_QT_RESOURCE_FACTORY_IMPLEMENTATION != 0)

#include "xf/interface/resourcefactory.h"
#include "xf/interface/dispatcher.h"

class XFResourceFactoryPort : public interface::XFResourceFactory
{
public:
    ~XFResourceFactoryPort() override {}

    static interface::XFResourceFactory * getInstance();
	
    interface::XFDispatcher * getDefaultDispatcher() override;
    interface::XFDispatcher * createDispatcher() override;
    interface::XFThread * createThread(interface::XFThreadEntryPointProvider * pProvider,
                                             interface::XFThread::EntryMethodBody entryMethod,
                                             const char * threadName,
                                             const uint32_t stackSize = 0) override;
    interface::XFMutex * createMutex() override;

protected:
    XFResourceFactoryPort() {}
};

#endif // USE_XF_PORT_ACTIVE_QT_RESOURCE_FACTORY_IMPLEMENTATION
#endif // XF_PORT_ACTIVE_QT_RESOURCE_FACTORY_H

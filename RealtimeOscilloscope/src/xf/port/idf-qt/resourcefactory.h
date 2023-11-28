#ifndef XF_PORT_IDF_QT_RESOURCE_FACTORY_H
#define XF_PORT_IDF_QT_RESOURCE_FACTORY_H

#include "config/xf-config.h"

#if (USE_XF_PORT_IDF_QT_RESOURCE_FACTORY_IMPLEMENTATION != 0)

/** @ingroup port_idf_qt
 *  @{
 */

#include "xf/interface/resourcefactory.h"
#include "xf/interface/dispatcher.h"

/**
 * @brief XFResourceFactoryPort implementation for the IDF Qt port.
 */
class XFResourceFactoryPort : public interface::XFResourceFactory
{
public:
    ~XFResourceFactoryPort() override {}

    static interface::XFResourceFactory * getInstance();            ///< Returns a pointer to the unique instance of the XF resource factory.

    interface::XFDispatcher * getDefaultDispatcher() override;                                      ///< Returns the default dispatcher.
    interface::XFDispatcher * createDispatcher() override;                                          ///< Creates and returns a new dispatcher.
    interface::XFThread * createThread(interface::XFThreadEntryPointProvider * pProvider,
                                             interface::XFThread::EntryMethodBody entryMethod,
                                             const char * threadName,
                                             const uint32_t stackSize = 0) override;                 ///< Creates and returns a new thread.
    interface::XFMutex * createMutex() override;                                                     ///< Creates and returns a new mutex.

protected:
    XFResourceFactoryPort() {}
};

/** @} */ // end of port_idf_qt group
#endif // USE_XF_PORT_IDF_QT_RESOURCE_FACTORY_IMPLEMENTATION
#endif // XF_PORT_IDF_QT_RESOURCE_FACTORY_H

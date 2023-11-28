#ifndef XF_DISPATCHER_ACTIVE_H
#define XF_DISPATCHER_ACTIVE_H

#include <config/xf-config.h>

#if (USE_XF_DISPATCHER_ACTIVE_DEFAULT_IMPLEMENTATION != 0)

#include "xf/interface/dispatcher.h"
#include "xf/interface/thread.h"
#include "xf/interface/mutex.h"

/*
 * Please specify/include the XFEventQueuePort class in the xf-config file!
 */

/** @ingroup port_default
 *  @{
 */

/**
 * @brief A dispatcher implementation which may be executed using an internal thread.
 *
 * The active dispatcher internally requests a Thread from the XFResourceFactory and
 * starts it with the start() method. The thread itself is responsible to execute the 
 * protected method execute().
 */
class XFDispatcherActiveDefault : public interface::XFThreadEntryPointProvider,
                                  public interface::XFDispatcher
{
public:
    XFDispatcherActiveDefault();
    ~XFDispatcherActiveDefault() override;

    bool isActive() const  override { return (pThread_ != nullptr) ? true : false; }

    void start() override;
    void stop() override;
    void pushEvent(XFEvent * pEvent, bool fromISR=false) override;

    void scheduleTimeout(int timeoutId, int interval, interface::XFReactive * pReactive) override;
    void unscheduleTimeout(int timeoutId, interface::XFReactive * pReactive) override;

    int executeOnce() override;

protected:
    int execute(const void * param = nullptr) override;
    void dispatchEvent(const XFEvent * pEvent) const override;

protected:
    bool isExecuting_;                   ///< True as long as the thread is executing the main loop.
    interface::XFThread * pThread_;     ///< Pointer to Thread executing the dispatcher.
    XFEventQueue events_;        		///< Thread-safe queue holding events waiting to get dispatched.
};

/** @} */ // end of port_default group
#endif // USE_XF_DISPATCHER_ACTIVE_DEFAULT_IMPLEMENTATION
#endif // XF_DISPATCHER_ACTIVE_H

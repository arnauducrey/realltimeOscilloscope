#include <config/xf-config.h>

#if (USE_XF_DISPATCHER_ACTIVE_DEFAULT_IMPLEMENTATION != 0)

#include <cassert>
#if defined(XF_TRACE_EVENT_PUSH_POP) && (XF_TRACE_EVENT_PUSH_POP != 0)
    #include "trace/trace.h"
#endif // XF_TRACE_EVENT_PUSH_POP
#include "xf/eventstatus.h"
#include "xf/interface/timeoutmanager.h"
#include "xf/interface/reactive.h"
#include "xf/interface/thread.h"
#include "xf/interface/resourcefactory.h"
#include "dispatcher-active.h"

using interface::XFTimeoutManager;
using interface::XFResourceFactory;
using interface::XFReactive;

XFDispatcherActiveDefault::XFDispatcherActiveDefault() :
    isExecuting_(false),
    pThread_(nullptr)
{
    // Create Thread
    pThread_ = XFResourceFactory::getInstance()->createThread(this,
                                                              interface::XFThread::EntryMethodBody(&XFDispatcherActiveDefault::execute),
                                                              "dispatcherThread");
    assert(pThread_);
}

XFDispatcherActiveDefault::~XFDispatcherActiveDefault()
{
    isExecuting_ = false;
    pThread_->stop();

    if (pThread_)
    {
        delete pThread_;
    }
}

void XFDispatcherActiveDefault::start()
{
    assert(pThread_);
    isExecuting_ = true;
    pThread_->start();
}

void XFDispatcherActiveDefault::stop()
{
    isExecuting_ = false;
    pThread_->stop();
}

void XFDispatcherActiveDefault::pushEvent(XFEvent * pEvent, bool fromISR)
{
#if defined(XF_TRACE_EVENT_PUSH_POP) && (XF_TRACE_EVENT_PUSH_POP != 0)
        Trace::out("Push event: 0x%x", pEvent);
#endif // XF_TRACE_EVENT_PUSH_POP
        events_.push(pEvent, fromISR);
}

void XFDispatcherActiveDefault::scheduleTimeout(int timeoutId, int interval, interface::XFReactive * pReactive)
{
    // Forward timeout to the timeout manager
    XFTimeoutManager::getInstance()->scheduleTimeout(timeoutId, interval, pReactive);
}

void XFDispatcherActiveDefault::unscheduleTimeout(int timeoutId, interface::XFReactive * pReactive)
{
    // Forward timeout to the timeout manager
    XFTimeoutManager::getInstance()->unscheduleTimeout(timeoutId, pReactive);
}

int XFDispatcherActiveDefault::execute(const void * param /* = nullptr */)
{
    (void)param;

    while(isExecuting_)
    {
        while (events_.empty() && isExecuting_)
        {
            events_.pend();	// Wait until something to do
        }

        executeOnce();
    }

    return 0;
}

int XFDispatcherActiveDefault::executeOnce()
{
    if (!events_.empty())
    {
        const XFEvent * pEvent;

        // Deque next event from queue
        pEvent = events_.front(); events_.pop();
#if defined(XF_TRACE_EVENT_PUSH_POP) && (XF_TRACE_EVENT_PUSH_POP != 0)
            Trace::out("Pop event:  0x%x", pEvent);
#endif // XF_TRACE_EVENT_PUSH_POP

        if (pEvent)
        {
            // Forward the event to the behavioral class
            dispatchEvent(pEvent);

            if (pEvent->getEventType() == XFEvent::Terminate)
            {
                // Exit the event loop
                isExecuting_ = false;
            }

            if (pEvent->deleteAfterConsume())
            {
                // Remove the consumed event
                delete pEvent;
            }
        }
    }

    return isExecuting_;
}

void XFDispatcherActiveDefault::dispatchEvent(const XFEvent * pEvent) const
{
    XFReactive::TerminateBehavior terminateBehavior;

    terminateBehavior = pEvent->getBehavior()->process(pEvent);

    // Check if behavior should be deleted
    if (terminateBehavior and pEvent->getBehavior()->deleteOnTerminate())
    {
        delete pEvent->getBehavior();
    }
}

#endif // USE_XF_DISPATCHER_ACTIVE_DEFAULT_IMPLEMENTATION

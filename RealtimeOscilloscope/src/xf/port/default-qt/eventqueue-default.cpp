#include <config/xf-config.h>

#if (USE_XF_EVENT_QUEUE_DEFAULT_QT_IMPLEMENTATION != 0)

#include <cassert>
#include <QtGlobal>
#include <QMutexLocker>
#include "eventqueue-default.h"

XFEventQueueDefault::XFEventQueueDefault() :
    mutex_(QMutex::NonRecursive)
{
}

XFEventQueueDefault::~XFEventQueueDefault()
{
    newEvents_.wakeAll();
}

bool XFEventQueueDefault::empty() const
{
    return queue_.isEmpty();
}

bool XFEventQueueDefault::push(const XFEvent * pEvent)
{
    QMutexLocker locker(&mutex_);
    queue_.enqueue(pEvent);
    // Tell waiting thread(s) there is again an event present
    newEvents_.wakeAll();
    return true;
}

const XFEvent * XFEventQueueDefault::front()
{
    return queue_.front();
}

void XFEventQueueDefault::pop()
{
    QMutexLocker locker(&mutex_);
    queue_.dequeue();
}

bool XFEventQueueDefault::pend()
{
    QMutexLocker locker(&mutex_);
    // Wait for new events. Mutex needs to be in lock-state
    // prior to call wait()!
    newEvents_.wait(&mutex_);

    return !queue_.isEmpty();
}

#endif // USE_XF_EVENT_QUEUE_DEFAULT_QT_IMPLEMENTATION

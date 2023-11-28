
#include <config/xf-config.h>

#if (USE_XF_TIMEOUTMANAGER_DEFAULT_IMPLEMENTATION != 0)

#include <cassert>
#include "xf/interface/reactive.h"
#include "xf/interface/mutex.h"
#include "port-functions.h"
#include "timeoutmanager-default.h"

using interface::XFMutex;

interface::XFTimeoutManager * interface::XFTimeoutManager::getInstance()
{
    return XFTimeoutManagerDefault::getInstance();
}

interface::XFTimeoutManager * XFTimeoutManagerDefault::getInstance()
{
    static XFTimeoutManagerDefault timeoutManager;
    return &timeoutManager;
}

XFTimeoutManagerDefault::XFTimeoutManagerDefault() :
    pMutex_(nullptr)
{
    pMutex_ = XFMutex::create();
    assert(pMutex_);
}

XFTimeoutManagerDefault::~XFTimeoutManagerDefault()
{
}

void XFTimeoutManagerDefault::start()
{
    // Check tickInterval. Set default value if not set
    if (tickInterval_ == 0)
    {
        tickInterval_ = 10;
    }

    XF_startTimeoutManagerTimer(uint32_t(tickInterval_));
}

void XFTimeoutManagerDefault::scheduleTimeout(int32_t timeoutId, int32_t interval, interface::XFReactive * pReactive)
{
    XFTimeout * pTimeout = new XFTimeout(timeoutId, interval, pReactive);

    if (pTimeout)
    {
        addTimeout(pTimeout);
    }
}

void XFTimeoutManagerDefault::unscheduleTimeout(int32_t timeoutId, interface::XFReactive * pReactive)
{
    const XFTimeout timeout(timeoutId, 0, pReactive);
    XFTimeout * pTimeout;

    pMutex_->lock();
    {
        for (TimeoutList::iterator i = timeouts_.begin();
             i != timeouts_.end(); /*Do not increment here!*/)
        {
            pTimeout = *i;

            // Check if behavior and timeout id are equal
            if (*pTimeout == timeout)
            {
                TimeoutList::iterator next = i;

                // Check if remaining ticks can be given further
                if (++next != timeouts_.end())
                {
                    // Add (remaining) ticks to next timeout in list
                    (*next)->addToRelTicks(pTimeout->getRelTicks());
                }

                i = timeouts_.erase(i);
                // Iterator now points to the next element

                delete pTimeout;
            }
            else
            {
                i++;
            }
        }
    }
    pMutex_->unlock();
}

void XFTimeoutManagerDefault::tick()
{
    assert(tickInterval_);      // Did you call start()?!
    int32_t intervalToSubtract = tickInterval_;

    while (!timeouts_.empty())
    {
        pMutex_->lock();
        {
            XFTimeout * pFirstTimeout = timeouts_.front();

            // Subtract time elapsed
            pFirstTimeout->substractFromRelTicks(intervalToSubtract);

            // From now on set it to zero.
            intervalToSubtract = 0;

            // Check timeout timed out
            if (pFirstTimeout->getRelTicks() <= 0)
            {
                // Check remaining ticks can be given further
                if (timeouts_.size() > 1)
                {
                    TimeoutList::iterator i = timeouts_.begin();

                    // Add ticks overrun to next timeout
                    i++;
                    (*i)->substractFromRelTicks(abs(pFirstTimeout->getRelTicks()));
                }

                // Inject the timeout back to the behavioral class
                returnTimeout(pFirstTimeout);

                // Remove timeout
                timeouts_.pop_front();

                // Check if timeouts with same timeout value are present
                for (TimeoutList::iterator it = timeouts_.begin(); it != timeouts_.end(); /*Do not increment here!*/)
                {
                    if ((*it)->getRelTicks() == 0)
                    {
                        returnTimeout(*it);			// Return them true
                        it = timeouts_.erase(it);	// Remove timeout and adjust iterator to next element
                    }
                    else
                    {
                        break;
                    }
                }
            }
            else
            {
                pMutex_->unlock();
                // Done. Exit while loop
                break;
            }
        }
        pMutex_->unlock();
    }
}

void XFTimeoutManagerDefault::addTimeout(XFTimeout * pNewTimeout)
{
    if (!timeouts_.empty())
    {
        pMutex_->lock();
        {
            // Insert timeout before timeout(s) triggering later
            TimeoutList::iterator i = timeouts_.begin();

            unsigned int index = 0;

            // Find the right place to insert new timeout
            while (i != timeouts_.end() &&
                   (*i)->getRelTicks() < pNewTimeout->getRelTicks())
            {	// -> by: test4, test5
                pNewTimeout->substractFromRelTicks((*i)->getRelTicks());
                i++; index++;
            }

            if (i != timeouts_.end())
            {
                if ((*i)->getRelTicks() != pNewTimeout->getRelTicks())
                {	// -> by: test1, test4, test5
                    // Timeout are going to timeout at different times

                    // Insert new timeout before
                    timeouts_.insert(i, pNewTimeout);

                    // Remove time from following timeout
                    (*i)->substractFromRelTicks(pNewTimeout->getRelTicks());

                }
                else
                {	// -> by: test1, test5
                    const int32_t relTicks = (*i)->getRelTicks();
                    // Timeouts timeout at the same time. Put
                    // the new one behind the actual.
                    i++;

                    // Check if even more timeouts with the same timeout
                    while(i != timeouts_.end() && (*i)->getRelTicks() == 0)
                    {	// -> by: test5
                        i++;
                    }

                    // Insert new timeout behind actual
                    timeouts_.insert(i, pNewTimeout);
                    // Remove time from actual timeout
                    pNewTimeout->substractFromRelTicks(relTicks);
                }
            }
            else
            {	// -> by: test4, test5
                // Add timeout at the end of the list
                timeouts_.insert(timeouts_.end(), pNewTimeout);
            }
        }
        pMutex_->unlock();
    }
    else
    {	// -> by: test1, test2, test3, test4, test5
        timeouts_.push_front(pNewTimeout);
    }
}

void XFTimeoutManagerDefault::returnTimeout(XFTimeout * pTimeout)
{
    pTimeout->getBehavior()->pushEvent(pTimeout);
}

#endif // USE_XF_TIMEOUTMANAGER_DEFAULT_IMPLEMENTATION

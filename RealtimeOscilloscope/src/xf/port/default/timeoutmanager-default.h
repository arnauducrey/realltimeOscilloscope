#ifndef XF_TIMEOUTMANAGER_DEFAULT_H
#define XF_TIMEOUTMANAGER_DEFAULT_H

#include <list>
#include <config/xf-config.h>

#if (USE_XF_TIMEOUTMANAGER_DEFAULT_IMPLEMENTATION != 0)

#include "xf/interface/timeoutmanager.h"
#include "xf/timeout.h"
#include "xf/interface/mutex.h"

/** @ingroup port_default
 *  @{
 */

/**
 * @brief Default implementation of the XF TimeoutManager
 */
class XFTimeoutManagerDefault : public interface::XFTimeoutManager
{
    friend interface::XFTimeoutManager * interface::XFTimeoutManager::getInstance();
public:
    ~XFTimeoutManagerDefault() override;
	
    static interface::XFTimeoutManager * getInstance();     ///< Returns a pointer to the single instance of the class.

    void start() override;                                                                                   ///< See interface::XFTimeoutManager
    void scheduleTimeout(int32_t timeoutId, int32_t interval, interface::XFReactive * pReactive) override;   ///< See interface::XFTimeoutManager
    void unscheduleTimeout(int32_t timeoutId, interface::XFReactive * pReactive) override;                   ///< See interface::XFTimeoutManager
    void tick() override;                                                                                    ///< See interface::XFTimeoutManager

protected:
	XFTimeoutManagerDefault();
    void addTimeout(XFTimeout * pNewTimeout) override;      ///< Adds the timeout to #_timeouts.
	
	/**
	 * Returns the timeout back to the queue of the thread executing
	 * the behavioral instance.
	 */
	void returnTimeout(XFTimeout * pTimeout);	///< Returns timeout back to behavioral class.

protected:
	typedef std::list<XFTimeout *> TimeoutList;		///< Type used for the _timeouts property.
	
    TimeoutList timeouts_;							///< Container holding timeouts to manage.

    interface::XFMutex * pMutex_;					///< Mutex to protect access to TimeoutList.
};

/** @} */ // end of port_default group
#endif // USE_XF_TIMEOUTMANAGER_DEFAULT_IMPLEMENTATION
#endif // XF_TIMEOUTMANAGER_DEFAULT_H

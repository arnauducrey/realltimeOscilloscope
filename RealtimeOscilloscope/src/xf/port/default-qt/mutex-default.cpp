#include <cassert>
#include <config/xf-config.h>

#if (USE_XF_MUTEX_DEFAULT_QT_IMPLEMENTATION != 0)

#include "mutex-default.h"

/**
 * @brief Implementation of interface::XFMutex::create method.
 */
interface::XFMutex * interface::XFMutex::create()
{
    return new XFMutexDefault;
}

void XFMutexDefault::lock()
{
    mutex_.lock();
}

void XFMutexDefault::unlock()
{
    mutex_.unlock();
}

bool XFMutexDefault::tryLock(int32_t timeout /* = 0 */)
{
    return mutex_.tryLock(timeout);
}

#endif // USE_XF_MUTEX_DEFAULT_QT_IMPLEMENTATION

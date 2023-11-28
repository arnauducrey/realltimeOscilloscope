#ifndef XF_MUTEX_DEFAULT_QT_H
#define XF_MUTEX_DEFAULT_QT_H

#include <config/xf-config.h>

#if (USE_XF_MUTEX_DEFAULT_QT_IMPLEMENTATION != 0)

#include <stdint.h>
#include <QMutex>
#include "xf/interface/mutex.h"

/** @ingroup port_default_qt
 *  @{
 */

/**
 * @brief Default Qt implementation for the XFMutex interface.
 */
class XFMutexDefault : public interface::XFMutex
{
    friend class XFResourceFactoryPort;
    friend class interface::XFMutex;
public:

    void lock() override;
    void unlock() override;

    bool tryLock(int32_t timeout = 0) override;

protected:
    XFMutexDefault() = default;     ///< Do not allow to directly create an object of this class. Call XFMutex::create() instead.

protected:
    QMutex mutex_;                  ///< The real mutex.
};

/** @} */ // end of port_default_qt group
#endif // USE_XF_MUTEX_DEFAULT_QT_IMPLEMENTATION
#endif // XF_MUTEX_DEFAULT_QT_H

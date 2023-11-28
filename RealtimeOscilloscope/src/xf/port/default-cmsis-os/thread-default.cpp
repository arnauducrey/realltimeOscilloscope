#include <config/xf-config.h>

#if (USE_XF_THREAD_DEFAULT_CMSIS_OS_IMPLEMENTATION != 0)

#include <cassert>
#include <string.h>
#include "thread-default.h"

#if !defined(XFTHREAD_DEFAULT_STACK_SIZE)
#	define XFTHREAD_DEFAULT_STACK_SIZE	512
#endif

//extern "C" const int  uxTopUsedPriority;
//const int __attribute__((used)) uxTopUsedPriority = configMAX_PRIORITIES - 1;

/**
 * Always keep threads after creation in suspended state.
 * Threads must be explicitly started with start()!
 */


/// Attributes structure for thread.
/*
typedef struct {
  const char                   *name;   ///< name of the thread
  uint32_t                 attr_bits;   ///< attribute bits
  void                      *cb_mem;    ///< memory for control block
  uint32_t                   cb_size;   ///< size of provided memory for control block
  void                   *stack_mem;    ///< memory for stack
  uint32_t                stack_size;   ///< size of stack
  osPriority_t              priority;   ///< initial thread priority (default: osPriorityNormal)
  TZ_ModuleId_t            tz_module;   ///< TrustZone module identifier
  uint32_t                  reserved;   ///< reserved (must be 0)
} osThreadAttr_t;*/

XFThreadDefault::XFThreadDefault(interface::XFThreadEntryPointProvider * pProvider,
                                 interface::XFThread::EntryMethodBody entryMethod,
                                 const char * threadName,
                                 const uint32_t stackSize /* = 0 */)
{
	uint32_t threadStackSize = stackSize;

	_pEntryMethodProvider = pProvider;
	_entryMethod = entryMethod;

	strcpy(_threadName, threadName);

	if (threadStackSize == 0)
	{
		threadStackSize = XFTHREAD_DEFAULT_STACK_SIZE;
	}

	::memset(&_threadAttr, 0, sizeof(_threadAttr));

	_threadAttr.name = _threadName;
	_threadAttr.priority = osPriorityNormal;
	_threadAttr.stack_size = threadStackSize;

	_threadFunc = &threadEntryPoint;

	_threadId = osThreadNew(_threadFunc, this, &_threadAttr);
	assert(_threadId);		// Check if thread creation was successful

	// Always keep threads after creation in suspended state.
	// Must be explicitly started with start();
	osThreadSuspend(_threadId);
}

//static
void XFThreadDefault::threadEntryPoint(void * param)
{
	XFThreadDefault * pThis = (XFThreadDefault *)param;

	(pThis->_pEntryMethodProvider->*pThis->_entryMethod)(param);
}

void XFThreadDefault::start()
{
	assert(_threadId != 0);		// Check if thread was created
	osThreadResume(_threadId);
}

void XFThreadDefault::stop()
{
	osThreadTerminate(_threadId);
}

void XFThreadDefault::setPriority(XFThreadPriority priority)
{
    osPriority prio = osPriorityNormal;
    switch (priority)
    {
    case XF_THREAD_PRIO_LOW:
        prio = osPriorityLow;
        break;
    case XF_THREAD_PRIO_HIGH:
        prio = osPriorityHigh;
        break;
    default:
        break;
    }

    osThreadSetPriority(_threadId, prio);
}

XFThreadPriority XFThreadDefault::getPriority() const
{
    const osPriority prio = osThreadGetPriority(_threadId);
    XFThreadPriority priority = XF_THREAD_PRIO_UNKNOWN;

    switch (prio)
    {
    case osPriorityLow:
        priority = XF_THREAD_PRIO_LOW;
        break;
    case osPriorityNormal:
        priority = XF_THREAD_PRIO_NORMAL;
        break;
    case osPriorityHigh:
        priority = XF_THREAD_PRIO_HIGH;
        break;
    default:
        break;
    }

    return priority;
}

void XFThreadDefault::delay(uint32_t milliseconds)
{
	osDelay(milliseconds);
}

#endif // USE_XF_THREAD_DEFAULT_CMSIS_OS_IMPLEMENTATION

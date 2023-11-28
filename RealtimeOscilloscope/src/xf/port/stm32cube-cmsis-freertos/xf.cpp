#include <config/xf-config.h>

#if (PORT_STM32CUBE_CMSIS_FREERTOS != 0)

#include <cassert>
#include <cmsis_os.h>

#include "xf/interface/timeoutmanager.h"
#include "xf/interface/resourcefactory.h"
#include "xf/interface/dispatcher.h"
#include "xf/xf.h"

using interface::XFTimeoutManager;
using interface::XFResourceFactory;

bool XF::isInitialized_ = false;

void XF_initialize(int timeInterval)
{
    XF::initialize(timeInterval);
}

void XF_exec()
{
    XF::exec();
}

void XF_execOnce()
{
    XF::execOnce();
}

void XF::initialize(int timeInterval /* = 10 */, int argc /* = 0 */, char * argv[] /* = nullptr */)
{
    if (!isInitialized_)
    {
		//osKernelInitialize();

		// Create and initialize TimeoutManager
        XFTimeoutManager::getInstance()->initialize(timeInterval);

        isInitialized_ = true;
    }
}

int XF::exec()
{
    // Start timeout manager
    XFTimeoutManager::getInstance()->start();

    // Start default dispatcher
	XFResourceFactory::getInstance()->getDefaultDispatcher()->start();

	// Start RTOS if it is not already running.
	// With STM32CubeMX, FreeRTOS and uGFX
	// OS may already run at this stage!

	if (osKernelGetState()!=osKernelRunning)
	{
	    osKernelStart();
	}


	return 0;
}

int XF::execOnce()
{
    assert(false);  // Should not be called within the FreeRTOS based port! Call XF_exec() instead!
}

#endif // PORT_STM32CUBE_CMSIS_FREERTOS

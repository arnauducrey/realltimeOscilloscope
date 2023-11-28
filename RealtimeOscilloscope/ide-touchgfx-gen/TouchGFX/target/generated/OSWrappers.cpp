/**
  ******************************************************************************
  * File Name          : OSWrappers.cpp
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
#include "touchgfx-config.h"
#if ( TOUCHGFX_BAREMETAL != 0)
#include <stm32f7xx_hal.h>
#include <TouchGFXHAL.hpp>
#endif // TOUCHGFX_BAREMETAL
#if ( TOUCHGFX_FREERTOS != 0)
#include <cassert>
#include <cmsis_os2.h>
#include <touchgfx/hal/HAL.hpp>
#endif // TOUCHGFX_FREERTOS
#include <touchgfx/hal/OSWrappers.hpp>


#if (TOUCHGFX_BAREMETAL != 0)
	static volatile uint32_t fb_sem;
	static volatile uint32_t vsync_sem;
#endif  // TOUCHGFX_BAREMETAL

#if (TOUCHGFX_FREERTOS != 0)
	#include <cmsis_os2.h>
	static osSemaphoreId_t frame_buffer_sem = NULL;
	static osMessageQueueId_t vsync_queue = NULL;
	// Just a dummy value to insert in the VSYNC queue.
	static uint32_t dummy = 0x5a;
#endif //TOUCHGFX_FREERTOS

using namespace touchgfx;

/*
 * Initialize frame buffer semaphore and queue/mutex for VSYNC signal.
 */
void OSWrappers::initialize()
{
	#if (TOUCHGFX_BAREMETAL != 0)
		fb_sem = 0;
		vsync_sem = 0;
	#endif  // TOUCHGFX_BAREMETAL
	#if (TOUCHGFX_FREERTOS != 0)
		// Create a queue of length 1
		frame_buffer_sem = osSemaphoreNew(1, 1, NULL); // Binary semaphore
		assert((frame_buffer_sem != NULL) && "Creation of framebuffer semaphore failed");

		// Create a queue of length 1
		vsync_queue = osMessageQueueNew(1, 4, NULL);
		assert((vsync_queue != NULL) && "Creation of vsync message queue failed");
	#endif // TOUCHGFX_FREERTOS
}

/*
 * Take the frame buffer semaphore. Blocks until semaphore is available.
 */
void OSWrappers::takeFrameBufferSemaphore()
{
	#if (TOUCHGFX_BAREMETAL != 0)
	  while(fb_sem);
	  fb_sem = 1;
	#endif  // TOUCHGFX_BAREMETAL
	#if (TOUCHGFX_FREERTOS != 0)
	    osSemaphoreAcquire(frame_buffer_sem, osWaitForever);
	#endif // TOUCHGFX_FREERTOS
}

/*
 * Release the frame buffer semaphore.
 */
void OSWrappers::giveFrameBufferSemaphore()
{
	#if (TOUCHGFX_BAREMETAL != 0)
	  fb_sem = 0;
	#endif  // TOUCHGFX_BAREMETAL
	#if (TOUCHGFX_FREERTOS != 0)
	  osSemaphoreRelease(frame_buffer_sem);
	#endif // TOUCHGFX_FREERTOS
}

/*
 * Attempt to obtain the frame buffer semaphore. If semaphore is not available, do
 * nothing.
 *
 * Note must return immediately! This function does not care who has the taken the semaphore,
 * it only serves to make sure that the semaphore is taken by someone.
 */
void OSWrappers::tryTakeFrameBufferSemaphore()
{
	#if (TOUCHGFX_BAREMETAL != 0)
	  fb_sem = 1;
	#endif  // TOUCHGFX_BAREMETAL
	#if (TOUCHGFX_FREERTOS != 0)
	    osSemaphoreAcquire(frame_buffer_sem, 0);
	#endif // TOUCHGFX_FREERTOS
}

/*
 * Release the frame buffer semaphore in a way that is safe in interrupt context. Called
 * from ISR.
 *
 * Release the frame buffer semaphore in a way that is safe in interrupt context.
 * Called from ISR.
 */
void OSWrappers::giveFrameBufferSemaphoreFromISR()
{
	#if (TOUCHGFX_BAREMETAL != 0)
		fb_sem = 0;
	#endif  // TOUCHGFX_BAREMETAL
	#if (TOUCHGFX_FREERTOS != 0)
	    osSemaphoreRelease(frame_buffer_sem);
	#endif // TOUCHGFX_FREERTOS
}

/*
 * Signal that a VSYNC has occurred. Should make the vsync queue/mutex available.
 *
 * Note This function is called from an ISR, and should (depending on OS) trigger a
 * scheduling.
 */
void OSWrappers::signalVSync()
{
	#if (TOUCHGFX_BAREMETAL != 0)
	  vsync_sem = 1;
	#endif  // TOUCHGFX_BAREMETAL
	#if (TOUCHGFX_FREERTOS != 0)
	    osMessageQueuePut(vsync_queue, &dummy, 0, 0);
	#endif // TOUCHGFX_FREERTOS
}

/*
  * Signal that the rendering of the frame has completed. Used by
  * some systems to avoid using any previous vsync.
  */
void OSWrappers::signalRenderingDone()
{
	#if (TOUCHGFX_BAREMETAL != 0)
		vsync_sem = 0;
	#endif  // TOUCHGFX_BAREMETAL
	#if (TOUCHGFX_FREERTOS != 0)
		// Empty implementation for CMSIS V2
	#endif // TOUCHGFX_FREERTOS
}

/*
 * This function checks if a VSync occurred after last rendering.
 * The function is used in systems that cannot wait in  waitForVSync
 * (because they are also checking other event sources.
 *
 * @note signalRenderingDone is typically used together with this function.
 *
 * @return True if VSync occurred.
 */
bool OSWrappers::isVSyncAvailable()
{
	#if (TOUCHGFX_BAREMETAL != 0)
	  return vsync_sem;
	#endif  // TOUCHGFX_BAREMETAL
	#if (TOUCHGFX_FREERTOS != 0)
	  return true;
	#endif // TOUCHGFX_FREERTOS
}

/*
 * This function check if a VSYNC has occured.
 * If VSYNC has occured, signal TouchGFX to start a rendering
 */
void OSWrappers::waitForVSync()
{
	#if (TOUCHGFX_BAREMETAL != 0)
	  if(vsync_sem)
	  {
		vsync_sem = 0;
		HAL::getInstance()->backPorchExited();
	  }
	#endif  // TOUCHGFX_BAREMETAL

	#if (TOUCHGFX_FREERTOS != 0)
	  uint32_t dummyGet;
	  // First make sure the queue is empty, by trying to remove an element with 0 timeout.
	  osMessageQueueGet(vsync_queue, &dummyGet, 0, 0);

	  // Then, wait for next VSYNC to occur.
	  osMessageQueueGet(vsync_queue, &dummyGet, 0, osWaitForever);
	#endif // TOUCHGFX_FREERTOS
}

/*
 * A function that causes executing task to sleep for a number of milliseconds.
 *
 * A function that causes executing task to sleep for a number of milliseconds.
 * This function is OPTIONAL. It is only used by the TouchGFX in the case of
 * a specific frame refresh strategy (REFRESH_STRATEGY_OPTIM_SINGLE_BUFFER_TFT_CTRL).
 * Due to backwards compatibility, in order for this function to be useable by the HAL
 * the function must be explicitly registered:
 * hal.registerTaskDelayFunction(&OSWrappers::taskDelay)
 *
 * see HAL::setFrameRefreshStrategy(FrameRefreshStrategy s)
 * see HAL::registerTaskDelayFunction(void (*delayF)(uint16_t))
 */
void OSWrappers::taskDelay(uint16_t ms)
{
	#if (TOUCHGFX_BAREMETAL != 0)
		HAL_Delay(ms);
	#endif  // TOUCHGFX_BAREMETAL

	#if (TOUCHGFX_FREERTOS != 0)
		osDelay(static_cast<uint32_t>(ms));
	#endif // TOUCHGFX_FREERTOS

}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

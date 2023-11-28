#include "config/touchgfx-config.h"

#if (TOUCHGFX_BAREMETAL != 0)

#include "app_touchgfx.h"
#include "gui/common/FrontendHeap.hpp"
#include "touchgfxtask.h"

extern "C" void touchgfx_init();
extern "C" void touchgfx_taskEntry();

TouchGfxTask::TouchGfxTask():
    interval_(20)
{

}

void TouchGfxTask::initialize()
{
    touchgfx_init();

}

void TouchGfxTask::start()
{
    startBehavior();
}

XFEventStatus TouchGfxTask::processEvent()
{
    // Generate a timeout on startup. Want to call touchgfx_taskEntry()
    // every x milliseconds
    if (getCurrentEvent()->getEventType() == XFEvent::Initial)
    {
        scheduleTimeout(1, 20);
    }

    // Call touchgfx_taskEntry on timeout
    if (getCurrentEvent()->getEventType() == XFEvent::Timeout and
        getCurrentTimeout()->getId() == 1)
    {
        touchgfx_taskEntry();

        scheduleTimeout(1, 20);
    }

    return XFEventStatus::Consumed;
}

#endif	// TOUCHGFX_BAREMETAL

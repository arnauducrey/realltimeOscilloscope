#ifndef TOUCHGFXTASK_H
#define TOUCHGFXTASK_H

#include "config/touchgfx-config.h"

#if (TOUCHGFX_BAREMETAL != 0)
#include "xf/behavior.h"

/**
 * @brief TouchGFX task used in IDF and/or bare-metal environments to drive LCD display.
 */
class TouchGfxTask : public XFBehavior
{
public:
    TouchGfxTask();

    void initialize();
    void start();

protected:
    XFEventStatus processEvent() override;

protected:
    const int32_t interval_;
};

#endif // TOUCHGFX_BAREMETAL
#endif // TOUCHGFXTASK_H

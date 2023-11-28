#include "mcu/mcu.h"
#include "critical.h"

static volatile int inIsr_ = 0;
static volatile int enterCriticalNested_ = 0;

void critical_enter()
{
	// Only disable interrupts when not calling from an ISR
	if (!critical_inIsr())
	{
		if (!enterCriticalNested_)
		{
			// Turn off the priority configurable interrupts
		    __disable_irq();
		}
		enterCriticalNested_++;
	}
}

void critical_exit()
{
	// Only enable interrupts when not calling from an ISR
	if (!critical_inIsr())
	{
		enterCriticalNested_--;

		if (!enterCriticalNested_)
		{
			// Turn on the interrupts with configurable priority
		    __enable_irq();
		}
	}
}

int critical_inIsr()
{
    // Variable must be put to TRUE in every ISR to indicate execution
    // of an ISR and need to put back to FALSE before leaving ISR.
    return inIsr_;
}

void critical_setInIsr(bool inIsr)
{
    inIsr_ = inIsr;
}

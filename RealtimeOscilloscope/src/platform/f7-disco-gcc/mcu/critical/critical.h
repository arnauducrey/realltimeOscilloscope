#ifndef CRITICAL_H
#define CRITICAL_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void critical_enter();                  ///< Disables all interrupts.
void critical_exit();                   ///< Enables all interrupts.

int critical_inIsr();                   ///< Returns 1 if in an interrupt service routine (ISR), 0 otherwise.
void critical_setInIsr(bool inIsr);     ///< Set or clear 'bInISR' variable.

#ifdef __cplusplus
}
#endif

#endif /* CRITICAL_H */

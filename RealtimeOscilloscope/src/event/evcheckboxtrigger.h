#ifndef EV_CHECK_BOX_TRIGGER_H
#define EV_CHECK_BOX_TRIGGER_H

#include "xf/customevent.h"
#include "events.h"
#include <stdint.h>

class evCheckBoxTrigger : public XFCustomEvent
{
public:
    evCheckBoxTrigger(bool checked, int eventId);
    bool isChecked() const;
private:
    const bool checked;
};

#endif // EV_CHECK_BOX_TRIGGER_H

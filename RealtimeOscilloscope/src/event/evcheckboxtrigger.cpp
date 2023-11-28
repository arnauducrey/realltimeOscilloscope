#include "events.h"
#include "evcheckboxtrigger.h"

evCheckBoxTrigger::evCheckBoxTrigger(bool checked, int eventId) :
    XFCustomEvent(eventId),
    checked(checked)
{

}

bool evCheckBoxTrigger::isChecked() const {
	return checked;
}

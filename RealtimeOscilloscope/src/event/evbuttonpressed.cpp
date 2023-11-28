#include "events.h"
#include "evbuttonpressed.h"

evButtonPressed::evButtonPressed(int buttonIndex) :
    XFCustomEvent(evButtonPressedId)
{
	_buttonIndex = buttonIndex;
}

uint16_t evButtonPressed::buttonIndex() {
	return _buttonIndex;
}

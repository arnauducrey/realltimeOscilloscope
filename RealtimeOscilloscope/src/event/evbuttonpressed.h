#ifndef EVBUTTONPRESSED_H
#define EVBUTTONPRESSED_H

#include "xf/customevent.h"
#include "events.h"
#include <stdint.h>

class evButtonPressed : public XFCustomEvent
{
public:
    evButtonPressed(int buttonIndex);
    uint16_t buttonIndex();
private:
    uint16_t _buttonIndex;

};

#endif // EVBUTTONPRESSED_H

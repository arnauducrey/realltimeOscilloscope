#include "xf/timeout.h"

XFTimeout::XFTimeout(int id, int interval, interface::XFReactive * pBehavior)
 : XFEvent(XFEvent::Timeout, id, pBehavior),
   interval_(interval),
   relTicks_(interval)
{
}

bool XFTimeout::operator ==(const XFTimeout & timeout) const
{
    // Check behavior and timeout id attributes, if there are equal
    return (pBehavior_ == timeout.pBehavior_ && getId() == timeout.getId()) ? true : false;
}

bool XFTimeout::deleteAfterConsume() const
{
    return true;
}

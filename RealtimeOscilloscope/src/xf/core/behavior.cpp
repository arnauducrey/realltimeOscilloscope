#include <cassert>
#include "xf/xf.h"
#include "xf/interface/resourcefactory.h"
#include "xf/interface/dispatcher.h"
#include "xf/timeout.h"
#include "xf/initialevent.h"
#include "xf/behavior.h"

using interface::XFResourceFactory;

XFBehavior::XFBehavior(bool ownDispatcher /* = false */) :
    pDispatcher_(nullptr),
    hasOwnDispatcher_(ownDispatcher),
    deleteOnTerminate_(false),
    pCurrentEvent_(nullptr)
{
    if (ownDispatcher)
    {
        pDispatcher_ = XFResourceFactory::getInstance()->createDispatcher();
    }
    else
    {
        pDispatcher_ = XFResourceFactory::getInstance()->getDefaultDispatcher();
    }

    // Force hasOwnDispatcher attribute to false in case actual
    // dispatcher is the default dispatcher. This may happen
    // if in an IDF an other dispatcher is requested, but
    // only the default dispatcher is present.
    if (pDispatcher_ == XFResourceFactory::getInstance()->getDefaultDispatcher())
    {
        hasOwnDispatcher_ = false;
    }
}

XFBehavior::~XFBehavior()
{

}

void XFBehavior::startBehavior()
{
    // Send initial event to state machine
    GEN(XFInitialEvent());

    // Start dispatcher if behavior has its own dispatcher
    if (hasOwnDispatcher())
    {
        getDispatcher()->start();
    }
}

void XFBehavior::pushEvent(XFEvent * pEvent, bool fromISR)
{
    // Set owner
    pEvent->setBehavior(this);
    // Push to dispatchers event queue
    getDispatcher()->pushEvent(pEvent, fromISR);
}

bool XFBehavior::deleteOnTerminate() const
{
    return deleteOnTerminate_;
}

void XFBehavior::setDeleteOnTerminate(bool deleteBehaviour)
{
    deleteOnTerminate_ = deleteBehaviour;
}

XFBehavior::TerminateBehavior XFBehavior::process(const XFEvent * pEvent)
{
    XFEventStatus eventStatus;

    setCurrentEvent(pEvent);

    eventStatus = processEvent();

    setCurrentEvent(nullptr);

    return (eventStatus == XFEventStatus::Terminate);
}

interface::XFDispatcher * XFBehavior::getDispatcher()
{
    return pDispatcher_;
}

void XFBehavior::setCurrentEvent(const XFEvent * pEvent)
{
    pCurrentEvent_ = pEvent;
}

const XFEvent * XFBehavior::getCurrentEvent() const
{
    return pCurrentEvent_;
}

const XFTimeout * XFBehavior::getCurrentTimeout()
{
    assert(getCurrentEvent()->getEventType() == XFEvent::Timeout);

    return static_cast<const XFTimeout *>(getCurrentEvent());
}

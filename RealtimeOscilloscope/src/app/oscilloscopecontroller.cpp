#include <assert.h>
#include "trace/trace.h"
#include "xf/eventstatus.h"
#include "event/evcheckboxtrigger.h"
//#include "main.h"
#include "gui.h"
#include "oscilloscopecontroller.h"

namespace oscilloscope {

Controller * Controller::_pInstance(nullptr);

const TDivOption Controller::_tdivOptions[] = {{TDIV_500us, "500 us / div"},
                                               {TDIV_1ms,     "1 ms / div"},
                                               {TDIV_2ms,     "2 ms / div"},
                                               {TDIV_5ms,     "5 ms / div"},
                                               {TDIV_10ms,   "10 ms / div"}};

Controller::Controller() :
    _pGui(nullptr),
    _adcValuesBuffer(nullptr),
	_adcValuesBufferSize(0),
	_tdivValue(TDIV_1ms)
{
    assert(!_pInstance);    // Only one instance of this class allowed!
    _pInstance = this;
}

//static
Controller & Controller::getInstance()
{
    assert(_pInstance);     // Create first an instance!
    return *_pInstance;
}

void Controller::initialize(Gui & gui, uint16_t * adcValuesBuffer, uint32_t adcValuesBufferSize)
{
    _pGui = &gui;
    _adcValuesBuffer = adcValuesBuffer;
    _adcValuesBufferSize = adcValuesBufferSize;

    gui.registerObserver(this);     // Get notified about GUI events
}

void Controller::start()
{
    startBehavior();
}

XFEventStatus Controller::processEvent()
{
    if (!_adcValuesBuffer)
    {
        Trace::out("oscilloscope::Controller Error: Attribute '_adcValuesBuffer' not set!");
    }

    assert(_adcValuesBuffer);
    assert(_adcValuesBufferSize > 0);

	if (getCurrentEvent()->getEventType() == XFEvent::Initial)
	{
		scheduleTimeout(TIMEOUT_ID, TIMEOUT_INTERVAL);

		doShowAnalogSignal();
	}

	if (getCurrentEvent()->getEventType() == XFEvent::Timeout &&
		getCurrentTimeout()->getId() == TIMEOUT_ID)
	{
		scheduleTimeout(TIMEOUT_ID, TIMEOUT_INTERVAL);

		doShowAnalogSignal();
	}

	if (getCurrentEvent()->getEventType() == XFEvent::Event &&
		getCurrentEvent()->getId() == BTN_PLUS_ID)
	{
		doButtonTimePlusPressed();
	}

	if (getCurrentEvent()->getEventType() == XFEvent::Event &&
		getCurrentEvent()->getId() == BTN_MINUS_ID)
	{
		doButtonTimeMinusPressed();
	}

	return XFEventStatus::Consumed;
}

void Controller::onButtonTimePlusPressed()
{
	XFEvent* evButtonPlus =new XFEvent(XFEvent::Event, BTN_PLUS_ID, this);

	pushEvent(evButtonPlus);
}

void Controller::onButtonTimeMinusPressed()
{
	XFEvent* evButtonMinus =new XFEvent(XFEvent::Event, BTN_MINUS_ID, this);

	pushEvent(evButtonMinus);
}

void Controller::onCheckBoxTriggerCheckState(bool checked)
{
    Trace::out("Trigger checkbox: %s", (checked) ? "checked" : "unchecked");

    GEN(evCheckBoxTrigger(checked, CHECK_BOX_TRIGGER_ID));
}
#define SAMPLINGRATE 100000.0
#define MAXPIX 450.0
void Controller::doShowAnalogSignal()
{
	// TODO: Call gui().drawGraphPoints() with the appropriate data.

	switch (getTDivValue()) {
		case TDivValue::TDIV_500us:
			gui().drawGraphPoints(_adcValuesBuffer, _adcValuesBufferSize,(float) SAMPLINGRATE*0.5*8 / (MAXPIX * 1000.0));
			//gui().drawGraphPoints(_adcValuesBuffer, _adcValuesBufferSize,(float) _adcValuesBufferSize/ (MAXPIX*20.0));
			break;
		case TDivValue::TDIV_1ms:
			gui().drawGraphPoints(_adcValuesBuffer, _adcValuesBufferSize,(float) SAMPLINGRATE*1*8 / (MAXPIX * 1000.0));
			//gui().drawGraphPoints(_adcValuesBuffer, _adcValuesBufferSize,(float) _adcValuesBufferSize/ (MAXPIX*10.0));
			break;
		case TDivValue::TDIV_2ms:
			gui().drawGraphPoints(_adcValuesBuffer, _adcValuesBufferSize,(float) SAMPLINGRATE*2*8 / (MAXPIX * 1000.0));
			//gui().drawGraphPoints(_adcValuesBuffer, _adcValuesBufferSize,(float) _adcValuesBufferSize/ (MAXPIX*5.0));
			break;
		case TDivValue::TDIV_5ms:
			gui().drawGraphPoints(_adcValuesBuffer, _adcValuesBufferSize,(float) SAMPLINGRATE*5*8 / (MAXPIX * 1000.0));
			//gui().drawGraphPoints(_adcValuesBuffer, _adcValuesBufferSize,(float) _adcValuesBufferSize/ (MAXPIX*2.0));
			break;
		case TDivValue::TDIV_10ms:
			gui().drawGraphPoints(_adcValuesBuffer, _adcValuesBufferSize,(float) SAMPLINGRATE*10*8 / (MAXPIX * 1000.0));
			//gui().drawGraphPoints(_adcValuesBuffer, _adcValuesBufferSize,(float) _adcValuesBufferSize/ (MAXPIX));
			break;
		default:
			break;
	}
}

void Controller::doButtonTimePlusPressed()
{
    if (_tdivValue < (TDIV_MAX - 1))
    {
        _tdivValue = (TDivValue)(_tdivValue + 1);

        gui().setTimeDivisionText(getText(_tdivValue));
    }
}

void Controller::doButtonTimeMinusPressed()
{
    if (_tdivValue > (TDIV_MIN + 1))
    {
        _tdivValue = (TDivValue)(_tdivValue - 1);

        gui().setTimeDivisionText(getText(_tdivValue));
    }
}

std::string Controller::getText(TDivValue tdivValue)
{
    const uint32_t count = sizeof(_tdivOptions)/sizeof(_tdivOptions[0]);

    for (uint32_t i = 0; i < count; i++)
    {
        if (_tdivOptions[i].tdivValue == tdivValue)
        {
            return _tdivOptions[i].text;
        }
    }
    return "n/a";
}

}   // namespace oscilloscope

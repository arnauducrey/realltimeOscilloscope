#include "freqgencontroller.h"
#include "factory.h"
#include "trace/trace.h"
#include <assert.h>
#include "event/evButtonPressed.h"
#include "event/events.h"

namespace oscilloscope
{

FreqGenController* FreqGenController::_instance = nullptr;

FreqGenController::FreqGenController():
    _pGui(nullptr)
{
    assert (_instance==nullptr);
    _instance = this;
    if (EXTFREQGEN_START_FREQUENCY == HZ_50   ||
    	EXTFREQGEN_START_FREQUENCY == HZ_110  ||
		EXTFREQGEN_START_FREQUENCY == HZ_240  ||
		EXTFREQGEN_START_FREQUENCY == HZ_370  ||
		EXTFREQGEN_START_FREQUENCY == HZ_500  ||
		EXTFREQGEN_START_FREQUENCY == HZ_700  ||
		EXTFREQGEN_START_FREQUENCY == HZ_1000 ||
		EXTFREQGEN_START_FREQUENCY == HZ_5000 ||
		EXTFREQGEN_START_FREQUENCY == HZ_10000)
    {
    	_frequency = EXTFREQGEN_START_FREQUENCY;
    }
    else
    {
    	_frequency = HZ_1000;
    }
    if (EXTFREQGEN_START_WAVEFORM == SINUS   ||
    	EXTFREQGEN_START_WAVEFORM == TRIANGLE   ||
		EXTFREQGEN_START_WAVEFORM == SQUARE)
    {
    	_mode = EXTFREQGEN_START_WAVEFORM;
    }
    else
    {
    	_mode = SINUS;
    }
    change();

    state = ST_INIT;
}

void FreqGenController::onButtonChanged(uint16_t buttonIndex, bool pressed) {
	if (pressed)
	{
			GEN(evButtonPressed(buttonIndex));
	}
}

FreqGenController* FreqGenController::getInstance() {
	return _instance;
}


 XFEventStatus FreqGenController::processEvent() {
	XFEventStatus retVal = XFEventStatus::NotConsumed;
	FCG_STATES oldState = state;
	const XFEvent* ev = getCurrentEvent();
	switch (state)
	{
	case ST_INIT:
		if (ev->getEventType()==XFEvent::Initial)
		{
			state = ST_WAITBUTTON;
		}
		break;
	case ST_WAITBUTTON:
		if (ev->getEventType()==XFEvent::Event && ev->getId() == evButtonPressedId)
		{
			switch (((evButtonPressed*) ev)->buttonIndex())
			{
				case 1:
					state = ST_CHANGEFREQDOWN;
					break;
				case 2:
					state = ST_CHANGEFREQUP;
					break;
				case 3:
					state = ST_CHANGEMODE;
						break;
				default:
					break;
			}
		}
		break;
	case ST_CHANGEFREQUP:
	case ST_CHANGEFREQDOWN:
	case ST_CHANGEMODE:
		if (ev->getEventType()==XFEvent::NullTransition )
		{
			state = ST_WAITBUTTON;
		}
		break;
	default:
		break;
	}
	if (oldState != state)
	{
		retVal = XFEventStatus::Consumed;
		switch (state)
		{
		case ST_INIT:
			break;
		case ST_WAITBUTTON:
			break;
		case ST_CHANGEFREQUP:
			frequencyUp();
			change();
			GEN(XFNullTransition());
			break;
		case ST_CHANGEFREQDOWN:
			frequencyDown();
			change();
			GEN(XFNullTransition());
			break;
		case ST_CHANGEMODE:
			shiftMode();
			change();
			GEN(XFNullTransition());
			break;
		default:
			break;
		}
	}
	return retVal;
}

void FreqGenController::start() {
	startBehavior();
}

void FreqGenController::frequencyUp() {
	//change the frequency up
	switch (_frequency)
	{
	case HZ_50:
		_frequency = HZ_110;
		break;
	case HZ_110:
		_frequency = HZ_240;
		break;
	case HZ_240:
		_frequency = HZ_370;
		break;
	case HZ_370:
		_frequency = HZ_500;
		break;
	case HZ_500:
		_frequency = HZ_700;
		break;
	case HZ_700:
		_frequency = HZ_1000;
		break;
	case HZ_1000:
		_frequency = HZ_5000;
		break;
	case HZ_5000:
		_frequency = HZ_10000;
		break;
	case HZ_10000:
		_frequency = HZ_50;
		break;
	default:
		break;
	}

	Trace::out("Frequency up to %s", toString(_frequency).c_str());

	// Update LCD display
	gui().setFreqGenFrequencyText(toString(_frequency));
}

void FreqGenController::frequencyDown() {
	//change the frequency down
	switch (_frequency)
	{
	case HZ_50:
		_frequency = HZ_10000;
		break;
	case HZ_110:
		_frequency = HZ_50;
		break;
	case HZ_240:
		_frequency = HZ_110;
		break;
	case HZ_370:
		_frequency = HZ_240;
		break;
	case HZ_500:
		_frequency = HZ_370;
		break;
	case HZ_700:
		_frequency = HZ_500;
		break;
	case HZ_1000:
		_frequency = HZ_700;
		break;
	case HZ_5000:
		_frequency = HZ_1000;
		break;
	case HZ_10000:
		_frequency = HZ_5000;
		break;
	default:
		break;
	}

	Trace::out("Frequency down to %s", toString(_frequency).c_str());

	// Update LCD display
	gui().setFreqGenFrequencyText(toString(_frequency));
}

void FreqGenController::shiftMode() {
	//change the waveform
	switch (_mode)
	{
	case SINUS:
		_mode = TRIANGLE;
		if (EXTFREQGEN_NOTIFICATION_TRACE == 1)
		{
			Trace::out("Triangle\n");
			gui().setFreqGenWaveformText("Tria -");
		}
		break;
	case TRIANGLE :
		_mode = SQUARE;
		if (EXTFREQGEN_NOTIFICATION_TRACE == 1)
		{
		    Trace::out("Square\n");
            gui().setFreqGenWaveformText("Rect -");
		}
		break;
	case SQUARE :
		_mode = SINUS;
		if (EXTFREQGEN_NOTIFICATION_TRACE == 1)
		{
		    Trace::out("Sine\n");
            gui().setFreqGenWaveformText("Sine -");
		}
		break;
	default:
		break;
	}
}

void FreqGenController::initialize(oscilloscope::Gui & gui) {
    _pGui = &gui;
    Factory::getFrequencyGenerator().setFrequency(_frequency);
    Factory::getFrequencyGenerator().setWaveForm(_mode);
}

void FreqGenController::change() {
	Factory::getFrequencyGenerator().setWaveForm(_mode);
	Factory::getFrequencyGenerator().setFrequency(_frequency);
}

//static
std::string FreqGenController::toString(const Frequency & frequency)
{
    std::string freqString("n/a");

    switch (frequency)
    {
    case HZ_50:
        freqString = "50 Hz";
        break;
    case HZ_110:
        freqString = "110 Hz";
        break;
    case HZ_240:
        freqString = "240 Hz";
        break;
    case HZ_370:
        freqString = "370 Hz";
        break;
    case HZ_500:
        freqString = "500 Hz";
        break;
    case HZ_700:
        freqString = "700 Hz";
        break;
    case HZ_1000:
        freqString = "1 kHz";
        break;
    case HZ_5000:
        freqString = "5 kHz";
        break;
    case HZ_10000:
        freqString = "10 kHz";
        break;
    }

    return freqString;
}

} /* namespace oscilloscope */



#ifndef APP_FREQGENCONTROLLER_H
#define APP_FREQGENCONTROLLER_H

#include <cassert>
#include <stdint.h>
#include "config/ext-freqgen-config.h"
#include "xf/behavior.h"
#include "board/interface/buttonscontrollercallbackprovider.h"
#include "ext-26pin/ext_26pin.h"

namespace oscilloscope
{

class Gui;

typedef enum _Frequency
{
	HZ_50 = 50,
	HZ_110 =110,
	HZ_240 = 240,
	HZ_370 = 370,
	HZ_500 = 500,
	HZ_700 = 700,
	HZ_1000 = 1000,
	HZ_5000 = 5000,
	HZ_10000 = 10000
} Frequency;

/**
 * @brief Frequency generator controller to drive external MinGen module with buttons.
 */
class FreqGenController : public interface::ButtonsControllerCallbackProvider,
						  public XFBehavior
{
//internal types
public:
	typedef enum _FGC_STATES
	{
		ST_INIT,
		ST_WAITBUTTON,
		ST_CHANGEFREQUP,
		ST_CHANGEFREQDOWN,
		ST_CHANGEMODE
	} FCG_STATES;

public:
    FreqGenController();
    static FreqGenController* getInstance();

//interface ButtonsControllerCallbackProvider
public:
    void onButtonChanged(uint16_t buttonIndex, bool pressed);

//interface XFBehavior
public:
    XFEventStatus processEvent();
    void start();
    void initialize(oscilloscope::Gui & gui);

protected:
    inline oscilloscope::Gui & gui() const { assert(_pGui); return *_pGui; }

// internal methods
private:
    void frequencyUp();
    void frequencyDown();
    void shiftMode();
    void change();

    static std::string toString(const Frequency & frequency);

private:
    static FreqGenController* _instance;
    oscilloscope::Gui * _pGui;
    Frequency _frequency;
    f_mode _mode;
    FCG_STATES state;
};

} /* namespace oscilloscope */
#endif // APP_FREQGENCONTROLLER_H

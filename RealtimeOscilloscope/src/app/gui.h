#ifndef APP_GUI_H
#define APP_GUI_H

#include "interface/guiobserver.h"

namespace oscilloscope {

/**
 * @brief The GUI (Graphical User Interface) class is handling the oscilloscope view on the LCD.
 *
 * The GUI class uses a graphical library to show the oscilloscope view.
 *
 * Internally, all requests are forwarded to the TouchGFX generated classes.
 *
 * Note: Keep in mind that the internal graph element provides only a maximum of 460 data points!
 */
class Gui
{
public:
    Gui();

    static Gui* getInstance();

    bool registerObserver(interface::GuiObserver * pGuiObserver);
    void start();

    void drawGraphPoints(uint16_t * values, uint16_t maxSize, float xScale = 1);

    bool isRedLedEnabled() const;
    void setRedLed(bool enable);

    bool isOscilloscopePageEnabled() const;

    void setTimeDivisionText(std::string text);
    void setFreqGenWaveformText(std::string text);
    void setFreqGenFrequencyText(std::string text);

    inline float getXAxisPixelsPerField() const { return _xAxisPixelsPerField; }

    void onButtonTimePlusPressed();
    void onButtonTimeMinusPressed();
    void onCheckBoxTriggerCheckState(bool checked);

protected:
    inline interface::GuiObserver & observer() const { assert(_pGuiObserver); return *_pGuiObserver; }

protected:
    static Gui* _instance;
    interface::GuiObserver * _pGuiObserver;         ///< GUI observer notified about GUI events.
    bool _redLedOn;
    float _xAxisPixelsPerField;                     ///< Number of pixels per field on the x-axis.
};

}   // namespace oscilloscope
#endif // APP_GUI_H

#include <cstring>
#include <cassert>
#include <cmath>
#include "trace/trace.h"
#include "gui.h"
#include "TouchGFX/gui/include/gui/mainview_screen/MainViewView.hpp"
#include "TouchGFX/gui/include/gui/model/Model.hpp"

#define TOUCH_THREAD_STACK_SIZE     2048

namespace oscilloscope {

Gui* Gui::_instance = nullptr;

Gui::Gui() :
    _pGuiObserver(nullptr),
    _redLedOn(false),
    _xAxisPixelsPerField(roundf(460.0/8))     // Width of the chart graph divided by the number of divisions to show
{
	if(_instance==nullptr){
		    _instance = this;
		}
}

Gui* Gui::getInstance() {
	return _instance;
}

bool Gui::registerObserver(interface::GuiObserver * pGuiObserver)
{
    if (!_pGuiObserver and pGuiObserver)
    {
        _pGuiObserver = pGuiObserver;
        return true;
    }
    return false;
}

void Gui::start()
{
}

void Gui::drawGraphPoints(uint16_t * values, uint16_t maxSize, float xScale /* = 1 */)
{
	Model* model = Model::getInstance();
	if(model!=nullptr){
		model->setGraphPoints(values, maxSize, xScale);
	}
}

bool Gui::isRedLedEnabled() const
{
    return _redLedOn;
}

void Gui::setRedLed(bool enable)
{
    _redLedOn = enable;
}

bool Gui::isOscilloscopePageEnabled() const
{
	return true;
}

void Gui::setTimeDivisionText(std::string text)
{
	Model* model = Model::getInstance();
	if(model!=nullptr){
		model->setDivTimLabel(text);
	}
}

void Gui::setFreqGenWaveformText(std::string text)
{
	Model* model = Model::getInstance();
	if(model!=nullptr){
		model->setModeSignal(text);
	}
}

void Gui::setFreqGenFrequencyText(std::string text)
{
	Model* model = Model::getInstance();
	if(model!=nullptr){
		model->setFreqSignal(text);
	}
}

void Gui::onButtonTimePlusPressed()
{
    _pGuiObserver->onButtonTimePlusPressed();
}

void Gui::onButtonTimeMinusPressed()
{
    _pGuiObserver->onButtonTimeMinusPressed();
}

void Gui::onCheckBoxTriggerCheckState(bool checked)
{
    _pGuiObserver->onCheckBoxTriggerCheckState(checked);
}

}   // namespace oscilloscope

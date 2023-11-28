#include <cassert>
#include "gui/mainview_screen/MainViewView.hpp"
#include "app/gui.h"

MainViewView* MainViewView::_instance = nullptr;

MainViewView::MainViewView()
{
    assert(!_instance);
	if(_instance==nullptr){
	    _instance = this;
	}
}

MainViewView* MainViewView::getInstance() {
	return _instance;
}

void MainViewView::setupScreen()
{
    MainViewViewBase::setupScreen();
}

void MainViewView::tearDownScreen()
{
    MainViewViewBase::tearDownScreen();
}

void MainViewView::onBtnPlusPressed(){
	oscilloscope::Gui::getInstance()->onButtonTimePlusPressed();
}

void MainViewView::onBtnMinusPressed(){
    oscilloscope::Gui::getInstance()->onButtonTimeMinusPressed();
}

void MainViewView::onCheckBoxTriggerPressed(){

    // Get actual state of the trigger checkbox by reading the status of the red LED
    const bool checked = !redLedDisable.isVisible();
    // Notify GUI
    oscilloscope::Gui::getInstance()->onCheckBoxTriggerCheckState(checked);
}

void MainViewView::setModeSignal(const std::string str){
	Unicode::fromUTF8((const unsigned char *)str.c_str(),labelSignalBuffer1,LABELSIGNALBUFFER1_SIZE);
	labelSignal.invalidate();
}

void MainViewView::setFreqSignal(const std::string str){
	Unicode::fromUTF8((const unsigned char *)str.c_str(),labelSignalBuffer2,LABELSIGNALBUFFER2_SIZE);
	labelSignal.invalidate();
}

void MainViewView::setLabelDivTime(const std::string str){
	Unicode::fromUTF8((const unsigned char *)str.c_str(),labelDivTimeBuffer,LABELDIVTIME_SIZE);
    labelDivTime.invalidate();
}

void MainViewView::drawGraph(uint16_t* values, uint16_t maxSize, float xScale /* = 1 */){
	chart.clear();

	chartMajorXAxisGrid.setInterval(460/8);     // Show 8 division on the chart

	chart.setGraphRangeX(0, 460-1);

	for(float addedPoints = 0, valuesIndex = 0;
	    (uint32_t)valuesIndex < maxSize and addedPoints < 460;
	    addedPoints++)
	{
		chart.addDataPoint(values[(uint32_t)valuesIndex]);
		valuesIndex = addedPoints * xScale;
	}
	chart.invalidate();
}

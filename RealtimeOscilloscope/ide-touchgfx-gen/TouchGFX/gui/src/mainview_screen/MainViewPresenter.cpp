#include <gui/mainview_screen/MainViewView.hpp>
#include <gui/mainview_screen/MainViewPresenter.hpp>

MainViewPresenter::MainViewPresenter(MainViewView& v)
    : view(v)
{

}

void MainViewPresenter::activate()
{

}

void MainViewPresenter::deactivate()
{

}

void MainViewPresenter::notifyModeChanged(std::string newMode){
	view.setModeSignal(newMode);
}

void MainViewPresenter::notifyFreqChanged(std::string newFreq){
	view.setFreqSignal(newFreq);
}

void MainViewPresenter::notifyDivTimChanged(std::string newDivTim){
	view.setLabelDivTime(newDivTim);
}

void MainViewPresenter::notifyDrawGraph(uint16_t * values, uint16_t maxSize, float xScale /* = 1 */){
	view.drawGraph(values, maxSize, xScale);
};

#ifndef MAINVIEWPRESENTER_HPP
#define MAINVIEWPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class MainViewView;

class MainViewPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    MainViewPresenter(MainViewView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~MainViewPresenter() {};

    void notifyModeChanged(std::string newMode) override;
    void notifyFreqChanged(std::string newFreq) override;
    void notifyDivTimChanged(std::string newDivTim) override;
    void notifyDrawGraph(uint16_t* values, uint16_t maxSize, float xScale = 1) override;

private:
    MainViewPresenter();

    MainViewView& view;
};

#endif // MAINVIEWPRESENTER_HPP


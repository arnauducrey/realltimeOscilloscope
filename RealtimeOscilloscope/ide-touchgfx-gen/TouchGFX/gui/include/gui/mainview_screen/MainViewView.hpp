#ifndef MAINVIEWVIEW_HPP
#define MAINVIEWVIEW_HPP

#include <gui_generated/mainview_screen/MainViewViewBase.hpp>
#include <gui/mainview_screen/MainViewPresenter.hpp>

#include <string>

class MainViewView : public MainViewViewBase
{
public:
    MainViewView();
    virtual ~MainViewView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    static MainViewView* getInstance();

    void onBtnPlusPressed() override;
    void onBtnMinusPressed() override;
    void onCheckBoxTriggerPressed() override;

    void setModeSignal(const std::string str);
    void setFreqSignal(const std::string str);
    void setLabelDivTime(const std::string str);
    void drawGraph(uint16_t * values, uint16_t maxSize, float xScale = 1);

protected:
    /*
     * Singleton
     */
    static MainViewView* _instance;
};

#endif // MAINVIEWVIEW_HPP

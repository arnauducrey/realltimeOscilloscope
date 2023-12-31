/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef MAINVIEWVIEWBASE_HPP
#define MAINVIEWVIEWBASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <mvp/View.hpp>
#include <gui/mainview_screen/MainViewPresenter.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <touchgfx/widgets/graph/GraphWrapAndClear.hpp>
#include <touchgfx/widgets/graph/GraphElements.hpp>
#include <touchgfx/widgets/canvas/PainterRGB565.hpp>
#include <touchgfx/widgets/Button.hpp>
#include <touchgfx/widgets/Image.hpp>

class MainViewViewBase : public touchgfx::View<MainViewPresenter>
{
public:
    MainViewViewBase();
    virtual ~MainViewViewBase() {}
    virtual void setupScreen();

    /*
     * Virtual Action Handlers
     */
    virtual void onBtnMinusPressed()
    {
        // Override and implement this function in MainView
    }

    virtual void onBtnPlusPressed()
    {
        // Override and implement this function in MainView
    }

    virtual void onCheckBoxTriggerPressed()
    {
        // Override and implement this function in MainView
    }

protected:
    FrontendApplication& application() {
        return *static_cast<FrontendApplication*>(touchgfx::Application::getInstance());
    }

    /*
     * Member Declarations
     */
    touchgfx::Box __background;
    touchgfx::Box bgView;
    touchgfx::TextArea labelTitle;
    touchgfx::Box bgSignal;
    touchgfx::TextAreaWithTwoWildcards labelSignal;
    touchgfx::TextAreaWithOneWildcard labelDivTime;
    touchgfx::Box chartBG;
    touchgfx::GraphWrapAndClear<460> chart;
    touchgfx::GraphElementDiamonds chartDiamonds1;
    touchgfx::PainterRGB565 chartDiamonds1Painter;
    touchgfx::GraphElementGridX chartMajorXAxisGrid;
    touchgfx::Button btnPlus;
    touchgfx::Button btnMinus;
    touchgfx::Button btnTrigger;
    touchgfx::TextArea labelTrigger;
    touchgfx::TextArea labelPlus;
    touchgfx::TextArea labelMinus;
    touchgfx::Image redLedEnable;
    touchgfx::Image redLedDisable;

    /*
     * Wildcard Buffers
     */
    static const uint16_t LABELSIGNALBUFFER1_SIZE = 20;
    touchgfx::Unicode::UnicodeChar labelSignalBuffer1[LABELSIGNALBUFFER1_SIZE];
    static const uint16_t LABELSIGNALBUFFER2_SIZE = 20;
    touchgfx::Unicode::UnicodeChar labelSignalBuffer2[LABELSIGNALBUFFER2_SIZE];
    static const uint16_t LABELDIVTIME_SIZE = 20;
    touchgfx::Unicode::UnicodeChar labelDivTimeBuffer[LABELDIVTIME_SIZE];

private:

    /*
     * Callback Declarations
     */
    touchgfx::Callback<MainViewViewBase, const touchgfx::AbstractButton&> buttonCallback;

    /*
     * Callback Handler Declarations
     */
    void buttonCallbackHandler(const touchgfx::AbstractButton& src);

    /*
     * Canvas Buffer Size
     */
    static const uint16_t CANVAS_BUFFER_SIZE = 7200;
    uint8_t canvasBuffer[CANVAS_BUFFER_SIZE];
};

#endif // MAINVIEWVIEWBASE_HPP

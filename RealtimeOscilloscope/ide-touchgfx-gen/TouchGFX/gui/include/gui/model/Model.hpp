#ifndef MODEL_HPP
#define MODEL_HPP

#include <string>
#include "touchgfx-config.h"
#if (TOUCHGFX_FREERTOS != 0)
#include "FreeRTOS.h"
#include "queue.h"
#endif // TOUCHGFX_FREERTOS

class ModelListener;
#if (TOUCHGFX_FREERTOS != 0)
enum GUI_EVENT{
	UNDEFINED=0,
	FREQUENCY_EVENT,
	MODE_EVENT,
	DIVTIM_EVENT,
	GRAPH_EVENT
};
#endif // TOUCHGFX_FREERTOS

class Model
{
public:
    Model();

    static Model* getInstance();

    void bind(ModelListener* listener)
    {
        modelListener = listener;
    }

    void tick();

    void setModeSignal(const std::string str);
    void setFreqSignal(const std::string str);
    void setDivTimLabel(const std::string str);
    void setGraphPoints(uint16_t * values, uint16_t maxSize, float xScale = 1);
protected:
    ModelListener* modelListener;
    static Model* _instance;
    std::string textMode;
    std::string textFreq;
    std::string textDivTim;
    uint16_t* values;               ///< Pointer to signal data.
    uint16_t maxSize;               ///< Maximum size of 'values' array.
    float xScale;                   ///< X axis scaling factor.
#if (TOUCHGFX_BAREMETAL != 0)
    bool flagMode;
    bool flagDivTim;
    bool flagFreq;
    bool flagGraph;
#endif // TOUCHGFX_BAREMETAL
#if (TOUCHGFX_FREERTOS != 0)
    xQueueHandle gui_msg_q;
#endif // TOUCHGFX_FREERTOS

};

#endif // MODEL_HPP

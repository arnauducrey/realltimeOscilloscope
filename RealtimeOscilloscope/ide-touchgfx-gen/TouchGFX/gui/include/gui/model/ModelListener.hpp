#ifndef MODELLISTENER_HPP
#define MODELLISTENER_HPP

#include <gui/model/Model.hpp>

class ModelListener
{
public:
    ModelListener() : model(0) {}
    
    virtual ~ModelListener() {}

    void bind(Model* m)
    {
        model = m;
    }

    virtual void notifyModeChanged(std::string newMode){};
    virtual void notifyFreqChanged(std::string newFreq){};
    virtual void notifyDivTimChanged(std::string newDivTim){};
    virtual void notifyDrawGraph(uint16_t * values, uint16_t maxSize, float xScale = 1){};
protected:
    Model* model;
};

#endif // MODELLISTENER_HPP

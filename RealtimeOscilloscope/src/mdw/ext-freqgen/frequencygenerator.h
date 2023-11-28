#ifndef EXT_FREQGEN_FREQUENCYGENERATOR_H
#define EXT_FREQGEN_FREQUENCYGENERATOR_H

#include "ext-26pin/ext_26pin.h"

namespace external
{

/**
 * @brief FrequencyGenerator class providing access to external frequency generator "MiniGen" module.
 */
class FrequencyGenerator
{
public:
    FrequencyGenerator();
    void initialize();
    void setWaveForm(f_mode mode);
    void setFrequency(uint32_t frequency);

public: //singleton
    static FrequencyGenerator* getInstance();

private: //methods
    void setGen();

private:
    uint32_t _frequency;
    f_mode _mode;
    bool _initialized;
    static FrequencyGenerator* _instance;
};

} /* namespace external */
#endif // EXT_FREQGEN_FREQUENCYGENERATOR_H

#include "frequencygenerator.h"
#include "trace/trace.h"
#include <assert.h>

namespace external
{

FrequencyGenerator* FrequencyGenerator::_instance = nullptr;
FrequencyGenerator::FrequencyGenerator()
{
	assert (_instance==nullptr);
	_instance = this;
	_mode = SINUS;
	_frequency = 500;
	_initialized = false;

}

void FrequencyGenerator::initialize() {
	if (Ext_FreqGen_Init() == 0)
	{
		_initialized = true;
	}
}

void FrequencyGenerator::setWaveForm(f_mode mode) {
	_mode = mode;
	setGen();
}

void FrequencyGenerator::setFrequency(uint32_t frequency) {
	_frequency = frequency;
	setGen();
}

FrequencyGenerator* FrequencyGenerator::getInstance() {
	return _instance;
}

void FrequencyGenerator::setGen() {
	if (_initialized )
	{
		Ext_FreqGen_Set(_frequency, _mode);
	}
	else
	{
		trace_out("Frequency Generator not initialized\n");
	}
}

} /* namespace external */



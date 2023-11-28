#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

Model* Model::_instance = nullptr;

Model::Model() :
    modelListener(0),
    values(nullptr),
    maxSize(0),
    xScale(1.0f)
{
	if(_instance==nullptr){
		_instance = this;
	}

#if (TOUCHGFX_FREERTOS != 0)
	gui_msg_q = xQueueGenericCreate(5, sizeof(GUI_EVENT), 0);
#endif // TOUCHGFX_FREERTOS
#if (TOUCHGFX_BAREMETAL != 0)
    flagMode = flagDivTim = flagFreq = flagGraph = false;
#endif // TOUCHGFX_BAREMETAL
}

Model* Model::getInstance() {
	return _instance;
}

void Model::tick()
{
	//*******************************************************
	//
	//  HANDLE MESSAGES
	//
	//  Check for messages from backend, with zero timeout to
	//  avoid blocking the UI.
	//
	//*******************************************************
	//
#if (TOUCHGFX_FREERTOS != 0)
	GUI_EVENT msg;
	if (xQueueReceive(gui_msg_q, &msg, 0) == pdTRUE)
	  {
		if(msg==FREQUENCY_EVENT){
			modelListener->notifyFreqChanged(textFreq);
		}
		if(msg==MODE_EVENT){
			modelListener->notifyModeChanged(textMode);
		}
		if(msg==DIVTIM_EVENT){
			modelListener->notifyDivTimChanged(textDivTim);
		}
		if(msg==GRAPH_EVENT){
			modelListener->notifyDrawGraph(values, maxSize);
		}
	  }
#endif // TOUCHGFX_FREERTOS
#if (TOUCHGFX_BAREMETAL != 0)
	if(this->flagFreq){
		flagFreq=false;
		modelListener->notifyFreqChanged(textFreq);
	}
	if(this->flagMode){
		flagMode=false;
		modelListener->notifyModeChanged(textMode);
	}
	if(this->flagDivTim){
		flagDivTim=false;
		modelListener->notifyDivTimChanged(textDivTim);
		}
	if(this->flagGraph){
		flagGraph=false;
		modelListener->notifyDrawGraph(values, maxSize, xScale);
	}
#endif // TOUCHGFX_BAREMETAL
}

void Model::setGraphPoints(uint16_t * values, uint16_t maxSize, float xScale /* = 1 */){
	this->values=values;
	this->maxSize=maxSize;
	this->xScale = xScale;
#if (TOUCHGFX_BAREMETAL != 0)
	this->flagGraph=true;
#endif // TOUCHGFX_BAREMETAL
#if (TOUCHGFX_FREERTOS != 0)
	GUI_EVENT msg = GRAPH_EVENT;
	if(uxQueueMessagesWaiting(gui_msg_q)==0){
		xQueueSend(gui_msg_q, &msg, 0);
	}
#endif // TOUCHGFX_FREERTOS
	// Makes async call to modelListener->notifyDrawGraph(...) see Model::tick()
}

void Model::setDivTimLabel(const std::string str){
	textDivTim = str;
#if (TOUCHGFX_BAREMETAL != 0)
	this->flagDivTim=true;
#endif // TOUCHGFX_BAREMETAL
#if (TOUCHGFX_FREERTOS != 0)
	GUI_EVENT msg = DIVTIM_EVENT;
	xQueueSend(gui_msg_q, &msg, 0);
#endif // TOUCHGFX_FREERTOS
}

void Model::setModeSignal(const std::string str){
	textMode = str;
#if (TOUCHGFX_BAREMETAL != 0)
	this->flagMode=true;
#endif // TOUCHGFX_BAREMETAL
#if (TOUCHGFX_FREERTOS != 0)
	GUI_EVENT msg = MODE_EVENT;
	xQueueSend(gui_msg_q, &msg, 0);
#endif // TOUCHGFX_FREERTOS
}

void Model::setFreqSignal(const std::string str){
	textFreq = str;
#if (TOUCHGFX_BAREMETAL != 0)
	this->flagFreq=true;
#endif // TOUCHGFX_BAREMETAL
#if (TOUCHGFX_FREERTOS != 0)
	GUI_EVENT msg = FREQUENCY_EVENT;
	xQueueSend(gui_msg_q, &msg, 0);
#endif // TOUCHGFX_FREERTOS
}

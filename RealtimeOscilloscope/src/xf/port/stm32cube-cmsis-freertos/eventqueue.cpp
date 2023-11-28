#include <config/xf-config.h>

#if (USE_XF_PORT_STM32CUBE_CMSIS_FREERTOS_EVENT_QUEUE_IMPLEMENTATION != 0)

#include <cassert>
#include <croutine.h>
#include "queue.h"
#include "eventqueue.h"

XFEventQueuePort::XFEventQueuePort(const uint32_t queueSize /* = 8 */)
{
	popedEvent = nullptr;

	_queueId = osMessageQueueNew(queueSize, sizeof(XFEvent *), nullptr);
	assert(_queueId);
}

XFEventQueuePort::~XFEventQueuePort()
{
	osMessageQueueDelete(_queueId);
}

bool XFEventQueuePort::empty() const
{
	return (osMessageQueueGetCount(_queueId) == 0);
}

bool XFEventQueuePort::push(const XFEvent * pEvent, bool fromISR)
{
	if(fromISR){
		BaseType_t pxHigherPriorityTaskWoken = false;
		BaseType_t status = xQueueSendFromISR((QueueHandle_t)_queueId, &pEvent, &pxHigherPriorityTaskWoken);
		return (status == pdPASS);
	}
	else{
		const osStatus status = osMessageQueuePut(_queueId, &pEvent, 0, 100);
		return (status == osOK);
	}
}

XFEvent * XFEventQueuePort::front()
{
	assert(!empty());

	if(popedEvent==nullptr)
	{
		osStatus_t status = osMessageQueueGet(_queueId, &popedEvent, nullptr, 100);
		assert(status == osStatus_t::osOK);
	}
	return popedEvent;
}

void XFEventQueuePort::pop()
{
	if(popedEvent != nullptr)
	{
		popedEvent = nullptr;
	}
	else{
		XFEvent * pEvent;
		osMessageQueueGet(_queueId, &pEvent, nullptr, 0);
	}
}

bool XFEventQueuePort::pend()
{
	XFEvent * dummy;
	return xQueuePeek(reinterpret_cast<QueueHandle_t>(_queueId), &dummy,  0xFFFFFFFFL);
}

#endif // USE_XF_PORT_STM32CUBE_CMSIS_FREERTOS_EVENT_QUEUE_IMPLEMENTATION

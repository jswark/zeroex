#pragma once

#include "baseObject.h"

class EventsHandler : virtual public BaseObject
{
	virtual void update() = 0;
};

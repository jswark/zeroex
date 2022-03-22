#pragma once

#include "render/baseObject.h"

class GraphicObject : public virtual BaseObject {
	virtual void render() = 0;
};

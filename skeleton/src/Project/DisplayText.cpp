#include "DisplayText.h"
#include "../../RenderUtils.hpp"

void DisplayText::release() {
	DeregisterDisplayText(this);
	delete this;
}
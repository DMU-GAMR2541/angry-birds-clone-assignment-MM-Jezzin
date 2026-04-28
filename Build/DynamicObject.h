#pragma once
#include "GameObject.h"

class DynamicObject : public  virtual GameObject { // DynamicObject inherits from GameObject, and is a virtual base class to avoid ambiguity in multiple inheritance see StaticObject.h for explanation.


public:
	DynamicObject() = default; // Default constructor
	virtual ~DynamicObject() = default; // Default destructor
};

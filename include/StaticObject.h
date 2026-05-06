#pragma once
#include "GameObject.h"

class StaticObject : public virtual GameObject { // StaticObject inherits from GameObject, using virtual inheritance to avoid ambiguity in case of multiple inheritance. 
	//This is for the UI class which inherits from both StaticObject and DynamicObject, which both inherit from GameObject.
public:
	StaticObject() = default; // Default constructor
	virtual ~StaticObject() = default; // Default destructor
};
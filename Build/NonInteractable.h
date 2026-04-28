#pragma once
#include "StaticObject.h" // NonInteractable inherits from StaticObject


// Classes can inherit from multiple classes at a time.
class NonInteractable : public StaticObject { // NonInteractable inherits from StaticObject. 
public:
	NonInteractable() = default; // Default constructor
	virtual ~NonInteractable() = default; // Default destructor
};
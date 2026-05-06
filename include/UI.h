#pragma once
#include "DynamicObject.h"
#include "StaticObject.h"



class UI : public StaticObject, public DynamicObject { // UI inherits from both DynamicObject and StaticObject
	UI() = default; // Default constructor
	virtual ~UI() = default; // Default destructor
};
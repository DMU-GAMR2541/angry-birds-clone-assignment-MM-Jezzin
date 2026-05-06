#pragma once
#include "DynamicObject.h" 
#include <iostream> 



class Bird : public DynamicObject { // Bird inherits from DynamicObject
	
public:
	Bird() = default; // Default constructor
	virtual ~Bird() = default; // Default destructor

	void Update() override {
		// Implement bird-specific update logic here
		std::cout << "Bird Is Here";
	}
};
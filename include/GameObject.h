#pragma once
#pragma once

#include <iostream>
class GameObject { // Base class for all game objects
public:
	GameObject() = default; // Default constructor
	virtual ~GameObject() = default; // Virtual destructor to ensure proper cleanup of derived classes


	virtual void Update() {
		std::cout << "Game Object Update" << std::endl; // Placeholder for update logic, can be overridden by derived classes to implement specific behavior.
	}
};

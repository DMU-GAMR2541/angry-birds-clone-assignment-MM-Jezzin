#pragma once
#pragma once

class GameObject { // Base class for all game objects
public:
	GameObject() = default; // Default constructor
	virtual ~GameObject() = default; // Virtual destructor to ensure proper cleanup of derived classes
};

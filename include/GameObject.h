#pragma once
#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
class GameObject { // Base class for all game objects
public:
	GameObject() = default; // Default constructor
	virtual ~GameObject() = default; // Virtual destructor to ensure proper cleanup of derived classes

	virtual void render(sf::RenderWindow& window) = 0; // Pure virtual function for rendering, must be implemented by derived classes

	virtual void update() = 0; // Pure virtual function for updating, must be implemented by derived classes
};

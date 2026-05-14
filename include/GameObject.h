#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
class GameObject { // Base class for all game objects
public:
	virtual void render(sf::RenderWindow& window) = 0; // Pure virtual function for rendering, must be implemented by derived classes

	virtual void update() = 0; // Pure virtual function for updating, must be implemented by derived classes

	GameObject() = default; // Default constructor
	virtual void TakeDamage(float damage) = 0;
	virtual ~GameObject() = default;

	bool destroyed = false; // Indicates whether the object is destroyed or not
	float health = 0.0f;

};

#pragma once
#include "DynamicObject.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <box2d/box2d.h>

//Handles Physics and Rendering of the pigs in the game. Pigs are dynamic objects that can interact with other objects and can be affected by forces such as gravity, collisions, and user input. They are the main targets in the game and can be destroyed by the birds launched from the catapult.

class Pig : public virtual DynamicObject { // Pig inherits from DynamicObject
private:


public:
	Pig() = default; // Default constructor

	//Initializes a Pig object with the specified parameters.
	Pig(std::string PigLoc, sf::IntRect PigIntRect, b2Vec2 PigStartPos, b2World& World, float PigDensity, float PigFriction, float PigRestitution, float maxHealth);


	void TakeDamage(float damage); //Reduces the pig's health by the specified damage amount. If the pig's health drops to zero or below, it is considered destroyed and can be removed from the game.p
}
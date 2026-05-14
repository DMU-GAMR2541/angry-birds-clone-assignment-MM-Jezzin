#pragma once
#include "DynamicObject.h"
#include <SFML/Graphics.hpp>
#include <string>

class Structure : public DynamicObject { // Structure inherits from DynamicObject

public:
	Structure() = default; // Default constructor

	//Initializes a Structure object with the specified parameters.

	Structure(std::string texturePath, sf::IntRect spriteRect, b2Vec2 SctructurePos, b2World& world, float StructureDensity, float StructureFriction, float StructureRestitution, float angle);
	
	void update();

	void TakeDamage(float damage) override; //Reduces the structure's health by the specified damage amount. If the structure's health drops to zero or below, it is considered destroyed and can be removed from the game.

};
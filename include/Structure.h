#pragma once
#include "DynamicObject.h"

class Structure : public virtual DynamicObject { // Structure inherits from DynamicObject

public:
	Structure() = default; // Default constructor

	//Initializes a Structure object with the specified parameters.

	Structure(std::string texturePath, sf::IntRect spriteRect, b2Vec2 SctructurePos, b2World& world, float StructureDensity, float StructureFriction, float StructureRestitution);
};
#include "Structure.h"

Structure::Structure(std::string texturePath, sf::IntRect spriteRect, b2Vec2 SctructurePos, b2World& world, float StructureDensity, float StructureFriction, float StructureRestitution) : DynamicObject(texturePath, spriteRect, SctructurePos, world, StructureDensity, StructureFriction, StructureRestitution) {
	// Call the base class constructor to initialize the structure object with the provided parameters.
}
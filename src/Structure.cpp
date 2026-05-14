#include "Structure.h"

Structure::Structure(std::string texturePath, sf::IntRect spriteRect, b2Vec2 StructurePos, b2World& world, float StructureDensity, float StructureFriction, float StructureRestitution) : DynamicObject(texturePath, spriteRect, StructurePos, world, StructureDensity, StructureFriction, StructureRestitution, ShapeType::Box) {
	// Call the base class constructor to initialize the structure object with the provided parameters.
}
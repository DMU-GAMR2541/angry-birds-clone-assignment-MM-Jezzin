#include "Structure.h"

Structure::Structure(std::string texturePath, sf::IntRect spriteRect, b2Vec2 StructurePos, b2World& world, float StructureDensity, float StructureFriction, float StructureRestitution, float angle) : DynamicObject(texturePath, spriteRect, StructurePos, world, StructureDensity, StructureFriction, StructureRestitution, ShapeType::Box) {
	
	b2_body->SetTransform(b2_body->GetPosition(), angle);
	//adding angle to it can be rotated when created so theyre not just flat.
}
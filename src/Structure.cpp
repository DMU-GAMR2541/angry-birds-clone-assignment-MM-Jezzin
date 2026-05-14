#include "Structure.h"

Structure::Structure(std::string texturePath, sf::IntRect spriteRect, b2Vec2 StructurePos, b2World& world, float StructureDensity, float StructureFriction, float StructureRestitution, float angle) : DynamicObject(texturePath, spriteRect, StructurePos, world, StructureDensity, StructureFriction, StructureRestitution, ShapeType::Box) {
	
	b2_body->SetTransform(b2_body->GetPosition(), angle);


	
}

void Structure::update() {
    b2Vec2 vel = b2_body->GetLinearVelocity(); // Get the current linear velocity of the structure's body

    if (vel.Length() < 0.1f) { // Check if the structure's velocity is below a certain threshold to consider it at rest
	    b2_body->SetLinearVelocity(b2Vec2(0, 0)); // Set the structure's body type to static, making it immovable and not affected by physics forces
	    b2_body->SetAwake(false); // Set the structure's body to sleep.
    }
}

void Structure::TakeDamage(float damage) {
	// Need to add health 
	health -= damage; // Reduce the structure's health by the specified damage amount.
	std::cout << "Structure took damage: " << damage << std::endl;


		if (health <= 0) { // If the structure's health drops to 0
				std::cout << "Structure destroyed!" << std::endl;
		}

}
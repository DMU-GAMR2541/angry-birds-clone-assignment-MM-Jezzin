#include <iostream>
#include "Pig.h"


Pig::Pig(std::string PigLoc, sf::IntRect PigIntRect, b2Vec2 PigStartPos, b2World& World, float PigDensity, float PigFriction, float PigRestitution, float maxHealth) : DynamicObject(PigLoc, PigIntRect, PigStartPos, World, PigDensity, PigFriction, PigRestitution, ShapeType::Circle) {
	
	
	health = maxHealth; // Set the health of the pig to 100 when it is created.
	// Set the user data of the pig's body to point to the pig object itself.
	// This allows us to access the pig object when we detect collisions with it.

}


void Pig::TakeDamage(float damage) {
	health -= damage; // Reduce the pig's health by the specified damage amount.


	if (health <= 0) { // If the pig's health drops to zero or below, it is considered destroyed.
		// Here you can add code to remove the pig from the game, play a destruction animation, etc.
		std::cout << "Pig destroyed!" << std::endl;
	}
};
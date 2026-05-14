#include "ContactListener.h"
#include "Pig.h"
#include "Bird.h"
#include "Structure.h"

#include <cmath>


static void* GetUserPointer(b2Body* body) {
	return reinterpret_cast<void*>(body->GetUserData().pointer);
}

void ContactListener::BeginContact(b2Contact* contact) {

	//std::cout << "Collision detected!" << std::endl;
	b2Body* bodyA = contact->GetFixtureA()->GetBody();
	b2Body* bodyB = contact->GetFixtureB()->GetBody();
	// Get the user data pointers for both bodies
    
	auto* objA = reinterpret_cast<GameObject*>(bodyA->GetUserData().pointer);
	auto* objB = reinterpret_cast<GameObject*>(bodyB->GetUserData().pointer);

	//std::cout << "A ptr: " << objA << " B ptr: " << objB << "\n";

	if (!objA || !objB) return;

	if (objA || objB) {
		// If either object is null, we can't process the collision
		return;
	}

	b2Vec2 vA = bodyA->GetLinearVelocity();
	b2Vec2 vB = bodyB->GetLinearVelocity();
	float impact = (vA - vB).Length();

	if (impact < 0.5f) return; // Ignore minor collisions that don't cause significant damage

	float damage = impact * 5.0f; // Scale the damage based on the impact velocity

	std::cout << "Impact: " << impact << " Damage: " << damage << "\n";

	objA->TakeDamage(damage);
	objB->TakeDamage(damage);
}
#include "ContactListener.h"
#include "Pig.h"
#include "Bird.h"
#include "Structure.h"

#include <cmath>


static void* GetUserPointer(b2Body* body) {
	return reinterpret_cast<void*>(body->GetUserData().pointer);
}

void ContactListener::BeginContact(b2Contact* contact) {

	b2Body* bodyA = contact->GetFixtureA()->GetBody();
	b2Body* bodyB = contact->GetFixtureB()->GetBody();
	// Get the user data pointers for both bodies
    
	auto* objA = reinterpret_cast<GameObject*>(GetUserPointer(bodyA));
	auto* objB = reinterpret_cast<GameObject*>(GetUserPointer(bodyB));

	if (!objA || !objB) return;

	b2Vec2 vA = bodyA->GetLinearVelocity();
	b2Vec2 vB = bodyB->GetLinearVelocity();
	float impact = (vA - vB).Length();

	float damage = impact * 5.0f; // Scale the damage based on the impact velocity

	objA->TakeDamage(damage);
	objB->TakeDamage(damage);
}
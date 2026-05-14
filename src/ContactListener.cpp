#include "ContactListener.h"
#include "Pig.h"
#include "Bird.h"
#include "Structure.h"
#include <iostream>
#include <cmath>


static GameObject* GetObject(b2Body* body) { 
	// Helper function to get the GameObject pointer from a b2Body's user data

	
	return reinterpret_cast<GameObject*>(body->GetUserData().pointer);
}

void ContactListener::BeginContact(b2Contact* contact)
{
    auto* objA = reinterpret_cast<GameObject*>(
        contact->GetFixtureA()->GetBody()->GetUserData().pointer);

    auto* objB = reinterpret_cast<GameObject*>(
        contact->GetFixtureB()->GetBody()->GetUserData().pointer);

    if (!objA || !objB) return;

    float damage = 1.0f; 

    objA->TakeDamage(damage);
    objB->TakeDamage(damage);
}
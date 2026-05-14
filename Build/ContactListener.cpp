#include "ContactListener.h"
#include "pig.h"
#include "Bird.h"

#include <iostream>

#include "Structure.h"


void ContacctListener::BeginContact(b2Contact* contact) {
	// Get the two fixtures that are in contact
	void* aData = contact->GetFixtureA()->GetBody()->GetUserData().pointer;
	void* bData = contact->GetFixtureB()->GetBody()->GetUserData().pointer;
	//A and B are 2 objects that collide
	//each fixture belongs to a body, and each body has user data that points to the game object it represents
	//Data stores pointer for each object.
	
}
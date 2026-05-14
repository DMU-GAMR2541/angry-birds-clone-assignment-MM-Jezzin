#pragma once
#include <box2d/box2d.h>

class ContactListener : public b2ContactListener {
public:
	void BeginContact(b2Contact* contact) override;
		// This function is called when two fixtures begin to touch.
	
};
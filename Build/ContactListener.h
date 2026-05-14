#pragma once
#include <box2d/box2d.h>

class ContacctListener : public b2ContactListener {

public:
	// Called when two fixtures begin to touch
	void BeginContact(b2Contact* contact) override;

};

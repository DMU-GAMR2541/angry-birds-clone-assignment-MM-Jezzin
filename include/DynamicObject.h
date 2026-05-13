#pragma once
#include "GameObject.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <box2d/box2d.h>



class DynamicObject : public virtual GameObject {

private:
	sf::Sprite DynSprite; //Adds sprite
	sf::Texture DynTexture; //Adds texture
	sf::RenderWindow* DynWindow; 
	std::string DynTextureLoc;


	//Physics for the sprites
	b2Vec2 b2_pos;
	b2BodyDef b2_BodyDef;
	b2FixtureDef b2_fixtureDef;
	b2CircleShape b2_dynamicCircle;
	b2Body* b2_body;


public:
	DynamicObject() = default; // Default constructor

	virtual ~DynamicObject() = default; // Virtual destructor to ensure proper cleanup of derived classes
	//Initializes a DynamicObject with the specified parameters.
	DynamicObject(std::string DynConstructor, sf::IntRect DynIntRect, b2Vec2 DynStartPos, b2World& world, float Density, float Friction, float Restitution);


	void render(sf::RenderWindow& GObjRenderWindow) override;
	void update() override;
	void UpdateSprite();


	void impulse(b2Vec2 b2_impulse, bool awake); //Applies a linear impulse to the center of the physics body, allowing for movement and interaction with other objects in the physics simulation. The 'awake' parameter determines whether the body should be awakened if it is currently sleeping, which can affect how the physics engine processes the impulse and updates the body's state accordingly.
	void setGravityScale(float scale); //Sets the gravity scale for the physics body, allowing for adjustments to how gravity affects the object in the physics simulation. A higher scale will make the object more affected by gravity, while a lower scale will reduce its influence.


	bool fired = false; //A boolean variable that indicates whether the bird has been fired or launched in the game. This variable can be used to track the state of the object and determine if it has been activated or is still in its initial state, which can affect how it interacts with other objects and the physics simulation in the game.
};
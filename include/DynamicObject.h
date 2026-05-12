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





};
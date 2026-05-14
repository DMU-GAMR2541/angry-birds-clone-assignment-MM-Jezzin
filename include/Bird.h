#pragma once
#include "DynamicObject.h" 
#include <iostream> 
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

class Bird : public DynamicObject { // Bird inherits from DynamicObject
	
public:
    Bird(std::string BirdLoc,
        sf::IntRect BirdIntRect,b2Vec2 BirdStartPos,b2World& World,float BirdDensity,float BirdFriction, float BirdRestitution);

    void load();
    void fire(b2Vec2 impulse);


    b2Body* getBody();

    void TakeDamage(float damage) override;

    bool loaded = true;
    bool fired = false;
   

};
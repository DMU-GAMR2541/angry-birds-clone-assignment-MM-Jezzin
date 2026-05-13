#pragma once
#include "DynamicObject.h" 
#include <iostream> 
#include <SFML/Graphics.hpp>


class Bird : public DynamicObject { // Bird inherits from DynamicObject
	
public:
    Bird(std::string BirdLoc,
        sf::IntRect BirdIntRect,b2Vec2 BirdStartPos,b2World& World,float BirdDensity,float BirdFriction, float BirdRestitution);

    void load();
    void fire(b2Vec2 impulse);

    bool loaded = true;
    bool fired = false;
    bool isMoving();


};
#pragma once
#include "DynamicObject.h" 
#include <iostream> 
#include <SFML/Graphics.hpp>


class Bird : public DynamicObject { // Bird inherits from DynamicObject
	
public:
	Bird() = default; // Default constructor

	
	//Initializes a Bird object with the specified parameters.
	Bird(std::string BirdLoc, sf::IntRect BirdIntRect, b2Vec2 BirdStartPos, b2World& World, float BirdDensity, float BirdFriction, float BirdRestitution) : DynamicObject(BirdLoc, BirdIntRect, BirdStartPos, World, BirdDensity, BirdFriction, BirdRestitution) {};
};
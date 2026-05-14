#include "DynamicObject.h" 
#include <iostream> 
#include <SFML/Graphics.hpp>
#include "Bird.h"




Bird::Bird(std::string BirdLoc, sf::IntRect BirdIntRect, b2Vec2 BirdStartPos, b2World& World, float BirdDensity, float BirdFriction, float BirdRestitution) : DynamicObject(BirdLoc, BirdIntRect, BirdStartPos, World, BirdDensity, BirdFriction, BirdRestitution, ShapeType::Circle) {
    load(); // When a Bird object is created, it is frozen. This is now here so the pigs arent frozen too.


}
void Bird::load()
{
    b2_body->SetGravityScale(0.0f);
    b2_body->SetLinearVelocity({ 0,0 });
    b2_body->SetAngularVelocity(0  );
    b2_body->SetAwake(false);
    loaded = true;
    fired = false;
    
}

void Bird::fire(b2Vec2 impulse)
{
    b2_body->SetGravityScale(1.0f);
    b2_body->SetAwake(true);
    b2_body->ApplyLinearImpulseToCenter(impulse, true);

    fired = true;
    loaded = false;
}

void Bird::TakeDamage(float damage) {
    health -= damage;

	if (health <= 0) {
		std::cout << "Bird destroyed!" << std::endl;
		destroyed = true;
	}
	std::cout << "Bird took damage: " << damage << std::endl;
};

b2Body* Bird::getBody() {
    return b2_body;
};
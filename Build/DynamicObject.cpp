#include "DynamicObject.h"

const float SCALE = 30.f;
DynamicObject::DynamicObject(std::string DynConstructor, sf::IntRect DynIntRect, b2Vec2 DynStartPos, b2World& world, float Density, float Friction, float Restitution) 
{
		DynTextureLoc = DynConstructor;

		if (!DynTexture.loadFromFile(DynTextureLoc)) { // Load the texture from the specified file
			std::cout << "Error loading texture: " << std::endl;// Print an error message if the texture fails to load
		}

		DynSprite.setTexture(DynTexture);// Set the loaded texture to the sprite
		DynSprite.setTextureRect(DynIntRect); // Set the texture rectangle for the sprite, which defines the portion of the texture to be used for rendering
		//DynSprite.setPosition(200.f, 200.f);

		DynSprite.setOrigin(DynIntRect.width / 2.f, DynIntRect.height / 2.f); // Set the origin of the sprite to its center, which allows for proper rotation around the middle.
		
		
		
		b2_dynamicCircle.m_radius = 0.5; // Set the radius of the dynamic circle shape for physics simulation

		b2_BodyDef.type = b2_dynamicBody; // Set the body type to dynamic, which means it will be affected by forces and collisions in the physics simulation
		b2_BodyDef.position = DynStartPos;
		b2_body = world.CreateBody(&b2_BodyDef); // Create the body in the Box2D world using the defined body definition

		b2_fixtureDef.shape = &b2_dynamicCircle;
		b2_fixtureDef.density = 1.0f;
		b2_fixtureDef.friction = 0.3f;
		b2_fixtureDef.restitution = 0.5f;

		b2_body->CreateFixture(&b2_fixtureDef); // Create a fixture for the body using the defined fixture definition, which includes the shape, density, friction, and restitution properties
}

void DynamicObject::render(sf::RenderWindow & GObjRenderWindow) {
	GObjRenderWindow.draw(DynSprite); // Draw the sprite on the provided render window
}

void DynamicObject::update() {
}

void DynamicObject::UpdateSprite() {
	DynSprite.setPosition(sf::Vector2f(b2_body->GetPosition().x * SCALE, b2_body->GetPosition().y * SCALE)); // Update the sprite's position based on the physics body's position, scaling it to match the visual representation
}
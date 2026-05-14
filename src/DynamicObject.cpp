#include "DynamicObject.h"

const float SCALE = 30.f;
DynamicObject::DynamicObject(std::string texturePath, sf::IntRect spriteRect, b2Vec2 startPos, b2World& world, float Density, float Friction, float Restitution, ShapeType shapeType, float rotation)
{



	DynTextureLoc = texturePath;

	if (!DynTexture.loadFromFile(DynTextureLoc)) { // Load the texture from the specified file
		std::cout << "Error loading texture: " << std::endl;// Print an error message if the texture fails to load
	}

	DynSprite.setTexture(DynTexture);// Set the loaded texture to the sprite
	DynSprite.setTextureRect(spriteRect); // Set the texture rectangle for the sprite, which defines the portion of the texture to be used for rendering

	DynSprite.setOrigin(spriteRect.width / 2.f, spriteRect.height / 2.f); // Set the origin of the sprite to its center, which allows for proper rotation around the middle.


	b2_BodyDef.type = b2_dynamicBody; // Set the body type to dynamic, which means it will be affected by forces and collisions in the physics simulation
	b2_BodyDef.position = startPos; // Set the initial position of the body in the physics world using the provided starting position
	b2_body = world.CreateBody(&b2_BodyDef); // Create the body in the Box2D world using the defined body definition

	//Shape Switch between pig, bird and structure

	if (shapeType == ShapeType::Circle) {

		float Size = (spriteRect.width + spriteRect.height) / 4.0f;
		b2_dynamicCircle.m_radius = Size / SCALE; // Set the radius of the dynamic circle shape based on the average of the sprite's width and height, scaled to match the physics simulation

		b2_fixtureDef.shape = &b2_dynamicCircle; // Set the shape of the fixture to the defined dynamic circle, which will be used for collision detection and response in the physics simulation
	}
	else 
	{
		b2PolygonShape boxShape;

		boxShape.SetAsBox((spriteRect.width / 2.f) / SCALE, (spriteRect.height / 2.f) / SCALE); 

		static b2PolygonShape safeBox;
		safeBox = boxShape;

		// Set the shape of the fixture to the defined box shape, which will be used for collision detection and response in the physics simulation
		b2_fixtureDef.shape = &safeBox;
	}

		b2_fixtureDef.density = Density;
		b2_fixtureDef.friction = Friction;
		b2_fixtureDef.restitution = Restitution;

		b2_body->CreateFixture(&b2_fixtureDef); // Create a fixture for the body using the defined fixture definition, which includes the shape, density, friction, and restitution properties



}


void DynamicObject::render(sf::RenderWindow & GObjRenderWindow) {
	GObjRenderWindow.draw(DynSprite); // Draw the sprite on the provided render window
}

void DynamicObject::update() {
}

void DynamicObject::UpdateSprite() {
	DynSprite.setPosition(sf::Vector2f(b2_body->GetPosition().x * SCALE, b2_body->GetPosition().y * SCALE)); // Update the sprite's position based on the physics body's position, scaling it to match the visual representation

	//added rotation to the sprite based on the physics body's angle.
	DynSprite.setRotation(b2_body->GetAngle() * 180.0f / b2_pi); // Update the sprite's rotation based on the physics body's angle, converting it from radians to degrees for proper rendering

}


//Applies a linear impulse to the center of the physics body, allowing for movement and interaction with other objects in the physics simulation. The 'awake' parameter determines whether the body should be awakened if it is currently sleeping, which can affect how the physics engine processes the impulse and updates the body's state accordingly.
void DynamicObject::impulse(b2Vec2 b2_impulse, bool awake) //
{
	b2_body->ApplyLinearImpulseToCenter(b2_impulse, awake); //Adds impulse to an object
};

void DynamicObject::setGravityScale(float scale) {
	b2_body->SetGravityScale(scale); //Sets the gravity scale for the physics body, allowing for adjustments to how gravity affects the object in the physics simulation. A higher scale will make the object more affected by gravity, while a lower scale will reduce its influence.
}
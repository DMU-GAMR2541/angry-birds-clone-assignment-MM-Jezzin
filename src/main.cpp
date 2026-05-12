#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <iostream>
#include "Bird.h"
#include "Pig.h"

#include <filesystem>
#include <list> // Include the list header for using std::list - the lists for the different birds and pigs :)
#include <vector>

int main() {

    std::cout << std::filesystem::current_path() << std::endl;
    // --- 1. WINDOW SETUP ---
	sf::RenderWindow window(sf::VideoMode(800, 600), "Annoyed_Flocks"); // Create a window with the title "Annoyed_Flocks" and dimensions 800x600 pixels
    window.setFramerateLimit(60);

    //Box2D works in meters. SFML works in pixels.
    const float SCALE = 30.0f;

    //Can set a definition for PI.
    const float PI = 3.1415927;

    //setup world.
    b2Vec2 b2_gravity(0.0f, 9.8f); // Earth-like gravity
    b2World world(b2_gravity);

    //Setup ground for the circle to move / bounce on.
    //Needs to have a body definition and a body. We use a raw pointer for the b2Body as Box2d does the management itself.
    //A body can be defined as having a position, velocity, and mass. 
    b2BodyDef b2_groundBodyDef;
    b2_groundBodyDef.position.Set(400.0f / SCALE, 590.0f / SCALE);
    b2Body* b2_groundBody = world.CreateBody(&b2_groundBodyDef);

    //Define a fixture shape that relates to the collision for the ground.
    b2PolygonShape b2_groundBox;
    b2_groundBox.SetAsBox(400.0f / SCALE, 10.0f / SCALE);
    b2_groundBody->CreateFixture(&b2_groundBox, 0.0f);

    //Set up the ground visualisation.
    sf::RectangleShape sf_groundVisual(sf::Vector2f(800.0f, 20.0f));
    sf_groundVisual.setOrigin(400.0f, 10.0f);
    sf_groundVisual.setFillColor(sf::Color(34, 139, 34)); // Forest Green

    //Setting up a wall for the ball to hit.
    b2BodyDef b2_wallDef;
    b2_wallDef.position.Set(750.0f / SCALE, 500.0f / SCALE);
    b2Body* b2_wallBody = world.CreateBody(&b2_wallDef);


    b2PolygonShape b2_wallBox;
    b2_wallBox.SetAsBox(10.0f / SCALE, 80.0f / SCALE);
    b2_wallBody->CreateFixture(&b2_wallBox, 0.0f);

    sf::RectangleShape sf_wallVisual(sf::Vector2f(20.0f, 160.0f));
    sf_wallVisual.setOrigin(10.0f, 80.0f);
    sf_wallVisual.setFillColor(sf::Color::Red);

    //Rather than having an immovable wall, we can use the dynamic body type to create one that can have velocity etc.
    b2BodyDef b2_plankDef;

    b2_plankDef.type = b2_dynamicBody;
    b2_plankDef.position.Set(550.0f / SCALE, 450.0f / SCALE);
    b2Body* b2_plankBody = world.CreateBody(&b2_plankDef);

    b2PolygonShape b2_plankBox;
    b2_plankBox.SetAsBox(10.0f / SCALE, 60.0f / SCALE);

    b2FixtureDef b2_plankFixture;
    b2_plankFixture.shape = &b2_plankBox;
    b2_plankFixture.density = 1.5f;   // Light wood
    b2_plankFixture.friction = 0.3f;
    b2_plankBody->CreateFixture(&b2_plankFixture);

    sf::RectangleShape sf_plankVisual(sf::Vector2f(20.0f, 120.0f));
    sf_plankVisual.setOrigin(10.0f, 60.0f);
    sf_plankVisual.setFillColor(sf::Color(139, 69, 19)); // Brown

    //Create a ball that is fired when space is pressed. We need to first have a dynamic ball to do it.
    b2BodyDef b2_ballDef;
    b2_ballDef.type = b2_dynamicBody;
    b2_ballDef.position.Set(100.0f / SCALE, 500.0f / SCALE);
    b2Body* b2_ballBody = world.CreateBody(&b2_ballDef);

    b2CircleShape b2_circleShape;
    b2_circleShape.m_radius = 15.0f / SCALE;

    b2FixtureDef b2_ballFixture;
    b2_ballFixture.shape = &b2_circleShape;
    b2_ballFixture.density = 1.0f;
    b2_ballFixture.restitution = 0.5f; // Bounciness
    b2_ballBody->CreateFixture(&b2_ballFixture);

    sf::CircleShape sf_ballVisual(15.0f);
    sf_ballVisual.setOrigin(15.0f, 15.0f);
    sf_ballVisual.setFillColor(sf::Color::Yellow);




  

    //Adding physics to the sprites

    b2Vec2 b2_pos;
	b2BodyDef b2_BodyDef;
	b2FixtureDef b2_fixtureDef;
	b2Body* b2_body;

	//b2CircleShape b2_dynamicCircle;
    
	//Creates a bird
	//Bird bird("../assets/Ang_Birds/Adapted_Birds.png", sf::IntRect(940, 196,80, 80), b2Vec2(250.0f / SCALE, 200.0f / SCALE), world, 1.0f, 3.0f, 0.5f); // Create a Bird instance with texture, sprite rectangle, and position
	//list of birds and pigs
	std::vector<std::unique_ptr<Bird>> Birds;
    
	std::vector<sf::IntRect>birdsprites = { sf::IntRect(903, 798, 47, 47),sf::IntRect(300, 752, 100, 95),sf::IntRect(0, 378, 40, 32) 
	}; //These are the position and size of the different birds in the sprite sheet. We can use these to create multiple birds with different appearances. The sf::IntRect constructor takes four parameters: the x and y coordinates of the top-left corner of the rectangle, and the width and height of the rectangle. These rectangles define the portion of the sprite sheet that will be used for each bird's texture.

	int xOffset = 100; // This variable is used to increment the x position of each bird when creating multiple birds. It starts at 100 and is increased by 100 for each subsequent bird, ensuring that the birds are spaced apart horizontally when they are created.

	for (const auto& spriteRect : birdsprites) {
		Birds.push_back(std::make_unique<Bird>("../assets/Ang_Birds/Angry_Birds.png", spriteRect, b2Vec2((250.0f + xOffset) / SCALE, 200.0f / SCALE), world, 1.0f, 3.0f, 0.5f)); // Create a Bird instance with texture, sprite rectangle, and position
            xOffset += 100; // Increment the xOffset for the next bird's position
	}


    //Creates a pig
    //Pig PigEnemy("../assets/Ang_Birds/sprite_1.png", sf::IntRect(0, 0, 60, 52), b2Vec2(250.0f / SCALE, 200.0f / SCALE), world, 1.0f, 3.0f, 0.5f); // Create a Pig instance with texture, sprite rectangle, and position
	//list of pigs
	std::vector<std::unique_ptr<Pig>> Pigs;

    
	std::vector<sf::IntRect>pigsprites = { sf::IntRect(928, 447, 80, 80),sf::IntRect(733, 155, 110, 100),sf::IntRect(732, 856, 60, 45) };

	xOffset = 0; // Reset xOffset for pigs

	for (const auto& spriteRect : pigsprites) {
		Pigs.push_back(std::make_unique<Pig>("../assets/Ang_Birds/Angry_Birds.png", spriteRect, b2Vec2((500.0f + xOffset) / SCALE, 200.0f / SCALE), world, 1.0f, 3.0f, 0.5f)); // Create a Pig instance with texture, sprite rectangle, and position
		xOffset += 100; // Increment the xOffset for the next pig's position

	}
    // --- 7. MAIN LOOP ---
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

			// INPUT HANDLING: Press SPACE to launchok the ball. We can apply an impulse to the ball to make it move. The impulse is a sudden force applied to an object, which changes its velocity. In Box2D, we can use the ApplyLinearImpulse function to apply an impulse to a body. The impulse is defined as a vector that specifies the direction and magnitude of the force. In this case, we want to apply an impulse in the positive X direction (to the right) and negative Y direction (upwards) to launch the ball towards the targets.
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    // Reset position of the ball so that it can be fired again from its original poisition.
                    b2_ballBody->SetTransform(b2Vec2(100.0f / SCALE, 500.0f / SCALE), 0);
                    b2_ballBody->SetLinearVelocity(b2Vec2(0, 0));
                    b2_ballBody->SetAngularVelocity(0);

                    // Apply impulse (X-axis, Y-axis) Negative Y is UP in Box2D because gravity is positive.
                    b2_ballBody->ApplyLinearImpulse(b2Vec2(5.0f, -5.0f), b2_ballBody->GetWorldCenter(), true);

                    std::cout << "Firing!!!!" << std::endl;
                }
            }
        }

        // Update Physics
        world.Step(1.0f / 60.0f, 8, 3);

		for (auto& pig : Pigs) { // Loop through each Pig in the list and update it
			pig->update(); // Update the Pig instance (if needed)
			pig->UpdateSprite();
		}
		for (auto& bird : Birds) { // Loop through each Bird in the list and update it
           bird->update(); // Update the Bird instance (if needed)
           bird->UpdateSprite();
        }

		//bird.update(); // Update the Bird instance (if needed)
		//bird.UpdateSprite(); // Update the Bird's sprite position based on its physics body

        //All of the visuals needs to be synced with the physics.

        sf_ballVisual.setPosition(b2_ballBody->GetPosition().x * SCALE, b2_ballBody->GetPosition().y * SCALE);
        sf_ballVisual.setRotation(b2_ballBody->GetAngle() * (180.0f / PI));

        //Static objects usually don't move, but we set the position once.
        sf_groundVisual.setPosition(b2_groundBody->GetPosition().x * SCALE, b2_groundBody->GetPosition().y * SCALE);
        sf_wallVisual.setPosition(b2_wallBody->GetPosition().x * SCALE, b2_wallBody->GetPosition().y * SCALE);

        // Dynamic wall.
        sf_plankVisual.setPosition(b2_plankBody->GetPosition().x * SCALE, b2_plankBody->GetPosition().y * SCALE);
        sf_plankVisual.setRotation(b2_plankBody->GetAngle() * (180.0f / PI));

        //Render all of the content at each frame. Remember you need to clear the screen each iteration or artefacts remain.
        window.clear(sf::Color(135, 206, 235)); // Sky Blue

        window.draw(sf_groundVisual);
        window.draw(sf_wallVisual);
        window.draw(sf_plankVisual);
        window.draw(sf_ballVisual);
		
		for (auto& pig : Pigs) { // Loop through each Pig in the list and render it
			pig->render(window); // Render the Pig instance
		}

		//bird.render(window); // Render the Bird instance
		for (auto& bird : Birds) { // Loop through each Bird in the list and render it
			bird->render(window); // Render the Bird instance
		}
        window.display();
    }

    return 0;
}
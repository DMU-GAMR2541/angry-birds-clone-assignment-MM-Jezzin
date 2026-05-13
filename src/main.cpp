#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <iostream>
#include "Bird.h"
#include "Pig.h"

#include <filesystem>
#include <list> // Include the list header for using std::list - the lists for the different birds and pigs :)
#include <vector>
#include <algorithm> // Include the algorithm header for using std::clamp in the catapult dragging system   
#include <cmath> // Include the cmath header for using mathematical functions like std::sqrt in the catapult dragging system


int main() {

    std::cout << std::filesystem::current_path() << std::endl;
    // --- 1. WINDOW SETUP ---
	sf::RenderWindow window(sf::VideoMode(1200, 600), "Annoyed_Flocks"); // Create a window with the title "Annoyed_Flocks" and dimensions 800x600 pixels
    window.setFramerateLimit(60);

    //Box2D works in meters. SFML works in pixels.
    const float SCALE = 30.0f;

    //Can set a definition for PI.
    const float PI = 3.1415927;

    //Trying to make catapult work :)
    sf::Vector2f slingpos(150.0f, 500.0f); // Position of the sling, where the birs is going to sit before it gets catapulted.
    float slingRadiusY = 50.0f; // Radius of the sling, which determines how far the bird can be pulled back before being launched. This is used to limit the distance the bird can be dragged from the sling position, ensuring that the launch force is proportional to how far the bird is pulled back.
    float slingRadiusX = 50.0f; // Radius of the sling in the X direction, which determines how far the bird can be pulled back horizontally before being launched. This is used to limit the distance the bird can be dragged from the sling position in the horizontal direction, ensuring that the launch force is proportional to how far the bird is pulled back horizontally.
    float launchForceMultiplier = 15.0f; // Multiplier for the launch force, which determines how much force is applied to the bird when it is launched. This is used to adjust the strength of the launch, allowing for fine-tuning of the gameplay experience. A higher multiplier will result in a stronger launch, while a lower multiplier will result in a weaker launch.
	int currentBird = 0; // Index of the current bird being launched, which is used to keep track of which bird is currently active and being controlled by the player. This allows for managing multiple birds in the game and ensuring that the correct bird is launched when the player interacts with the sling.
	bool Dragging = false; // Flag to indicate whether the player is currently dragging the bird, which is used to manage the state of the sling and the bird's position. When this flag is true, it indicates that the player is actively dragging the bird, allowing for real-time updates to the bird's position and launch force based on the player's input.
	bool launched = false; // Flag to indicate whether the bird has been launched, which is used to manage the state of the game and ensure that the bird is only launched once per interaction with the sling. When this flag is true, it indicates that the bird has been launched, preventing further dragging or launching until the next bird is selected.
	float waitingTimer = 0.0f; // Timer to track the waiting time after a bird is launched, which can be used to implement a delay before allowing the next bird to be launched.
	float waitingTimeThreshold = 5.0f; // Threshold for the waiting time, which determines how long the player must wait after launching a bird before being able to launch the next one.
	sf::Clock birdTimer; // Clock to track the time since the last bird was launched.


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
    b2_groundBox.SetAsBox(900.0f / SCALE, 10.0f / SCALE);
    b2_groundBody->CreateFixture(&b2_groundBox, 0.0f);

    //Adding a wall that stops birds flying off the screen on the right when they are catapulted.
	b2BodyDef b2_rightWallDef;
	b2_rightWallDef.position.Set(1200.0f / SCALE, 300.0f / SCALE);
	b2Body* b2_rightWallBody = world.CreateBody(&b2_rightWallDef);

	b2PolygonShape b2_rightWallBox;
	b2_rightWallBox.SetAsBox(10.0f / SCALE, 300.0f / SCALE);
	b2_rightWallBody->CreateFixture(&b2_rightWallBox, 0.0f);

    //Adding visualisation for the right wall.
	sf::RectangleShape sf_rightWallVisual(sf::Vector2f(20.0f, 600.0f));
	sf_rightWallVisual.setOrigin(10.0f, 300.0f);
	sf_rightWallVisual.setFillColor(sf::Color(34,139,34));

    

    //Set up the ground visualisation.
    sf::RectangleShape sf_groundVisual(sf::Vector2f(1200.0f, 20.0f));
    sf_groundVisual.setOrigin(400.0f, 10.0f);
    sf_groundVisual.setFillColor(sf::Color(34, 139, 34)); // Forest Green




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

    //Another block. (IDK what to call it yet)
    b2BodyDef b2_WallDef2;
	b2_WallDef2.type = b2_dynamicBody;
    b2_WallDef2.position.Set(800.0f / SCALE, 500.0f / SCALE);
    b2Body* b2_WallBody2 = world.CreateBody(&b2_WallDef2);

	b2PolygonShape b2_WallBox2;
	b2_WallBox2.SetAsBox(10.0f / SCALE, 60.0f / SCALE);

	b2FixtureDef b2_WallFixture2;
	b2_WallFixture2.shape = &b2_WallBox2;
	b2_WallFixture2.density = 1.0f;   // Light wood
	b2_WallFixture2.friction = 0.3f;
	b2_WallBody2->CreateFixture(&b2_WallFixture2);

    //Vizualisation of block 2.
    sf::RectangleShape sf_WallVisual2(sf::Vector2f(20.0f, 120.0f));
    sf_WallVisual2.setOrigin(10.0f, 60.0f);
    sf_WallVisual2.setFillColor(sf::Color(139, 69, 19)); // Brown



    //Another block. (IDK what to call it yet)
    b2BodyDef b2_WallDef3;
    b2_WallDef3.type = b2_dynamicBody;
    b2_WallDef3.position.Set(10.0f / SCALE, 475.0f / SCALE);
    b2Body* b2_WallBody3 = world.CreateBody(&b2_WallDef3);

    b2PolygonShape b2_WallBox3;
    b2_WallBox3.SetAsBox(60.0f / SCALE, 10.0f / SCALE);

    b2FixtureDef b2_WallFixture3;
    b2_WallFixture3.shape = &b2_WallBox3;
    b2_WallFixture3.density = 1.0f;   // Light wood
    b2_WallFixture3.friction = 0.3f;
    b2_WallBody3->CreateFixture(&b2_WallFixture3);

    //Vizualisation of block 3
    sf::RectangleShape sf_WallVisual3(sf::Vector2f(120.0f, 20.0f));
    sf_WallVisual3.setOrigin(60.0f, 10.0f);
    sf_WallVisual3.setFillColor(sf::Color(139, 69, 19)); // Brown





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

	int xOffset = -100; // This variable is used to increment the x position of each bird when creating multiple birds. It starts at 100 and is increased by 100 for each subsequent bird, ensuring that the birds are spaced apart horizontally when they are created.

	for (const auto& spriteRect : birdsprites) { // Loop through each sf::IntRect in the birdsprites vector and create a Bird instance for each one, using the corresponding sprite rectangle and position. The position of each bird is determined by the xOffset variable, which is incremented for each bird to ensure they are spaced apart horizontally.
		Birds.push_back(std::make_unique<Bird>("../assets/Ang_Birds/Angry_Birds.png", spriteRect, b2Vec2((100.0f - xOffset) / SCALE, 500.0f / SCALE), world, 1.0f, 3.0f, 0.5f)); // Create a Bird instance with texture, sprite rectangle, and position
            xOffset += 60; // Increment the xOffset for the next bird's position
	}
	// Set the initial position of the first bird in the list to be on the sling. The position is calculated based on the sling's position and the defined radius, ensuring that the bird starts at the correct location for launching.
	Birds[0]->getBody()->SetTransform(b2Vec2(slingpos.x / SCALE, slingpos.y / SCALE), 0);// Set the initial position of the first bird in the list to be on the sling


	// Set the first bird's body type to kinematic, which allows it to be moved directly without being affected by forces or collisions, making it suitable for being positioned on the sling before launch. This ensures that the bird can be dragged and positioned on the sling without being influenced by gravity or other physics interactions until it is launched.
	Birds[0]->getBody()->SetType(b2_kinematicBody); // Set the first bird's body type to kinematic, which allows it to be moved directly without being affected by forces or collisions, making it suitable for being positioned on the sling before launch.
    
    
    //list of pigs
	std::vector<std::unique_ptr<Pig>> Pigs;

	//These are the position and size of the different pigs in the sprite sheet. We can use these to create multiple pigs with different appearances. The sf::IntRect constructor takes four parameters: the x and y coordinates of the top-left corner of the rectangle, and the width and height of the rectangle. These rectangles define the portion of the sprite sheet that will be used for each pig's texture.
	std::vector<sf::IntRect>pigsprites = { sf::IntRect(928, 447, 80, 80),sf::IntRect(733, 155, 110, 100),sf::IntRect(732, 856, 60, 45) };

	xOffset = 0; // Reset xOffset for pigs

	for (const auto& spriteRect : pigsprites) {
		Pigs.push_back(std::make_unique<Pig>("../assets/Ang_Birds/Angry_Birds.png", spriteRect, b2Vec2((500.0f + xOffset) / SCALE, 200.0f / SCALE), world, 1.0f, 3.0f, 0.5f)); // Create a Pig instance with texture, sprite rectangle, and position
		xOffset += 100; // Increment the xOffset for the next pig's position

	}

  
    int activeBird = 0; //Index of the current active bird in sling
    
    // --- 7. MAIN LOOP ---
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            //New INPUT HANDLING for catapult using mouse button.

			if (event.type == sf::Event::MouseButtonPressed) {
				if (event.mouseButton.button == sf::Mouse::Left) {
					// Check if the mouse click is within the sling area
					if (currentBird < static_cast<int>(Birds.size()) && !launched) { // Check if there are any birds left to launch
                        Dragging = true; // Set dragging flag to true when the left mouse button is pressed
					}
				}
			}


            if (event.type  == sf::Event::MouseButtonReleased) { //On the event that the mouse button is released
                if (event.mouseButton.button == sf::Mouse::Left) { //If it was the left mouse button that was released
                    if (Dragging) { //If the user is dragging the bird.

                        Bird* activeBird = Birds[currentBird].get(); // Get a pointer to the current active bird using the currentBird index
                        b2Body* body = activeBird->getBody(); // Get the Box2D body of the active bird

                        sf::Vector2f birdPos(body->GetPosition().x * SCALE, body->GetPosition().y * SCALE); // Get the current position of the bird in pixels by multiplying the Box2D position by the SCALE factor
                        sf::Vector2f slingVec = slingpos - birdPos;
                        // Calculate the vector from the sling position to the bird's position,
                        // which represents the direction and distance the bird has been dragged from the sling
                        //This is to work out how far and the strength the bird gets flung.

                        body->SetType(b2_dynamicBody);
						body->SetGravityScale(1.0f); //Adding gravity so they dont float.
                        // Set the bird's body type to dynamic, when it is launched.


                        body->ApplyLinearImpulseToCenter(b2Vec2(slingVec.x * launchForceMultiplier / SCALE, slingVec.y * launchForceMultiplier / SCALE), true);
                        // Apply a linear impulse to the center of the bird's body, using the slingVec multiplied by the launchForceMultiplier and divided by SCALE to convert it to Box2D units. This impulse will launch the bird in the direction opposite to where it was dragged from the sling, with a strength proportional to how far it was dragged.


                        Dragging = false; // Set dragging flag to false when the left mouse button is released, regardless of whether the bird was launched or not
                        launched = true; // Set launched flag to true when the bird is launched, preventing further dragging or launching until the next bird is selected
						birdTimer.restart(); // Restart the bird timer to track the time since the last bird was launched.
                        activeBird->fired = true; // Set the fired flag of the active bird to true, indicating that it has been launched
                    }
                }
            }
        }

        // Update Physics
        world.Step(1.0f / 60.0f, 8, 3);

        //Catapult Dragging system

        if (Dragging && currentBird < Birds.size()) { //if the player is trying to drag the catapult and there is a bird to launch.
			Bird* activeBird = Birds[currentBird].get(); // Get pointer to the current active bird using the currentBird index
			b2Body* body = activeBird->getBody(); // Get the Box2D body of the active bird

			sf::Vector2i mousePos = sf::Mouse::getPosition(window); // Get the current position of the mouse cursor relative to the window
			
			//Using static cast to convert the mouse position from integer to float.
            sf::Vector2f mouseWorld(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)); 
            
			// Calculate the vector from the sling position to the mouse position.
            sf::Vector2f dragAmount = mouseWorld - slingpos;

			// Limit the drag amount to the defined sling radius to prevent excessive dragging.
            // This is done by clamping the dragAmount vector's x and y components to the maximum 
            // allowed values based on the slingRadiusX and slingRadiusY.
            dragAmount.x = std::clamp(dragAmount.x, -slingRadiusX, slingRadiusX);
			dragAmount.y = std::clamp(dragAmount.y, -slingRadiusY, slingRadiusY);

			sf::Vector2f finalPosition = slingpos + dragAmount; 
            // Calculate the final position of the bird based on the sling position and the limited drag amount
        
			// Update the bird's position to follow the mouse while dragging
            // This is done by setting the bird's physics body's transform to the calculated final position
			body->SetTransform(b2Vec2(finalPosition.x / SCALE, finalPosition.y / SCALE), 0);
        }

        if (launched) { //If bird has been launched


            // Get pointer to the current active bird using the currentBird index
			Bird* activeBird = Birds[currentBird].get();

            // Get the current speed of the active bird by calculating the length of its linear velocity vector
			float speed = activeBird->getBody()->GetLinearVelocity().Length(); 

			if (birdTimer.getElapsedTime().asSeconds() > waitingTimeThreshold) { // If has it been 5 seconds since the bird was launched.
            
				currentBird++; // Move to the next bird in the list by incrementing the currentBird index
				launched = false; // Reset the launched flag to false, allowing the next bird to be dragged and launched

                if (currentBird < Birds.size()) { // Check if there are more birds left to launch
					
                    // Get pointer to the next bird using the updated currentBird index
                    Bird* nextBird = Birds[currentBird].get();

					b2Body* nextBody = nextBird->getBody(); // Get the Box2D body of the next bird
					
                    // Set the next bird's body type to kinematic,
                    nextBody->SetType(b2_kinematicBody); 
					nextBody->SetGravityScale(0.0f); // Set gravity scale to 0 so it doesnt fall off the sling before being launched.

                    // Set the next bird's position to be on the sling, using the defined sling position and scaling it.
					nextBody->SetTransform(b2Vec2(slingpos.x / SCALE, slingpos.y / SCALE), 0); 


                    // Reset the next bird's linear velocity to zero to ensure it starts stationary on the sling
					nextBody->SetLinearVelocity(b2Vec2(0, 0));

					//Set angular velocity to 0 as well.
					nextBody->SetAngularVelocity(0);
                }
            } 
        }

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
       // sf_wallVisual.setPosition(b2_wallBody->GetPosition().x * SCALE, b2_wallBody->GetPosition().y * SCALE);

        // Dynamic wall.
        sf_plankVisual.setPosition(b2_plankBody->GetPosition().x * SCALE, b2_plankBody->GetPosition().y * SCALE);
        sf_plankVisual.setRotation(b2_plankBody->GetAngle() * (180.0f / PI));

		// Right wall
		sf_rightWallVisual.setPosition(b2_rightWallBody->GetPosition().x * SCALE, b2_rightWallBody->GetPosition().y * SCALE);
        
        
		// Block 2
		sf_WallVisual2.setPosition(b2_WallBody2->GetPosition().x * SCALE, b2_WallBody2->GetPosition().y * SCALE);
		sf_WallVisual2.setRotation(b2_WallBody2->GetAngle() * (180.0f / PI));

		// Block 3
		sf_WallVisual3.setPosition(b2_WallBody3->GetPosition().x * SCALE, b2_WallBody3->GetPosition().y * SCALE);
		sf_WallVisual3.setRotation(b2_WallBody3->GetAngle() * (180.0f / PI));

        //Render all of the content at each frame. Remember you need to clear the screen each iteration or artefacts remain.
        window.clear(sf::Color(135, 206, 235)); // Sky Blue

        window.draw(sf_groundVisual);
       // window.draw(sf_wallVisual);
        window.draw(sf_plankVisual);
        window.draw(sf_ballVisual);
		window.draw(sf_rightWallVisual);
		window.draw(sf_WallVisual2);
		window.draw(sf_WallVisual3);
		
		for (auto& pig : Pigs) { // Loop through each Pig in the list and render it
			pig->render(window); // Render the Pig instance
		}

		//bird.render(window); // Render the Bird instance
		for (auto& bird : Birds) { // Loop through each Bird in the list and render it
			bird->update(); // Update the Bird instance (if needed)
			bird->UpdateSprite(); // Update the Bird's sprite position based on its physics body
            bird->render(window); // Render the Bird instance
		}
        window.display();
    }

    return 0;
}
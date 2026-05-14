#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <iostream>
#include "Bird.h"
#include "Pig.h"
#include "Structure.h" 

#include <filesystem>
#include <list> // Include the list header for using std::list - the lists for the different birds and pigs :)
#include <vector>
#include <algorithm> // Include the algorithm header for using std::clamp in the catapult dragging system   
#include <cmath> // Include the cmath header for using mathematical functions like std::sqrt in the catapult dragging system
#include <memory> // Include the memory header for using smart pointers like std::unique_ptr to manage dynamic memory for birds, pigs, and structures in the game

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
	int currentBird = 0; // Index of the current bird being launched, which is used to keep track of which bird is currently active and being controlled by the player. This allows for managing multiple birds in the game and ensuring that the correct bird is launched when the player interacts with the sling.
	bool Dragging = false; // Flag to indicate whether the player is currently dragging the bird, which is used to manage the state of the sling and the bird's position. When this flag is true, it indicates that the player is actively dragging the bird, allowing for real-time updates to the bird's position and launch force based on the player's input.
	bool launched = false; // Flag to indicate whether the bird has been launched, which is used to manage the state of the game and ensure that the bird is only launched once per interaction with the sling. When this flag is true, it indicates that the bird has been launched, preventing further dragging or launching until the next bird is selected.
	float waitingTimer = 0.0f; // Timer to track the waiting time after a bird is launched, which can be used to implement a delay before allowing the next bird to be launched.
	float waitingTimeThreshold = 5.0f; // Threshold for the waiting time, which determines how long the player must wait after launching a bird before being able to launch the next one.
	sf::Clock birdTimer; // Clock to track the time since the last bird was launched.


    //For the structures
    float startX = 700.0f;
    float startY = 500.0f;


    //Anchor points for beams, then can just add onto that if i want to duplicate
	float baseY = 500.0f;
	float leftX = 780.0f;
    float midX = 900.0f;
	float rightX = 1020.0f;
    float innerX = 900.0f; // X position for the inner structure.

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

    //Adding physics to the sprites

    b2Vec2 b2_pos;
	b2BodyDef b2_BodyDef;
	b2FixtureDef b2_fixtureDef;
	b2Body* b2_body;

    //ContactListener
    
    ContactListener contactListener;
	world.SetContactListener(&contactListener);

	//list of birds and pigs
	std::vector<std::unique_ptr<Bird>> Birds;
    
	std::vector<sf::IntRect>birdsprites = { sf::IntRect(903, 798, 47, 47),sf::IntRect(300, 752, 100, 95),sf::IntRect(0, 378, 40, 32) 
	}; //These are the position and size of the different birds in the sprite sheet. We can use these to create multiple birds with different appearances. The sf::IntRect constructor takes four parameters: the x and y coordinates of the top-left corner of the rectangle, and the width and height of the rectangle. These rectangles define the portion of the sprite sheet that will be used for each bird's texture.

    //Giving different birds launch strengths as one doesnt go far enough.
	std::vector<float> launchForceMultipliers = { 
         15.0f, //red
         60.0f,//big red
         10.0f }; //blue
    // This vector holds different launch force multipliers for each bird, allowing for varying launch strengths based on the type of bird being launched. Each multiplier corresponds to a specific bird in the birdsprites vector, providing a way to customize the launch behavior for different bird types in the game.





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



    //Making seperate pigs so i canplace on on top of the structures.

	Pigs.push_back(std::make_unique<Pig>("../assets/Ang_Birds/Angry_Birds.png", pigsprites[0], b2Vec2((leftX + 20) / SCALE, 390.0f / SCALE), world, 1.0f, 3.0f, 0.5f)); 

	Pigs.push_back(std::make_unique<Pig>("../assets/Ang_Birds/Angry_Birds.png", pigsprites[1], b2Vec2((rightX + 20) / SCALE, 370.0f / SCALE), world, 1.0f, 3.0f, 0.5f));

	Pigs.push_back(std::make_unique<Pig>("../assets/Ang_Birds/Angry_Birds.png", pigsprites[2], b2Vec2(innerX / SCALE, 420.0f / SCALE), world, 1.0f, 3.0f, 0.5f));

    //For the Wood Blocks
    std::vector<sf::IntRect> woodSprites = { sf::IntRect(884, 394, 166, 19), sf::IntRect(884, 394, 166, 19), };

    // For the Ice Blocks
	std::vector<sf::IntRect> iceSprites = { sf::IntRect(304, 396, 166, 19), sf::IntRect(304, 396, 166, 19), };

    //For the Stone Blocks
	std::vector<sf::IntRect> stoneSprites = { sf::IntRect(1417, 403, 166, 19), sf::IntRect(1417, 403, 166, 19), };

	//For Box Blocks
	std::vector<sf::IntRect> boxSprites = { sf::IntRect(472, 1124, 79, 83) };

    // List to hold dynamic objects that make up the structures in the game
    std::vector<std::unique_ptr<Structure>> Structures;


  
     //BUILDING THE MAP//

    //Guide for which way you want the beams:
    //0 = flat
	//1 = vertical
	//b2_pi / 2 = vertical 
    //b2_pi = upside down

    //Bottom Beams (holding it up)
    Structures.push_back(std::make_unique<Structure>("../assets/Ang_Birds/Angry_Birds_Spritesheet_Blocks.png", woodSprites[0],
        b2Vec2(leftX / SCALE, baseY / SCALE), world, 1.0f, 1.5f, 0.0f, b2_pi / 2));
	

    //Adding 40 on the x axid to move it to the right
    Structures.push_back(std::make_unique<Structure>("../assets/Ang_Birds/Angry_Birds_Spritesheet_Blocks.png", woodSprites[0],
        b2Vec2((leftX + 40) / SCALE, baseY / SCALE), world, 1.0f, 1.5f, 0.0f, b2_pi / 2));


	//Top Beams (pig on top)
	Structures.push_back(std::make_unique<Structure>("../assets/Ang_Birds/Angry_Birds_Spritesheet_Blocks.png", stoneSprites[0],
		b2Vec2((leftX + 20) / SCALE, 430.0f / SCALE), world, 3.0f, 1.8f, 0.00f, 0));

    //Now the structure on the right (using rightX)

	Structures.push_back(std::make_unique<Structure>("../assets/Ang_Birds/Angry_Birds_Spritesheet_Blocks.png", woodSprites[0],
		b2Vec2(rightX / SCALE, baseY / SCALE), world, 1.0f, 0.5f, 0.0f, b2_pi / 2));

	Structures.push_back(std::make_unique<Structure>("../assets/Ang_Birds/Angry_Birds_Spritesheet_Blocks.png", woodSprites[0],
		b2Vec2((rightX + 40) / SCALE, baseY / SCALE), world, 1.0f, 1.5f, 0.0f, b2_pi / 2));

	//Ice blocks (the ones that are easier to break)
	Structures.push_back(std::make_unique<Structure>("../assets/Ang_Birds/Angry_Birds_Spritesheet_Blocks.png", iceSprites[0],
		b2Vec2((rightX + 20) / SCALE, 430.0f / SCALE), world, 0.5f, 0.5f, 0.0f, 0));


    //Then another sctructure in the middle

    //The top
	Structures.push_back(std::make_unique<Structure>("../assets/Ang_Birds/Angry_Birds_Spritesheet_Blocks.png", boxSprites[0],
		b2Vec2(innerX / SCALE, 500.0f / SCALE), world, 0.5f, 0.1f, 0.3f, 0));

    //The bottom

	Structures.push_back(std::make_unique<Structure>("../assets/Ang_Birds/Angry_Birds_Spritesheet_Blocks.png", boxSprites[0],
		b2Vec2(innerX  / SCALE, 460.0f / SCALE), world, 1.0f, 0.5f, 0.3f, 0));


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


                        //applying launch force for each bird
						float currentLaunchForceMultiplier = launchForceMultipliers[currentBird]; 
                        // Get the launch force multiplier

						// Applying a linear impulse to the bird's center based on the sling vector and the current launch force multiplier, which determines the strength and direction of the launch.
                        body->ApplyLinearImpulseToCenter(b2Vec2(slingVec.x * currentLaunchForceMultiplier / SCALE, slingVec.y * currentLaunchForceMultiplier / SCALE), true);
                        

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
			b2Body* body = activeBird->getBody(); // Get the Box2D body of the active bird

            // Get the current speed of the active bird by calculating the length of its linear velocity vector
			float speed = activeBird->getBody()->GetLinearVelocity().Length(); 

            if (birdTimer.getElapsedTime().asSeconds() > waitingTimeThreshold) { // If has it been 5 seconds since the bird was launched.

                Bird* oldBird = Birds[currentBird].get(); // Get pointer to the current active bird using the currentBird index
                b2Body* oldBody = oldBird->getBody(); // Get the Box2D body of the current active bird


                world.DestroyBody(body);
                launched = false; // Reset the launched flag to false, allowing the next bird to be dragged and launched
                currentBird++; // Move to the next bird in the list by incrementing the currentBird index

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

		//Update the structure blocks
		for (auto& structure : Structures) { // Loop through each Structure in the list and update it
              // Update the Structure instance (if needed)
			structure->update();
			structure->UpdateSprite();

		}

        //All of the visuals needs to be synced with the physics.

        //Static objects usually don't move, but we set the position once.
        sf_groundVisual.setPosition(b2_groundBody->GetPosition().x * SCALE, b2_groundBody->GetPosition().y * SCALE);
       // sf_wallVisual.setPosition(b2_wallBody->GetPosition().x * SCALE, b2_wallBody->GetPosition().y * SCALE);

		// Right wall
		sf_rightWallVisual.setPosition(b2_rightWallBody->GetPosition().x * SCALE, b2_rightWallBody->GetPosition().y * SCALE);
    
        //Render all of the content at each frame. Remember you need to clear the screen each iteration or artefacts remain.
        window.clear(sf::Color(135, 206, 235)); // Sky Blue

        window.draw(sf_groundVisual);
		window.draw(sf_rightWallVisual);
		
		for (auto& pig : Pigs) { // Loop through each Pig in the list and render it
			pig->render(window); // Render the Pig instance
		}

		//bird.render(window); // Render the Bird instance
		for (auto& bird : Birds) { // Loop through each Bird in the list and render it
			bird->update(); // Update the Bird instance (if needed)
			bird->UpdateSprite(); // Update the Bird's sprite position based on its physics body
            bird->render(window); // Render the Bird instance
		}

        for (auto& structure : Structures) { // Loop through each Structure in the list and render it
            structure->render(window); // Render the Structure instance
        }
        window.display();
    }

    return 0;
}
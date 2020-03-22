#pragma once

#include <SFML/Graphics.hpp> //Contains many essential SFML classes and functions for rendering
#include <vector> //Contains std::vector for storing objects in an array
#include "ObjectManager.h" //Contains the ObjectManager class, which is used to store a collection of all instantiated objects
#include <atomic> //Contains std::atomic, which is used to prevent data races over an entire object


//Makes any object that inherits from this, renderable
class Renderable : public ObjectManager<Renderable>
{
	int renderLayer = 0; //The render layer of the object
	static std::atomic<bool> listUpdated; //Whether the renderable list has been updated or not


public:

	//Constructs a new renderable object
	Renderable(bool renderingEnabled = false);

	//Gets all renderable objectst that have been instantiated
	static const std::vector<Renderable*>& GetRenderables();

	//Enables or disables rendering for this object
	void EnableRendering(bool enabled = true);

	//Gets the render layer of the object
	int GetRenderLayer() const;
	//Sets the render layer of the object
	void SetRenderLayer(int newLayer);

	//Used to render the object to the scene
	virtual void Render(sf::RenderWindow& window) = 0;

	//The destructor
	virtual ~Renderable();
};


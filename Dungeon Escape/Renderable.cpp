#include "Renderable.h" //Contains the Renderable class for rendering to the game window
#include <algorithm> //Contains many commonly used algorithms such as std::sort and std::find


std::atomic<bool> Renderable::listUpdated = false; //Whether the renderable list has been updated or not

//Constructs a new renderable object
Renderable::Renderable(bool renderingEnabled) :
	ObjectManager<Renderable>(renderingEnabled)
{
	//A new renderable object has been added to the list of renderables. Notify that the list has been updated
	listUpdated = true;
}

//Gets all renderable objectst that have been instantiated
const std::vector<Renderable*>& Renderable::GetRenderables()
{
	//Get the list of renderable objects
	auto& renderables = ObjectManager<Renderable>::GetObjectList();
	//If the list has been updated
	if (listUpdated)
	{
		//Lock the list mutex to prevent modification
		std::unique_lock<std::recursive_mutex> listLock{ GetMutex() };
		//Sort all the renderable objects by their render layer. This makes sure that objects are rendered in the right order
		std::sort(begin(renderables), end(renderables), [](Renderable* A, Renderable* B)
			{
				return A->renderLayer > B->renderLayer;
			});
		//The list no longer needs to be updated
		listUpdated = false;
	}
	//Return the list
	return renderables;
}

//Enables or disables rendering for this object
void Renderable::EnableRendering(bool enabled)
{
	//Set rendering active or inactive
	SetActive(enabled);
}

//Gets the render layer of the object
int Renderable::GetRenderLayer() const
{
	return renderLayer;
}

//Sets the render layer of the object
void Renderable::SetRenderLayer(int newLayer)
{
	//if the render layer is changed
	if (renderLayer != newLayer)
	{
		//Notify that the list is to be updated
		listUpdated = true;
	}
	//Update the render layer
	renderLayer = newLayer;
}

//The destructor
Renderable::~Renderable()
{
	//An object has been removed, so update the list
	listUpdated = true;
}

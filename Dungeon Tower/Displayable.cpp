#include "Displayable.h"
#include "Renderer.h"

Displayable::Displayable() 
{
	Renderer::AddDisplayable(this);
}

Displayable::~Displayable()
{
	Renderer::RemoveDisplayable(this);
}

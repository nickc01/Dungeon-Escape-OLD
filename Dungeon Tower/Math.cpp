#include "Math.h" //Contains many commonly used math functions

//Determines if two sprites interect
bool Math::SpritesIntersect(const sf::Sprite& A, const sf::Sprite& B)
{
	//Get the bounds of each of the sprites and see if they intersect
	return RectsIntersect(A.getGlobalBounds(), B.getGlobalBounds());
}

//Determines if two entities intersect
bool Math::EntitiesIntersect(const Entity& A, const Entity& B)
{
	//Get the hitboxes of each of the entities and see if they intersect
	return RectsIntersect(A.GetHitBoxGlobalBounds(), B.GetHitBoxGlobalBounds());
}

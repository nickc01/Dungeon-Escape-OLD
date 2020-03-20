#include "Math.h"
#include "Math.h"

bool Math::SpritesIntersect(const sf::Sprite& A, const sf::Sprite& B)
{
	return RectsIntersect(A.getGlobalBounds(), B.getGlobalBounds());
}

bool Math::EntitiesIntersect(const Entity& A, const Entity& B)
{
	auto hitboxA = A.GetHitBoxGlobalBounds();
	auto hibBoxB = B.GetHitBoxGlobalBounds();

	auto spriteA = A.GetSprite()->getGlobalBounds();
	auto spriteB = B.GetSprite()->getGlobalBounds();


	return RectsIntersect(A.GetHitBoxGlobalBounds(), B.GetHitBoxGlobalBounds());
}

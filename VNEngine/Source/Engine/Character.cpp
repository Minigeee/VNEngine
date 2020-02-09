#include <Engine/Character.h>

using namespace vne;

// ============================================================================
// ============================================================================

Character::Character()
{

}

Character::Character(const sf::String& name) :
	mName		(name)
{

}

// ============================================================================

void Character::setName(const sf::String& name)
{
	mName = name;
}

void Character::addImage(const sf::String& label, sf::Texture* image)
{
	mImages[label.toUtf32()] = image;
}

// ============================================================================

const sf::String& Character::getName() const
{
	return mName;
}

sf::Texture* Character::getImage(const sf::String& label) const
{
	auto it = mImages.find(label.toUtf32());
	if (it != mImages.end())
		return it->second;
	return 0;
}

// ============================================================================
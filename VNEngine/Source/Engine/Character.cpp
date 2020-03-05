#include <Engine/Character.h>
#include <Engine/Scene.h>
#include <Engine/Action.h>
#include <Engine/Engine.h>

using namespace vne;

// ============================================================================
// ============================================================================

Character::Character() :
	mScene		(0)
{

}

Character::Character(const sf::String& name) :
	mScene		(0),
	mName		(name)
{

}

// ============================================================================

void Character::setScene(Scene* scene)
{
	mScene = scene;
}

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

void Character::say(const sf::String& dialogue)
{
	if (!mScene) return;

	DialogueAction* action = mScene->alloc<DialogueAction>();
	action->setName(mName);
	action->setDialogue(gQuotationSymbol + dialogue + gQuotationSymbol);
	action->setTextStyle(sf::Text::Regular);
	mScene->addAction(action);
}

void Character::think(const sf::String& dialogue)
{
	if (!mScene) return;

	DialogueAction* action = mScene->alloc<DialogueAction>();
	action->setName(mName);
	action->setDialogue(dialogue);
	action->setTextStyle(sf::Text::Italic);
	mScene->addAction(action);
}

// ============================================================================
#include <Engine/Character.h>
#include <Engine/Scene.h>
#include <Engine/Action.h>
#include <Engine/Engine.h>

using namespace vne;

// ============================================================================
// ============================================================================

Character::Character() :
	mScene		(0),
	mImageBox	(0)
{

}

Character::Character(const sf::String& name) :
	mScene		(0),
	mName		(name),
	mImageBox	(0)
{

}

// ============================================================================

void Character::setScene(Scene* scene)
{
	mScene = scene;

	// Add character image to the scene if it is a novel scene
	NovelScene* novelScene = 0;
	if (novelScene = dynamic_cast<NovelScene*>(mScene))
	{
		if (!mImageBox)
			mImageBox = novelScene->getUI().create<ImageBox>(mName + "Image");

		novelScene->getUI().addToRoot(mImageBox);
	}
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

void Character::show(const sf::String& image, Transition effect)
{
	if (!mScene) return;

	ImageAction* action = mScene->alloc<ImageAction>();
	action->setMode(ImageAction::Show);
	action->setTexture(mImages[image.toUtf32()]);
	action->setTransition(effect);
	action->setImageBox(mImageBox);
	mScene->addAction(action);
}

// ============================================================================
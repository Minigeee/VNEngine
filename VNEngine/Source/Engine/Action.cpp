#include <Engine/Action.h>

#include <Engine/Scene.h>

using namespace vne;

// ============================================================================
// ============================================================================

Action::Action() :
	mScene			(0),
	mIsComplete		(false)
{

}

Action::~Action()
{

}

// ============================================================================

void Action::setScene(Scene* scene)
{
	mScene = scene;
}

void Action::setCondition(const std::function<bool()>& cond)
{
	mCondition = cond;
}

bool Action::isComplete() const
{
	return mIsComplete;
}

bool Action::conditionMet() const
{
	return !mCondition || (mCondition && mCondition());
}

// ============================================================================

void Action::update(float dt)
{

}

void Action::handleEvent(const sf::Event& e)
{

}

// ============================================================================
// ============================================================================

ActionGroup::ActionGroup()
{

}

ActionGroup::~ActionGroup()
{

}

// ============================================================================

void ActionGroup::run()
{
	if (!conditionMet()) return;

	// Run all actions
	for (Uint32 i = 0; i < mActions.size(); ++i)
		mActions[i]->run();
}

// ============================================================================

void ActionGroup::addAction(Action* action)
{
	mActions.push_back(action);
}

// ============================================================================
// ============================================================================

DialogueAction::DialogueAction() :
	mCurrentLine		(0)
{

}

DialogueAction::~DialogueAction()
{

}

// ============================================================================

void DialogueAction::run()
{
	NovelScene* scene = static_cast<NovelScene*>(mScene);
	TextBox* dialogueText = scene->getDialogueText();

	// Set strings
	scene->getNameText()->setString(mName);
	dialogueText->setString(mDialogue);

	// Reset clip shapes
	for (int i = 0; i < dialogueText->getClipShapes().size(); ++i)
	{
		sf::RectangleShape* rect =
			static_cast<sf::RectangleShape*>(dialogueText->getClipShapes()[i]);
		rect->setScale(0.0f, 1.0f);
	}

	// Create any clipping shapes if necessary
	float charSize = dialogueText->getBounds().height / dialogueText->getLineLengths().size();
	for (int i = dialogueText->getClipShapes().size(); i < dialogueText->getLineLengths().size(); ++i)
	{
		sf::RectangleShape* rect = scene->alloc<sf::RectangleShape>();
		rect->setSize(sf::Vector2f(60.0f, charSize));
		rect->setPosition(dialogueText->getAbsPosition() + sf::Vector2f(0.0f, i * charSize));
		rect->setScale(0.0f, 1.0f);
		dialogueText->addClipShape(rect);
	}
}

void DialogueAction::update(float dt)
{
	NovelScene* scene = static_cast<NovelScene*>(mScene);
	TextBox* dialogueText = scene->getDialogueText();

	// Only do update if there are still lines left to display
	if (mCurrentLine < dialogueText->getClipShapes().size())
	{
		sf::RectangleShape* rect =
			static_cast<sf::RectangleShape*>(dialogueText->getClipShapes()[mCurrentLine]);
		rect->setScale(rect->getScale().x + 6.0f * dt, 1.0f);

		// If reach the end of line, go to next one
		if (rect->getScale().x * rect->getSize().x > dialogueText->getLineLength(mCurrentLine))
			++mCurrentLine;
	}
}

void DialogueAction::handleEvent(const sf::Event& e)
{
	if (e.type == sf::Event::MouseButtonPressed)
		mIsComplete = true;

	else if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Space)
		mIsComplete = true;
}

// ============================================================================

void DialogueAction::setName(const sf::String& name)
{
	mName = name;
}

void DialogueAction::setDialogue(const sf::String& dialogue)
{
	mDialogue = dialogue;
}

// ============================================================================
// ============================================================================
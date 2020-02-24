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
	mNumChars		(0)
{

}

DialogueAction::~DialogueAction()
{

}

// ============================================================================

void DialogueAction::run()
{
	NovelScene* scene = static_cast<NovelScene*>(mScene);

	// Set name
	scene->getNameText()->setString(mName);

	// Reset to empty dialogue string
	scene->getDialogueText()->setString("");
}

void DialogueAction::update(float dt)
{
	NovelScene* scene = static_cast<NovelScene*>(mScene);

	if (++mNumChars % 1 == 0)
		scene->getDialogueText()->setString(mDialogue.substring(0, mNumChars / 1));
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
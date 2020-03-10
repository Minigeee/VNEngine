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
	mCurrentLine		(0),
	mTextSpeed			(600.0f),
	mStyle				(sf::Text::Regular)
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
	dialogueText->setClipMode(ClipMode::Shapes);

	// Hide name box
	if (mName.getSize() == 0)
	{
		scene->getNameBox()->setVisible(false);
	}
	else
	{
		scene->getNameText()->setString(mName);
		scene->getNameBox()->setVisible(true);
	}

	// Change text style
	if (dialogueText->getStyle() != mStyle)
		dialogueText->setStyle(mStyle);

	// Set strings
	dialogueText->setString(mDialogue);

	const std::vector<sf::Drawable*>& clipShapes = dialogueText->getClipShapes();
	const sf::Vector2f& textPos = dialogueText->getAbsPosition();
	int numLines = (int)dialogueText->getNumLines();
	float outlineThickness = dialogueText->getOutlineThickness();

	// Calculate the height of each line
	const sf::FloatRect& textBounds = dialogueText->getBounds();
	// The actual position of the text is at a slight offset from the absolute position
	float textOffset = textBounds.top + outlineThickness - textPos.y;
	float lineHeight = (textBounds.height - 2.0f * outlineThickness + textOffset) / numLines;
	float outlineScale = outlineThickness / lineHeight;

	// Create any clipping shapes if necessary
	for (int i = dialogueText->getClipShapes().size(); i < numLines; ++i)
	{
		sf::RectangleShape* rect = scene->alloc<sf::RectangleShape>();
		rect->setPosition(textPos + sf::Vector2f(-outlineThickness, i * lineHeight));

		dialogueText->addClipShape(rect);
	}

	// Reset clip shapes
	for (int i = 0; i < dialogueText->getClipShapes().size(); ++i)
	{
		sf::RectangleShape* rect = static_cast<sf::RectangleShape*>(clipShapes[i]);
		rect->setSize(sf::Vector2f(1.0f, lineHeight + textOffset * 0.5f));
		rect->setScale(0.0f, 1.0f);
	}

	// Change first shape
	sf::RectangleShape* first = static_cast<sf::RectangleShape*>(clipShapes[0]);
	first->setScale(0.0f, 1.0f + outlineScale * (numLines == 1 ? 2.0f : 1.0f));
	first->setPosition(textPos + sf::Vector2f(-outlineThickness, -outlineThickness));

	// Change last shape
	if (numLines > 1)
	{
		sf::RectangleShape* first = static_cast<sf::RectangleShape*>(clipShapes[numLines - 1]);
		first->setScale(0.0f, 1.0f + outlineScale);
	}
}

void DialogueAction::update(float dt)
{
	NovelScene* scene = static_cast<NovelScene*>(mScene);
	TextBox* dialogueText = scene->getDialogueText();

	// Only do update if there are still lines left to display
	if (mCurrentLine < dialogueText->getNumLines())
	{
		sf::RectangleShape* rect =
			static_cast<sf::RectangleShape*>(dialogueText->getClipShapes()[mCurrentLine]);
		rect->setScale(rect->getScale() + sf::Vector2f(mTextSpeed * dt, 0.0f));

		// If reach the end of line, go to next one
		if (rect->getScale().x * rect->getSize().x > dialogueText->getLineLength(mCurrentLine))
			++mCurrentLine;
	}
}

void DialogueAction::handleEvent(const sf::Event& e)
{
	NovelScene* scene = static_cast<NovelScene*>(mScene);
	TextBox* dialogueText = scene->getDialogueText();

	if (e.type == sf::Event::MouseButtonPressed)
	{
		if (mCurrentLine < dialogueText->getNumLines())
		{
			mCurrentLine = dialogueText->getNumLines();
			dialogueText->setClipMode(ClipMode::Disabled);
		}
		else
			mIsComplete = true;
	}
	else if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Space)
	{
		if (mCurrentLine < dialogueText->getNumLines())
		{
			mCurrentLine = dialogueText->getNumLines();
			dialogueText->setClipMode(ClipMode::Disabled);
		}
		else
			mIsComplete = true;
	}
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

void DialogueAction::setTextSpeed(float speed)
{
	mTextSpeed = speed;
}

void DialogueAction::setTextStyle(Uint32 style)
{
	mStyle = style;
}

// ============================================================================
// ============================================================================

BackgroundAction::BackgroundAction() :
	mTexture		(0),
	mEffect			(None)
{

}

BackgroundAction::~BackgroundAction()
{

}

// ============================================================================

void BackgroundAction::run()
{
	NovelScene* scene = static_cast<NovelScene*>(mScene);
	ImageBox* bg = scene->getBackground();

	// Set the texture
	bg->setTexture(mTexture);

	mIsComplete = true;

	/* TODO : Implement background transitions */
}

// ============================================================================

void BackgroundAction::setTexture(sf::Texture* texture)
{
	mTexture = texture;
}

void BackgroundAction::setTransition(Effect effect)
{
	mEffect = effect;
}

// ============================================================================
// ============================================================================
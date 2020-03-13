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

void Action::setComplete(bool complete)
{
	mIsComplete = complete;
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
	mTransition		(Transition::None)
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

	/* TODO : Implement background transitions */
	if (mTransition == Transition::Fade)
	{

	}

	else if (mTransition == Transition::FadeToBlack)
	{

	}

	else if (mTransition == Transition::FadeFromBlack)
	{

	}

	else
	{
		// Set the texture
		bg->setTexture(mTexture);

		mIsComplete = true;
	}
}

// ============================================================================

void BackgroundAction::setTexture(sf::Texture* texture)
{
	mTexture = texture;
}

void BackgroundAction::setTransition(Transition effect)
{
	mTransition = effect;
}

void BackgroundAction::setDuration(float duration)
{
	mDuration = duration;
}

// ============================================================================
// ============================================================================

ImageAction::ImageAction() :
	mMode		(Show),
	mTexture	(0),
	mTransition	(Transition::None),
	mDuration	(1.0f),
	mImageBox	(0)
{

}

ImageAction::~ImageAction()
{

}

// ============================================================================

void ImageAction::setMode(Mode mode)
{
	mMode = mode;
}

void ImageAction::setTexture(sf::Texture* texture)
{
	mTexture = texture;
}

void ImageAction::setTransition(Transition effect)
{
	mTransition = effect;
}

void ImageAction::setDuration(float duration)
{
	mDuration = duration;
}

void ImageAction::setImageBox(ImageBox* box)
{
	mImageBox = box;
}

// ============================================================================

void ImageAction::run()
{
	if (mMode == Show)
		show();
	else
		hide();
}

void ImageAction::show()
{
	NovelScene* scene = static_cast<NovelScene*>(mScene);
	Uint32 zIndex = scene->getDialogueBox()->getZIndex() - 1;
	UI& ui = scene->getUI();

	// Make sure the image is in the right z-position
	mImageBox->setZIndex(zIndex);

	if (mTransition == Transition::Fade)
	{
		// Interpolate between the 2
		mImageBox->setVisible(true);

		// If transition from image to image, then create a temporary copy to blend between images
		if (mImageBox->isVisible() && mImageBox->getTexture())
		{
			// Create a copy of this image
			ImageBox* box = ui.copy<ImageBox>(mImageBox->getName() + ".TEMP", mImageBox->getName());
			ui.addToRoot(box);
			box->setZIndex(zIndex);
		}

		// Set the new texture
		mImageBox->setTexture(mTexture);

		// Create animation
		ColorAnimation* anim = scene->alloc<ColorAnimation>(
			std::bind(&ImageBox::setColor, mImageBox, std::placeholders::_1),
			sf::Color(255, 255, 255, 0), sf::Color::White,
			mDuration
			);
		// Set finish callback
		anim->setFinishedFunc(std::bind(&ImageAction::onAnimComplete, this));

		// Add animation
		ui.addAnimation(anim);
	}

	else if (mTransition == Transition::FadeToBlack)
	{
		mImageBox->setVisible(true);

		// Create animation
		ColorAnimation* anim = scene->alloc<ColorAnimation>(
			std::bind(&ImageBox::setColor, mImageBox, std::placeholders::_1),
			sf::Color::White, sf::Color::Black,
			mDuration
			);
		// Set finish callback
		anim->setFinishedFunc(std::bind(&ImageAction::onAnimComplete, this));

		// Add animation
		ui.addAnimation(anim);
	}

	else if (mTransition == Transition::FadeFromBlack)
	{
		mImageBox->setVisible(true);
		mImageBox->setTexture(mTexture);

		// Create animation
		ColorAnimation* anim = scene->alloc<ColorAnimation>(
			std::bind(&ImageBox::setColor, mImageBox, std::placeholders::_1),
			sf::Color::Black, sf::Color::White,
			mDuration
			);
		// Set finish callback
		anim->setFinishedFunc(std::bind(&ImageAction::onAnimComplete, this));

		// Add animation
		ui.addAnimation(anim);
	}

	else
	{
		// Set the texture
		mImageBox->setTexture(mTexture);

		mIsComplete = true;
	}
}

void ImageAction::hide()
{
	NovelScene* scene = static_cast<NovelScene*>(mScene);
	UI& ui = scene->getUI();

	if (mTransition == Transition::Fade)
	{
		// Create animation
		ColorAnimation* anim = scene->alloc<ColorAnimation>(
			std::bind(&ImageBox::setColor, mImageBox, std::placeholders::_1),
			sf::Color::White, sf::Color(255, 255, 255, 0),
			mDuration
			);
		// Set finish callback
		anim->setFinishedFunc(std::bind(&ImageAction::onAnimComplete, this));

		// Add animation
		ui.addAnimation(anim);
	}

	else if (mTransition == Transition::FadeToBlack)
	{
		// Create animation
		ColorAnimation* anim = scene->alloc<ColorAnimation>(
			std::bind(&ImageBox::setColor, mImageBox, std::placeholders::_1),
			sf::Color::White, sf::Color::Black,
			mDuration
			);
		// Set finish callback
		anim->setFinishedFunc(std::bind(&ImageAction::onAnimComplete, this));

		// Add animation
		ui.addAnimation(anim);
	}

	else
	{
		mImageBox->setVisible(false);
		mIsComplete = true;
	}
}

// ============================================================================

void ImageAction::onAnimComplete()
{
	NovelScene* scene = static_cast<NovelScene*>(mScene);
	UI& ui = scene->getUI();

	if (mMode == Show)
		// Remove temporary element
		ui.remove<ImageBox>(mImageBox->getName() + ".TEMP");
	else
		// Hide the image
		mImageBox->setVisible(false);

	mIsComplete = true;
}

// ============================================================================
// ============================================================================
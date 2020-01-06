#include <UI/Button.h>
#include <UI/UI.h>

#include <Core/Math.h>

#include <Engine/Engine.h>

using namespace vne;

// ============================================================================
// ============================================================================

Button::Button() :
	mState			(Default)
{

}

Button::~Button()
{

}

// ============================================================================
// ============================================================================

sf::RectangleShape& Button::getBody()
{
	mDrawablesChanged = true;
	return mBody;
}

const sf::RectangleShape& Button::getBody() const
{
	return mBody;
}

Text& Button::getLabel()
{
	mDrawablesChanged = true;
	return mLabel;
}

const Text& Button::getLabel() const
{
	return mLabel;
}

Button::State Button::getState() const
{
	return mState;
}

// ============================================================================

void Button::update(float dt)
{
	if (mDrawablesChanged)
	{
		updateAbsTransforms();

		mBody.setPosition(mAbsPosition);
		mBody.setRotation(mAbsRotation);
		mBody.setSize(mSize);
		mBody.setOrigin(mOrigin * mSize);

		const sf::FloatRect& xBounds =
			mLabel.getFont()->getGlyph(
				L'X',
				mLabel.getCharacterSize(),
				mLabel.getStyle() & sf::Text::Bold
			).bounds;
		const sf::FloatRect& box = mLabel.getLocalBounds();
		sf::Vector2f origin =
			sf::Vector2f(box.width * 0.5f, -xBounds.top * 0.5f + box.top)
			- mSize * 0.5f;

		// Adjust origin for text scale
		mLabel.setOrigin(origin / mLabel.getScale());
		mLabel.setPosition(mAbsPosition - mBody.getOrigin());
		mLabel.setRotation(mAbsRotation);

		mDrawablesChanged = false;
	}
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mBody, states);
	target.draw(mLabel, states);
}

// ============================================================================

void Button::setDefaultStateFunc(Button::Callback func)
{
	mDefaultStateFunc = func;

	State prevState = mState;
	mState = Default;
	mDefaultStateFunc(this, prevState);
}

void Button::setHoverStateFunc(Button::Callback func)
{
	mHoverStateFunc = func;
}

void Button::setPressStateFunc(Button::Callback func)
{
	mPressStateFunc = func;
}

// ============================================================================

void Button::onInit(UI* ui)
{
	if (ui->getDefaultFont())
		mLabel.setFont(*ui->getDefaultFont());

	// Set view
	mLabel.setView(&mEngine->getWindow().getView());
}

void Button::onMouseEnter(const sf::Event& e)
{
	if (mState == Default)
	{
		mState = Hover;
		if (mHoverStateFunc)
			mHoverStateFunc(this, Default);
	}
}

void Button::onMouseExit(const sf::Event& e)
{
	if (mState == Hover)
	{
		mState = Default;
		if (mDefaultStateFunc)
			mDefaultStateFunc(this, Hover);
	}
}

void Button::onMousePress(const sf::Event& e)
{
	if (mState == Hover)
	{
		mState = Press;
		if (mPressStateFunc)
			mPressStateFunc(this, Hover);
	}
}

void Button::onMouseRelease(const sf::Event& e)
{
	if (mState == Press)
	{
		if (mHasHover)
		{
			mState = Hover;
			if (mHoverStateFunc)
				mHoverStateFunc(this, Press);
		}
		else
		{
			mState = Default;
			if (mDefaultStateFunc)
				mDefaultStateFunc(this, Press);
		}
	}
}

// ============================================================================
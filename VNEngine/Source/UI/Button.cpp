#include <UI/Button.h>
#include <UI/UI.h>

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
	return mBody;
}

sf::Text& Button::getLabel()
{
	return mLabel;
}

Button::State Button::getState() const
{
	return mState;
}

// ============================================================================

void Button::update()
{
	if (!mDrawablesUpdated)
	{
		updateAbsTransforms();

		mBody.setPosition(mAbsPosition);
		mBody.setRotation(mAbsRotation);
		mBody.setSize(mSize);
		mBody.setOrigin(mOrigin * mSize);

		sf::Vector2f center = mAbsPosition - mBody.getOrigin() + mSize * 0.5f;
		const sf::FloatRect& xBounds =
			mLabel.getFont()->getGlyph(
				L'X',
				mLabel.getCharacterSize(),
				mLabel.getStyle() & sf::Text::Bold
			).bounds;
		const sf::FloatRect& box = mLabel.getLocalBounds();

		mLabel.setOrigin(
			box.width * 0.5f,
			-xBounds.top * 0.5f + box.top
		);
		mLabel.setPosition(center);
		mLabel.setRotation(mAbsRotation);

		mDrawablesUpdated = true;
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
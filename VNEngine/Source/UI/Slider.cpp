#include <UI/Slider.h>

#include <Core/Math.h>

using namespace vne;

// ============================================================================
// ============================================================================

Slider::Slider() :
	mValue			(0.0f),
	mSliderPressed	(false),
	mPressOffset	(0.0f)
{

}

Slider::~Slider()
{

}

// ============================================================================
// ============================================================================

sf::RectangleShape& Slider::getBody()
{
	mDrawablesChanged = true;
	return mBody;
}

const sf::RectangleShape& Slider::getBody() const
{
	return mBody;
}

sf::RectangleShape& Slider::getSlider()
{
	mDrawablesChanged = true;
	return mSlider;
}

const sf::RectangleShape& Slider::getSlider() const
{
	return mSlider;
}

void Slider::setValue(float value)
{
	float newValue = value;
	if (newValue < 0.0f)
		newValue = 0.0f;
	else if (newValue > 1.0f)
		newValue = 1.0f;

	if (mValue != newValue)
	{
		mValue = newValue;

		if (mValueChangedFunc)
			mValueChangedFunc(this);
	}
}

float Slider::getValue() const
{
	return mValue;
}

void Slider::setValueChangedFunc(const std::function<void(Slider*)>& func)
{
	mValueChangedFunc = func;
}

// ============================================================================

void Slider::update(float dt)
{
	if (mDrawablesChanged)
	{
		updateAbsTransforms();

		mBody.setPosition(mAbsPosition);
		mBody.setRotation(mAbsRotation);
		mBody.setSize(mSize);
		mBody.setOrigin(mOrigin * mSize);

		sf::Vector2f topLeft = mAbsPosition - mBody.getOrigin();
		sf::Vector2f sliderPos = -mSlider.getOrigin();
		const sf::Vector2f sliderSize = mSlider.getSize();

		// Clamp slider x-position
		if (mValue < 0.0f) mValue = 0.0f;
		else if (mValue > 1.0f) mValue = 1.0f;
		sliderPos.x = (mSize.x - sliderSize.x) * mValue;
		sliderPos.y = (mSize.y - sliderSize.y) * 0.5f;

		mSlider.setOrigin(-sliderPos);
		mSlider.setPosition(topLeft);
		mSlider.setRotation(mAbsRotation);

		mDrawablesChanged = false;
	}
}

// ============================================================================

void Slider::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mBody, states);
	target.draw(mSlider, states);
}

// ============================================================================
// ============================================================================

void Slider::onMousePress(const sf::Event& e)
{
	sf::Vector2f p = screenToLocal(sf::Vector2i(e.mouseButton.x, e.mouseButton.y));
	sf::FloatRect sliderBox = mSlider.getLocalBounds();
	sliderBox.left = -mSlider.getOrigin().x;
	sliderBox.top = -mSlider.getOrigin().y;

	mSliderPressed = sliderBox.contains(p);

	if (mSliderPressed)
	{
		// Offset is negative, so add instead of subtract
		mPressOffset = p.x + mSlider.getOrigin().x;
	}
}

// ============================================================================

void Slider::onMouseRelease(const sf::Event& e)
{
	mSliderPressed = false;
}

// ============================================================================

void Slider::onMouseMove(const sf::Event& e, const sf::Vector2f& p)
{
	if (mSliderPressed)
	{
		float newPos = -mSlider.getOrigin().x;

		// Only update if mouse x-coordinate changed
		if (newPos != p.x - mPressOffset)
		{
			newPos = p.x - mPressOffset;
			// Update value
			float newValue = newPos / (mSize.x - mSlider.getSize().x);
			if (newValue < 0.0f)
				newValue = 0.0f;
			else if (newValue > 1.0f)
				newValue = 1.0f;

			// Only update if value changed
			if (mValue != newValue)
			{
				mValue = newValue;

				if (mValueChangedFunc)
					mValueChangedFunc(this);

				mDrawablesChanged = true;
			}
		}
	}
}

// ============================================================================
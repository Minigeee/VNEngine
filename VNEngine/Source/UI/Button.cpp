#include <UI/Button.h>

using namespace vne;

// ============================================================================
// ============================================================================

Button::Button()
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

// ============================================================================

void Button::update()
{
	if (mTransformChanged)
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
	}
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mBody, states);
	target.draw(mLabel, states);
}

// ============================================================================
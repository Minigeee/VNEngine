#include <UI/TextCursor.h>

using namespace vne;

// ============================================================================
// ============================================================================

TextCursor::TextCursor() :
	mRelHeight			(-10.0f),
	mAnimationTime		(0.0f),
	mAnimationPeriod	(1.0f)
{
	// Default width 2
	mBody.setSize(sf::Vector2f(2.0f, 0.0f));
}

TextCursor::~TextCursor()
{

}

// ============================================================================
// ============================================================================

void TextCursor::setRelHeight(float h)
{
	mRelHeight = h;
}

void TextCursor::setWidth(float w)
{
	mBody.setSize(sf::Vector2f(w, mBody.getSize().y));
}

void TextCursor::setColor(const sf::Color& c)
{
	mBody.setFillColor(c);
}

void TextCursor::setTexture(sf::Texture* texture)
{
	mBody.setTexture(texture, true);
}

void TextCursor::setAnimationPeriod(float period)
{
	mAnimationPeriod = period;
}

// ============================================================================

float TextCursor::getRelHeight() const
{
	return mRelHeight;
}

float TextCursor::getWidth() const
{
	return mBody.getSize().x;
}

const sf::Color& TextCursor::getColor() const
{
	return mBody.getFillColor();
}

float TextCursor::getAnimationPeriod() const
{
	return mAnimationPeriod;
}

// ============================================================================
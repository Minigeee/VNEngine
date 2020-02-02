#include <UI/Image.h>

#include <Core/Math.h>

using namespace vne;

// ============================================================================
// ============================================================================

Image::Image()
{

}

Image::~Image()
{

}

// ============================================================================
// ============================================================================

void Image::setTexture(const sf::Texture* texture)
{
	mImage.setTexture(*texture);

	// Set size
	setSize((sf::Vector2f)texture->getSize());
}

void Image::setTextureRect(const sf::IntRect& rect, bool resize)
{
	mImage.setTextureRect(rect);

	// Resize
	if (resize)
		setSize((sf::Vector2f)rect.getSize());
}

void Image::setColor(const sf::Color& c)
{
	mImage.setColor(c);
}

const sf::Texture* Image::getTexture() const
{
	return mImage.getTexture();
}

const sf::IntRect& Image::getTextureRect() const
{
	return mImage.getTextureRect();
}

const sf::Color& Image::getColor() const
{
	return mImage.getColor();
}

// ============================================================================

void Image::onInit(UI* ui)
{

}

void Image::update(float dt)
{
	if (mDrawablesChanged)
	{
		updateAbsTransforms();

		mImage.setPosition(mAbsPosition);
		mImage.setRotation(mAbsRotation);
		mImage.setOrigin(mOrigin * mSize);

		// Change scale
		mImage.setScale(mSize / (sf::Vector2f)mImage.getTexture()->getSize());

		mDrawablesChanged = false;
	}
}

// ============================================================================

void Image::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mImage);
}

// ============================================================================
#include <UI/ImageBox.h>

#include <Core/Math.h>

using namespace vne;

// ============================================================================
// ============================================================================

ImageBox::ImageBox()
{

}

ImageBox::~ImageBox()
{

}

// ============================================================================
// ============================================================================

void ImageBox::setTexture(const sf::Texture* texture)
{
	mImage.setTexture(*texture);

	// Set size
	setSize((sf::Vector2f)texture->getSize());
}

void ImageBox::setTextureRect(const sf::IntRect& rect, bool resize)
{
	mImage.setTextureRect(rect);

	// Resize
	if (resize)
		setSize((sf::Vector2f)rect.getSize());
}

void ImageBox::setColor(const sf::Color& c)
{
	mImage.setColor(c);
}

const sf::Texture* ImageBox::getTexture() const
{
	return mImage.getTexture();
}

const sf::IntRect& ImageBox::getTextureRect() const
{
	return mImage.getTextureRect();
}

const sf::Color& ImageBox::getColor() const
{
	return mImage.getColor();
}

// ============================================================================

void ImageBox::onInit(UI* ui)
{

}

void ImageBox::update(float dt)
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

void ImageBox::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mImage);
}

// ============================================================================
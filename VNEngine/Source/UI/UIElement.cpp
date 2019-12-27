#include <UI/UIElement.h>

using namespace vne;

// ============================================================================
// ============================================================================

UIElement::UIElement() :
	mParent				(0),
	mRelPosition		(0.0f, 0.0f),
	mAbsPosition		(0.0f, 0.0f),
	mRelRotation		(0.0f),
	mAbsRotation		(0.0f),
	mSize				(0.0f, 0.0f),
	mOrigin				(0.0f, 0.0f),
	mAnchor				(0.0f, 0.0f),
	mTexture			(0),
	mTransformChanged	(false)
{

}

UIElement::~UIElement()
{

}

// ============================================================================
// ============================================================================

void UIElement::addChild(UIElement* child)
{
	child->mParent = this;
	mChildren.push_back(child);
}

// ============================================================================

void UIElement::setPosition(const sf::Vector2f& pos)
{
	mRelPosition = pos;
	mTransformChanged = true;
}

void UIElement::setPosition(float x, float y)
{
	mRelPosition = sf::Vector2f(x, y);
	mTransformChanged = true;
}

void UIElement::move(const sf::Vector2f& pos)
{
	mRelPosition += pos;
	mTransformChanged = true;
}

void UIElement::move(float x, float y)
{
	mRelPosition += sf::Vector2f(x, y);
	mTransformChanged = true;
}

// ============================================================================

void UIElement::setRotation(float rotation)
{
	mRelRotation = fmodf(rotation, 360.0f);
	mTransformChanged = true;
}

void UIElement::rotate(float rotation)
{
	mRelRotation = fmodf(mRelRotation + rotation, 360.0f);
	mTransformChanged = true;
}

// ============================================================================

void UIElement::setSize(const sf::Vector2f& size)
{
	mSize = size;
	mTransformChanged = true;
}

void UIElement::setSize(float w, float h)
{
	mSize = sf::Vector2f(w, h);
	mTransformChanged = true;
}

void UIElement::scale(const sf::Vector2f& size)
{
	mSize.x *= size.x;
	mSize.y *= size.y;
	mTransformChanged = true;
}

void UIElement::scale(float w, float h)
{
	mSize.x *= w;
	mSize.y *= h;
	mTransformChanged = true;
}

// ============================================================================

void UIElement::setOrigin(const sf::Vector2f& origin)
{
	mOrigin = origin;
	mTransformChanged = true;
}

void UIElement::setOrigin(float x, float y)
{
	mOrigin = sf::Vector2f(x, y);
	mTransformChanged = true;
}

// ============================================================================

void UIElement::setAnchor(const sf::Vector2f& anchor)
{
	mAnchor = anchor;
	mTransformChanged = true;
}

void UIElement::setAnchor(float x, float y)
{
	mAnchor = sf::Vector2f(x, y);
	mTransformChanged = true;
}

// ============================================================================

void UIElement::setTexture(sf::Texture* texture)
{
	mTexture = texture;
}

// ============================================================================

const sf::Vector2f& UIElement::getRelPosition() const
{
	return mRelPosition;
}

const sf::Vector2f& UIElement::getAbsPosition()
{
	updateAbsTransforms();
	return mAbsPosition;
}

float UIElement::getRelRotation() const
{
	return mRelRotation;
}

float UIElement::getAbsRotation()
{
	updateAbsTransforms();
	return mAbsRotation;
}

const sf::Vector2f& UIElement::getSize() const
{
	return mSize;
}

const sf::Vector2f& UIElement::getOrigin() const
{
	return mOrigin;
}

const sf::Vector2f& UIElement::getAnchor() const
{
	return mAnchor;
}

sf::Texture* UIElement::getTexture() const
{
	return mTexture;
}

UIElement* UIElement::getParent() const
{
	return mParent;
}

const std::vector<UIElement*>& UIElement::getChildren() const
{
	return mChildren;
}

// ============================================================================

void UIElement::updateAbsTransforms()
{
	if (mTransformChanged)
	{
		mAbsPosition = mRelPosition;
		mAbsRotation = mRelRotation;

		if (mParent)
		{
			mParent->updateAbsTransforms();

			// Calculate anchor offset
			sf::Vector2f anchorOffset(
				mAnchor.x * mParent->mSize.x,
				mAnchor.y * mParent->mSize.y
			);

			mAbsPosition += mParent->mAbsPosition + anchorOffset;
			mAbsRotation += mParent->mAbsRotation;
		}

		mAbsRotation = fmodf(mAbsRotation, 360.0f);

		mTransformChanged = false;
	}
}

// ============================================================================
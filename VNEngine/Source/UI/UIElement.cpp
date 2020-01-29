#include <UI/UIElement.h>

#include <Core/Math.h>

#include <Engine/Engine.h>

#include <UI/UI.h>

using namespace vne;

// ============================================================================
// ============================================================================

UIElement::UIElement() :
	mEngine				(0),
	mName				(""),
	mParent				(0),
	mRelPosition		(0.0f, 0.0f),
	mAbsPosition		(0.0f, 0.0f),
	mRelRotation		(0.0f),
	mAbsRotation		(0.0f),
	mSize				(0.0f, 0.0f),
	mOrigin				(0.0f, 0.0f),
	mAnchor				(0.0f, 0.0f),
	mIsVisible			(true),
	mClipEnabled		(false),
	mClipBounds			(0.0f, 0.0f, 0.0f, 0.0f),
	mTransformChanged	(false),
	mDrawablesChanged	(false),
	mHasFocus			(false),
	mHasHover			(false),
	mIsMousePressed		(false)
{

}

UIElement::~UIElement()
{

}

// ============================================================================

UIElement::UIElement(const UIElement& other) :
	mEngine				(0),
	mName				(""),
	mParent				(0),
	mRelPosition		(other.mRelPosition),
	mAbsPosition		(other.mAbsPosition),
	mRelRotation		(other.mRelRotation),
	mAbsRotation		(other.mAbsRotation),
	mSize				(other.mSize),
	mOrigin				(other.mOrigin),
	mAnchor				(other.mAnchor),
	mTransformChanged	(false),
	mDrawablesChanged	(false),
	mHasFocus			(false),
	mHasHover			(false),
	mIsMousePressed		(false)
{
	// Just reset parent and name
}

UIElement& UIElement::operator=(const UIElement& other)
{
	if (&other != this)
	{
		// Just reset parent and name
		mEngine = 0;
		mName = "";
		mParent = 0;
		mRelPosition = other.mRelPosition;
		mAbsPosition = other.mAbsPosition;
		mRelRotation = other.mRelRotation;
		mAbsRotation = other.mAbsRotation;
		mSize = other.mSize;
		mOrigin = other.mOrigin;
		mAnchor = other.mAnchor;
		mTransformChanged = true;
		mDrawablesChanged = true;
		mHasFocus = false;
		mHasHover = false;
		mIsMousePressed = false;
	}

	return *this;
}

// ============================================================================
// ============================================================================

void UIElement::init(UI* ui, const sf::String& name)
{
	// Only initialize if not initialized
	if (!mEngine)
	{
		mEngine = ui->mEngine;
		mName = name;

		// Custom init
		onInit(ui);
	}
}

void UIElement::setName(const sf::String& name)
{
	mName = name;
}

void UIElement::addChild(UIElement* child)
{
	child->mParent = this;
	mChildren.push_back(child);
}

// ============================================================================

void UIElement::moveToFront()
{
	if (mParent)
		setZIndex(mParent->mChildren.size() - 1);
}

void UIElement::moveToBack()
{
	setZIndex(0);
}

void UIElement::setZIndex(Uint32 index)
{
	if (mParent)
	{
		std::vector<UIElement*>& siblings = mParent->mChildren;
		Uint32 src = getZIndex();

		// Clamp to range of number of siblings
		if (index >= siblings.size())
			index = siblings.size() - 1;

		// Shift all siblings
		if (src < index)
		{
			for (int i = src; i < index; ++i)
				siblings[i] = siblings[i + 1];
		}
		else
		{
			for (int i = src; i > index; --i)
				siblings[i] = siblings[i - 1];
		}

		// Update element in list
		siblings[index] = this;
	}
}

// ============================================================================

void UIElement::setPosition(const sf::Vector2f& pos)
{
	mRelPosition = pos;
	transformDirty();
}

void UIElement::setPosition(float x, float y)
{
	mRelPosition = sf::Vector2f(x, y);
	transformDirty();
}

void UIElement::move(const sf::Vector2f& pos)
{
	mRelPosition += pos;
	transformDirty();
}

void UIElement::move(float x, float y)
{
	mRelPosition += sf::Vector2f(x, y);
	transformDirty();
}

// ============================================================================

void UIElement::setRotation(float rotation)
{
	mRelRotation = fmodf(rotation, 360.0f);
	transformDirty();
}

void UIElement::rotate(float rotation)
{
	mRelRotation = fmodf(mRelRotation + rotation, 360.0f);
	transformDirty();
}

// ============================================================================

void UIElement::setSize(const sf::Vector2f& size)
{
	mSize = size;
	transformDirty();
}

void UIElement::setSize(float w, float h)
{
	mSize = sf::Vector2f(w, h);
	transformDirty();
}

void UIElement::scale(const sf::Vector2f& size)
{
	mSize.x *= size.x;
	mSize.y *= size.y;
	transformDirty();
}

void UIElement::scale(float w, float h)
{
	mSize.x *= w;
	mSize.y *= h;
	transformDirty();
}

// ============================================================================

void UIElement::setOrigin(const sf::Vector2f& origin)
{
	mOrigin = origin;
	transformDirty();
}

void UIElement::setOrigin(float x, float y)
{
	mOrigin = sf::Vector2f(x, y);
	transformDirty();
}

// ============================================================================

void UIElement::setAnchor(const sf::Vector2f& anchor)
{
	mAnchor = anchor;
	transformDirty();
}

void UIElement::setAnchor(float x, float y)
{
	mAnchor = sf::Vector2f(x, y);
	transformDirty();
}

// ============================================================================

void UIElement::setVisible(bool visible, bool recursive)
{
	if (visible != mIsVisible)
	{
		mIsVisible = visible;

		if (recursive)
		{
			for (Uint32 i = 0; i < mChildren.size(); ++i)
				mChildren[i]->setVisible(visible, recursive);
		}
	}
}

void UIElement::setClipEnabled(bool enabled)
{
	if (enabled != mClipEnabled)
	{
		mClipEnabled = enabled;

		for (Uint32 i = 0; i < mChildren.size(); ++i)
			mChildren[i]->setClipEnabled(enabled);
	}
}

void UIElement::setClipBounds(const sf::FloatRect& bounds)
{
	mClipEnabled = true;
	mClipBounds = bounds;

	for (Uint32 i = 0; i < mChildren.size(); ++i)
		mChildren[i]->setClipBounds(bounds);
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

const sf::FloatRect& UIElement::getBounds()
{
	updateAbsTransforms();
	return mBounds;
}

bool UIElement::isVisible() const
{
	return mIsVisible;
}

bool UIElement::isClipEnabled() const
{
	return mClipEnabled;
}

const sf::FloatRect& UIElement::getClipBounds() const
{
	return mClipBounds;
}

const sf::String& UIElement::getName() const
{
	return mName;
}

UIElement* UIElement::getParent() const
{
	return mParent;
}

const std::vector<UIElement*>& UIElement::getChildren() const
{
	return mChildren;
}

Uint32 UIElement::getZIndex() const
{
	if (mParent)
	{
		const std::vector<UIElement*>& siblings = mParent->mChildren;

		// Brute force linear search
		for (Uint32 i = 0; i < siblings.size(); ++i)
		{
			if (siblings[i] == this)
				return i;
		}
	}

	// If this is an unattached element, or the search failed
	return 0;
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
			mAbsPosition += mParent->mAbsPosition + mAnchor * mParent->mSize;
			mAbsRotation += mParent->mAbsRotation;
		}

		mAbsRotation = fmodf(mAbsRotation, 360.0f);

		// Create transform, based on SFML Transformable
		float angle = -mAbsRotation * 3.141592654f / 180.f;
		float c = static_cast<float>(std::cos(angle));
		float s = static_cast<float>(std::sin(angle));
		float ox = mOrigin.x * mSize.x;
		float oy = mOrigin.y * mSize.y;
		float tx = -ox * c - oy * s + mAbsPosition.x;
		float ty = ox * s - oy * c + mAbsPosition.y;

		sf::Transform t(c, s, tx,
			-s, c, ty,
			0.f, 0.f, 1.f);

		// Update bounds
		mBounds = t.transformRect(sf::FloatRect(sf::Vector2f(0.0f, 0.0f), mSize));

		mTransformChanged = false;
	}
}

// ============================================================================

void UIElement::transformDirty()
{
	mTransformChanged = true;
	mDrawablesChanged = true;

	for (Uint32 i = 0; i < mChildren.size(); ++i)
		mChildren[i]->transformDirty();
}

// ============================================================================

sf::Vector2f UIElement::coordToLocal(const sf::Vector2f& coord)
{
	// Make sure transforms are updated
	updateAbsTransforms();

	// Adjust for translation
	sf::Vector2f p = coord - mAbsPosition;
	// Adjust for rotation
	float angle = -toRadians(mAbsRotation);
	float ca = cos(angle);
	float sa = sin(angle);
	p = sf::Vector2f(p.x * ca - p.y * sa, p.x * sa + p.y * ca);

	return p + mSize * mOrigin;
}

sf::Vector2f UIElement::screenToLocal(const sf::Vector2i& screen)
{
	return coordToLocal(mEngine->getWindow().mapPixelToCoords(screen));
}

// ============================================================================
// ============================================================================

void UIElement::setFocusFunc(const std::function<void(UIElement*)>& func)
{
	mFocusFunc = func;
}

void UIElement::setUnfocusFunc(const std::function<void(UIElement*)>& func)
{
	mUnfocusFunc = func;
}

void UIElement::setMouseEnterFunc(const UICallback& func)
{
	mMouseEnterFunc = func;
}

void UIElement::setMouseExitFunc(const UICallback& func)
{
	mMouseExitFunc = func;
}

void UIElement::setMouseMoveFunc(const UICallback& func)
{
	mMouseMoveFunc = func;
}

void UIElement::setMousePressFunc(const UICallback& func)
{
	mMousePressFunc = func;
}

void UIElement::setMouseReleaseFunc(const UICallback& func)
{
	mMouseReleaseFunc = func;
}

void UIElement::setMouseScrollFunc(const UICallback& func)
{
	mMouseScrollFunc = func;
}

void UIElement::setKeyPressFunc(const UICallback& func)
{
	mKeyPressFunc = func;
}

void UIElement::setKeyReleaseFunc(const UICallback& func)
{
	mKeyReleaseFunc = func;
}

void UIElement::setTextEnteredFunc(const UICallback& func)
{
	mTextEnteredFunc = func;
}

// ============================================================================

bool UIElement::hasFocus() const
{
	return mHasFocus;
}

bool UIElement::hasHover() const
{
	return mHasHover;
}

bool UIElement::isMousePressed() const
{
	return mIsMousePressed;
}

// ============================================================================

void UIElement::onInit(UI* ui)
{

}

void UIElement::onFocus()
{

}

void UIElement::onUnfocus()
{

}

void UIElement::onMouseEnter(const sf::Event& e)
{

}

void UIElement::onMouseExit(const sf::Event& e)
{

}

bool UIElement::onMouseMove(const sf::Event& e, const sf::Vector2f& localPos)
{
	return false;
}

bool UIElement::onMousePress(const sf::Event& e)
{
	return false;
}

bool UIElement::onMouseRelease(const sf::Event& e)
{
	return false;
}

bool UIElement::onMouseScroll(const sf::Event& e)
{
	return false;
}

void UIElement::onKeyPress(const sf::Event& e)
{

}

void UIElement::onKeyRelease(const sf::Event& e)
{

}

void UIElement::onTextEntered(const sf::Event& e)
{

}

// ============================================================================
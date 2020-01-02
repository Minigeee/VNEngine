#include <UI/UIElement.h>

#include <Core/Math.h>

using namespace vne;

// ============================================================================
// ============================================================================

UIElement::UIElement() :
	mEngine				(0),
	mParent				(0),
	mRelPosition		(0.0f, 0.0f),
	mAbsPosition		(0.0f, 0.0f),
	mRelRotation		(0.0f),
	mAbsRotation		(0.0f),
	mSize				(0.0f, 0.0f),
	mOrigin				(0.0f, 0.0f),
	mAnchor				(0.0f, 0.0f),
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
// ============================================================================

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
	mIsVisible = visible;

	if (recursive)
	{
		for (Uint32 i = 0; i < mChildren.size(); ++i)
			mChildren[i]->setVisible(visible, recursive);
	}
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

bool UIElement::isVisible() const
{
	return mIsVisible;
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

void UIElement::onMouseMove(const sf::Event& e)
{

}

void UIElement::onMousePress(const sf::Event& e)
{

}

void UIElement::onMouseRelease(const sf::Event& e)
{

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
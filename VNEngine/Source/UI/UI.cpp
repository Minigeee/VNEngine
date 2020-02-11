#include <UI/UI.h>
#include <UI/UIContainer.h>
#include <UI/Button.h>

#include <Core/Math.h>

#include <Engine/Engine.h>

using namespace vne;

// ============================================================================
// ============================================================================

UI::UI(Engine* engine) :
	mEngine				(engine),
	mRootElement		(0),
	mDefaultFont		(0),
	mCurrentFocus		(0),
	mCurrentHover		(0),
	mCurrentPress		(0),
	mCurrentKeyPress	(0)
{
	mRootElement = create<UIContainer>("RootElement");

	memset(mIsKeyPressed, false, sf::Keyboard::KeyCount);

	// Default text highlight colors
	mTextHighlight.setFillColor(sf::Color(120, 180, 255, 100));
}

UI::~UI()
{
	// Remove all animations
	for (Uint32 i = 0; i < mAnimations.size(); ++i)
		delete mAnimations[i];

	Resource<UIContainer>::free();
	Resource<Button>::free();
}

// ============================================================================
// ============================================================================

void UI::addToRoot(UIElement* element)
{
	mRootElement->addChild(element);
}

UIElement* UI::getRoot() const
{
	return mRootElement;
}

void UI::setDefaultFont(sf::Font* font)
{
	mDefaultFont = font;
}

void UI::addAnimation(I_UIAnimation* anim)
{
	anim->reset();
	mAnimations.push_back(anim);
}

sf::Font* UI::getDefaultFont() const
{
	return mDefaultFont;
}

TextCursor& UI::getTextCursor()
{
	return mTextCursor;
}

sf::RectangleShape& UI::getTextHighlight()
{
	return mTextHighlight;
}

// ============================================================================

void UI::setFocus(UIElement* element)
{
	if (element && element != mCurrentFocus)
	{
		// Release all keys
		if (mCurrentKeyPress)
		{
			for (Uint32 i = 0; i < sf::Keyboard::KeyCount; ++i)
			{
				if (mIsKeyPressed[i])
				{
					sf::Event e;
					e.type = sf::Event::KeyReleased;
					e.key.code = (sf::Keyboard::Key)i;

					mCurrentKeyPress->onKeyRelease(e);
					if (mCurrentKeyPress->mKeyReleaseFunc)
						mCurrentKeyPress->mKeyReleaseFunc(mCurrentKeyPress, e);
				}
			}

			mCurrentKeyPress = 0;
		}

		if (mCurrentFocus)
		{
			mCurrentFocus->mHasFocus = false;
			mCurrentFocus->onUnfocus();
			if (mCurrentFocus->mUnfocusFunc)
				mCurrentFocus->mUnfocusFunc(mCurrentFocus);
		}

		mCurrentFocus = element;

		element->mHasFocus = true;
		element->onFocus();
		if (element->mFocusFunc)
			element->mFocusFunc(element);
	}
}

// ============================================================================

bool UI::relayMouseEvent(UIElement* element, const sf::Event& e)
{
	bool handled = false;

	// Relay event to children first
	const std::vector<UIElement*>& children = element->getChildren();
	for (int i = children.size() - 1; i >= 0 && !handled; --i)
		handled |= relayMouseEvent(children[i], e);

	// Quit early if event handled or not visible
	if (handled) return true;
	if (!element->isVisible()) return false;


	// Update transforms
	element->updateAbsTransforms();

	// Get bounding box
	sf::FloatRect box(
		0.0f, 0.0f,
		element->mSize.x,
		element->mSize.y
	);

	// Get local space point
	sf::Vector2f p = element->screenToLocal(sf::Vector2i(e.mouseMove.x, e.mouseMove.y));

	// Get if inside / outside of box
	bool inside = box.contains(p);
	// Make sure it is also inside clipping area if enabled
	if (element->isClipEnabled())
	{
		sf::Vector2f coordSpace = mEngine->getWindow().mapPixelToCoords(sf::Vector2i(e.mouseMove.x, e.mouseMove.y));
		inside &= element->getClipBounds().contains(coordSpace);
	}

	// Test mouse enter
	if (!element->mHasHover && inside)
	{
		// Fire mouse leave event for previous hover element
		if (mCurrentHover)
		{
			mCurrentHover->mHasHover = false;

			mCurrentHover->onMouseExit(e);
			if (mCurrentHover->mMouseExitFunc)
				mCurrentHover->mMouseExitFunc(mCurrentHover, e);
		}

		// Now hovering
		element->mHasHover = true;
		mCurrentHover = element;

		element->onMouseEnter(e);
		if (element->mMouseEnterFunc)
			element->mMouseEnterFunc(element, e);

		// Mark event as handled
		handled = true;
	}

	// Test mouse exit
	else if (element->mHasHover && !inside)
	{
		// Not hovering
		element->mHasHover = false;
		// Reset current hover
		mCurrentHover = 0;

		element->onMouseExit(e);
		if (element->mMouseExitFunc)
			element->mMouseExitFunc(element, e);
	}

	// Mouse move within element
	else if (element->mHasHover && inside)
	{
		UIElement* current = element;

		// Pass event until at root or handled
		// The event isn't necessarily passed to elements directly underneath this element
		// Also not passed to any siblings
		do
		{
			handled = current->onMouseMove(e, p);
			if (current->mMouseMoveFunc)
				current->mMouseMoveFunc(current, e);

			current = current->mParent;

			if (current)
				p = current->screenToLocal(sf::Vector2i(e.mouseMove.x, e.mouseMove.y));

		} while (!handled && current);

		// Mark event as handled
		handled = true;
	}

	return handled;
}

void UI::handleEvent(const sf::Event& e)
{
	// Handle mouse events
	if (e.type == sf::Event::MouseMoved)
	{
		relayMouseEvent(mRootElement, e);

		if(mCurrentPress && mCurrentPress != mCurrentHover)
		{
			// Also give move event to current pressed, if it is not the current hover
			sf::Vector2f p = mCurrentPress->screenToLocal(sf::Vector2i(e.mouseMove.x, e.mouseMove.y));

			mCurrentPress->onMouseMove(e, p);
			if (mCurrentPress->mMouseMoveFunc)
				mCurrentPress->mMouseMoveFunc(mCurrentPress, e);
		}
	}

	// Handle mouse button events
	else if (e.type == sf::Event::MouseButtonPressed)
	{
		/* TODO : Maybe allow multitouch / More than one type of touch */

		if (mCurrentHover && !mCurrentPress)
		{
			UIElement* current = mCurrentHover;
			bool handled = false;

			// Pass event until at root or handled
			// The event isn't necessarily passed to elements directly underneath this element
			// Also not passed to any siblings
			do
			{
				// Update state
				current->mIsMousePressed = true;

				handled = current->onMousePress(e);
				if (current->mMousePressFunc)
					current->mMousePressFunc(current, e);

				if (handled)
				{
					mCurrentPress = current;

					// This element now has focus
					setFocus(current);
				}

				current = current->mParent;
			} while (!handled && current);
		}
	}
	else if (e.type == sf::Event::MouseButtonReleased)
	{
		if (mCurrentPress)
		{
			UIElement* current = mCurrentPress;
			bool handled = false;

			// Pass event until at root or handled
			// The event isn't necessarily passed to elements directly underneath this element
			// Also not passed to any siblings
			do
			{
				// Update state
				current->mIsMousePressed = false;

				handled = current->onMouseRelease(e);
				if (current->mMouseReleaseFunc)
					current->mMouseReleaseFunc(current, e);

				current = current->mParent;
			} while (!handled && current);

			// Reset current pressed
			mCurrentPress = 0;
		}
	}

	else if (e.type == sf::Event::MouseWheelScrolled)
	{
		if (mCurrentHover)
		{
			UIElement* current = mCurrentHover;
			bool handled = false;

			// Pass event until at root or handled
			// The event isn't necessarily passed to elements directly underneath this element
			// Also not passed to any siblings
			do
			{
				handled = current->onMouseScroll(e);
				if (current->mMouseScrollFunc)
					current->mMouseScrollFunc(current, e);

				current = current->mParent;
			} while (!handled && current);
		}
	}

	// Handle key events
	else if (e.type == sf::Event::KeyPressed && e.key.code >= 0)
	{
		// Handle for current focused
		if (mCurrentFocus)
		{
			mCurrentKeyPress = mCurrentFocus;

			// Mark as pressed
			mIsKeyPressed[e.key.code] = true;

			mCurrentFocus->onKeyPress(e);
			if (mCurrentFocus->mKeyPressFunc)
				mCurrentFocus->mKeyPressFunc(mCurrentFocus, e);
		}
	}
	else if (e.type == sf::Event::KeyReleased && e.key.code >= 0)
	{
		if (mCurrentKeyPress)
		{
			// Mark key as released
			mIsKeyPressed[e.key.code] = false;

			mCurrentKeyPress->onKeyRelease(e);
			if (mCurrentKeyPress->mKeyReleaseFunc)
				mCurrentKeyPress->mKeyReleaseFunc(mCurrentKeyPress, e);
		}
	}

	// Handle text input
	else if (e.type == sf::Event::TextEntered)
	{
		if (mCurrentFocus)
		{
			mCurrentFocus->onTextEntered(e);
			if (mCurrentFocus->mTextEnteredFunc)
				mCurrentFocus->mTextEnteredFunc(mCurrentFocus, e);
		}
	}
}

// ============================================================================

void UI::updateElement(UIElement* element, float dt)
{
	element->update(dt);

	for (Uint32 i = 0; i < element->mChildren.size(); ++i)
		updateElement(element->mChildren[i], dt);
}

void UI::update(float dt)
{
	// Make sure root element size matches view size
	const sf::Vector2f& viewSize = mEngine->getWindow().getView().getSize();
	if (mRootElement->getSize() != viewSize)
		mRootElement->setSize(viewSize);

	// Update UI animations
	for (Uint32 i = 0; i < mAnimations.size(); ++i)
	{
		// Update animation
		I_UIAnimation* anim = mAnimations[i];
		anim->update(dt);

		// Remove if finished
		if (anim->isFinished())
		{
			delete anim;
			mAnimations[i--] = mAnimations.back();
			mAnimations.pop_back();
		}
	}

	updateElement(mRootElement, dt);
}

// ============================================================================

void UI::drawElement(UIElement* element, sf::RenderTarget& target, const sf::RenderStates& states) const
{
	// Start clipping if this element has clipping enabled, but not parent
	bool startClipping = element->isClipEnabled() && (!element->getParent() || !element->getParent()->isClipEnabled());
	sf::View prevView = target.getView();

	if (startClipping)
	{
		const sf::FloatRect& bounds = element->getClipBounds();
		const sf::FloatRect& prevViewport = prevView.getViewport();

		// Calculate normalized position
		sf::Vector2f normPos(bounds.left, bounds.top);
		normPos = normPos / prevView.getSize() * prevViewport.getSize() + prevViewport.getPosition();

		// Calculate normalized size
		sf::Vector2f normSize(bounds.width, bounds.height);
		normSize = normSize / prevView.getSize() * prevViewport.getSize();

		// Set new view
		sf::View view(bounds);
		view.setViewport(sf::FloatRect(normPos, normSize));
		target.setView(view);
	}


	// Render element if visible
	if (element->isVisible())
		target.draw(*element, states);

	// Render children elements
	const std::vector<UIElement*>& children = element->getChildren();
	for (Uint32 i = 0; i < children.size(); ++i)
		drawElement(children[i], target, states);


	// Reset view if clipping
	if (startClipping)
		target.setView(prevView);
}

void UI::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// Draw entire tree
	drawElement(mRootElement, target, states);
}

// ============================================================================
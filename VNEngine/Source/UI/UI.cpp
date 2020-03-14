#include <UI/UI.h>
#include <UI/UIContainer.h>
#include <UI/Button.h>

#include <Core/Math.h>

#include <Engine/Engine.h>

#include <SFML/OpenGL.hpp>

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
	Resource<UIContainer>::free();
	Resource<Button>::free();
}

// ============================================================================
// ============================================================================

void UI::init()
{
	mRootElement->setSize(mEngine->getWindow().getView().getSize());
}

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

sf::Font* UI::getDefaultFont()
{
	if (!mDefaultFont)
		mDefaultFont = mEngine->getDefaultFont();

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
		inside &= element->getClipRegion().contains(coordSpace);
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

bool UI::handleEvent(const sf::Event& e)
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

		// Always unhandled because mouse movements should always be visible to other components

		return false;
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

			return handled;
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

			return handled;
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

			return handled;
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

		return (bool)mCurrentFocus;
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

		return (bool)mCurrentKeyPress;
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

		return (bool)mCurrentFocus;
	}

	return false;
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

	updateElement(mRootElement, dt);
}

// ============================================================================

void UI::drawElement(UIElement* element, sf::RenderTarget& target, const sf::RenderStates& states) const
{
	// Start clipping if this element has clipping enabled, but not parent
	bool clipping = element->isClipEnabled() && (!element->getParent() || !element->getParent()->isClipEnabled());

	// Handle region mode
	if (clipping && element->getClipMode() == ClipMode::Region)
	{
		const sf::FloatRect& region = element->getClipRegion();
		const sf::View& view = target.getView();
		const sf::FloatRect& viewport = view.getViewport();

		// Calculate normalized size
		sf::Vector2f pixelSize(region.width, region.height);
		pixelSize = pixelSize / view.getSize() * viewport.getSize();
		pixelSize *= (sf::Vector2f)target.getSize();

		// Calculate pixel position
		sf::Vector2f pixelPos(region.left, region.top);
		pixelPos = pixelPos / view.getSize() * viewport.getSize() + viewport.getPosition();
		pixelPos.y = 1.0f - pixelPos.y;
		pixelPos *= (sf::Vector2f)target.getSize();
		pixelPos.y -= pixelSize.y;

		glEnable(GL_SCISSOR_TEST);
		glScissor((int)pixelPos.x, (int)pixelPos.y, (int)pixelSize.x, (int)pixelSize.y);
	}

	// Handle shapes mode
	else if (clipping && element->getClipMode() == ClipMode::Shapes)
	{
		// Enable stencil
		glEnable(GL_STENCIL_TEST);

		// Clear stencil buffer
		glStencilMask(0xFF);
		glClear(GL_STENCIL_BUFFER_BIT);

		// Stencil functions
		glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);

		// Disable other buffers
		glColorMask(false, false, false, false);
		glDepthMask(false);

		// Draw shapes
		const std::vector<sf::Drawable*>& shapes = element->getClipShapes();
		for (int i = 0; i < shapes.size(); ++i)
			target.draw(*shapes[i]);

		// Reenable other buffers
		glColorMask(true, true, true, true);
		glDepthMask(true);

		// Draw in areas
		glStencilMask(0x00);
		glStencilFunc(GL_EQUAL, 1, 0xFF);
	}


	// Render element if visible
	if (element->isVisible())
		target.draw(*element, states);

	// Render children elements
	const std::vector<UIElement*>& children = element->getChildren();
	for (Uint32 i = 0; i < children.size(); ++i)
		drawElement(children[i], target, states);


	if (clipping && element->getClipMode() == ClipMode::Region)
		glDisable(GL_SCISSOR_TEST);
	else if (clipping && element->getClipMode() == ClipMode::Shapes)
		glDisable(GL_STENCIL_TEST);
}

void UI::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// Draw entire tree
	drawElement(mRootElement, target, states);
}

// ============================================================================
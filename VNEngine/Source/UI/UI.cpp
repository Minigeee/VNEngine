#include <UI/UI.h>
#include <UI/UIContainer.h>
#include <UI/Button.h>

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
	mRootElement = Resource<UIContainer>::create("RootElement");
	memset(mIsKeyPressed, false, sf::Keyboard::KeyCount);
}

UI::~UI()
{
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

sf::Font* UI::getDefaultFont() const
{
	return mDefaultFont;
}

// ============================================================================

bool UI::relayMouseEvent(UIElement* element, const sf::Event& e)
{

	/* TODO : Add handled events so that not every element will need to be checked */
	/* Mouse and key pressing / releasing can be shortened to single check */
	/* Mouse movement events and hovering still need to be checked by every element */

	bool handled = false;

	// Relay event to children first
	const std::vector<UIElement*>& children = element->getChildren();
	for (int i = children.size() - 1; i >= 0 && !handled; --i)
		handled |= relayMouseEvent(children[i], e);

	// Quit early if event handled
	if (handled) return true;


	// Update transforms
	element->updateAbsTransforms();

	/* TODO : Handle rotated elements eventually */
	// Get bounding box
	sf::FloatRect box(
		element->mAbsPosition.x,
		element->mAbsPosition.y,
		element->mSize.x,
		element->mSize.y
	);

	// Get coordinate space point
	sf::Vector2i mousePos(e.mouseMove.x, e.mouseMove.y);
	sf::Vector2f p = mEngine->getWindow().mapPixelToCoords(mousePos);

	// Get inside / outside of box
	bool inside = box.contains(p);

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
		element->onMouseMove(e);
		if (element->mMouseMoveFunc)
			element->mMouseMoveFunc(element, e);

		// Mark event as handled
		handled = true;
	}

	return handled;
}

void UI::handleEvent(const sf::Event& e)
{
	// Handle mouse events
	if (e.type == sf::Event::MouseMoved)
		relayMouseEvent(mRootElement, e);

	// Handle mouse button events
	else if (e.type == sf::Event::MouseButtonPressed)
	{
		/* TODO : Maybe allow multitouch / More than one type of touch */

		if (mCurrentHover && !mCurrentPress)
		{
			// Update state
			mCurrentHover->mIsMousePressed = true;
			mCurrentPress = mCurrentHover;

			// This element now has focus
			mCurrentHover->mHasFocus = true;
			mCurrentFocus = mCurrentHover;

			mCurrentHover->onMousePress(e);
			if (mCurrentHover->mMousePressFunc)
				mCurrentHover->mMousePressFunc(mCurrentHover, e);
		}
	}
	else if (e.type == sf::Event::MouseButtonReleased)
	{
		if (mCurrentPress)
		{
			// Update state
			mCurrentPress->mIsMousePressed = false;

			mCurrentPress->onMouseRelease(e);
			if (mCurrentPress->mMouseReleaseFunc)
				mCurrentPress->mMouseReleaseFunc(mCurrentPress, e);

			// Reset current pressed
			mCurrentPress = 0;
		}
	}

	// Handle key events
	else if (e.type == sf::Event::KeyPressed && !mIsKeyPressed[e.key.code])
	{
		mIsKeyPressed[e.key.code] = true;

		// Handle for current focused
		if (mCurrentFocus && (!mCurrentKeyPress || mCurrentKeyPress == mCurrentFocus))
		{
			// Increment key press counter
			++mCurrentFocus->mNumKeyPressed;
			mCurrentKeyPress = mCurrentFocus;

			mCurrentFocus->onKeyPress(e);
			if (mCurrentFocus->mKeyPressFunc)
				mCurrentFocus->mKeyPressFunc(mCurrentFocus, e);
		}
	}
	else if (e.type == sf::Event::KeyReleased)
	{
		mIsKeyPressed[e.key.code] = false;

		if (mCurrentKeyPress && mCurrentKeyPress->mNumKeyPressed)
		{
			// Decrement key counter
			--mCurrentKeyPress->mNumKeyPressed;

			mCurrentKeyPress->onKeyRelease(e);
			if (mCurrentKeyPress->mKeyReleaseFunc)
				mCurrentKeyPress->mKeyReleaseFunc(mCurrentKeyPress, e);

			// Reset key press element
			if (!mCurrentKeyPress->mNumKeyPressed)
				mCurrentKeyPress = 0;
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

void UI::updateElement(UIElement* element)
{
	element->update();

	for (Uint32 i = 0; i < element->mChildren.size(); ++i)
		updateElement(element->mChildren[i]);
}

void UI::update(float dt)
{
	updateElement(mRootElement);
}

// ============================================================================

void drawElement(UIElement* element, sf::RenderTarget& target, const sf::RenderStates& states)
{
	// Render element
	target.draw(*element, states);

	// Render children elements
	const std::vector<UIElement*>& children = element->getChildren();
	for (Uint32 i = 0; i < children.size(); ++i)
		drawElement(children[i], target, states);
}

void UI::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// Draw entire tree
	drawElement(mRootElement, target, states);
}

// ============================================================================
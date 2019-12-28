#include <UI/UI.h>
#include <UI/UIContainer.h>
#include <UI/Button.h>

#include <Engine/Engine.h>

using namespace vne;

// ============================================================================
// ============================================================================

UI::UI(Engine* engine) :
	mEngine			(engine),
	mRootElement	(0),
	mFocusedElement	(0)
{
	mRootElement = Resource<UIContainer>::create("RootElement");
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

// ============================================================================

void UI::relayEvent(UIElement* element, const sf::Event& e)
{
	/* TODO : Add handled events so that not every element will need to be checked */
	/* Mouse and key pressing / releasing can be shortened to single check */
	/* Mouse movement events and hovering still need to be checked by every element */

	// Relay event to children first
	const std::vector<UIElement*>& children = element->getChildren();
	for (Uint32 i = 0; i < children.size(); ++i)
		relayEvent(children[i], e);


	// Mouse movement events
	if (e.type == sf::Event::MouseMoved)
	{
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
			// Now hovering
			element->mHasHover = true;

			element->onMouseEnter(e);
			if (element->mMouseEnterFunc)
				element->mMouseEnterFunc(element, e);
		}

		// Test mouse exit
		else if (element->mHasHover && !inside)
		{
			// Not hovering
			element->mHasHover = false;

			element->onMouseExit(e);
			if (element->mMouseExitFunc)
				element->mMouseExitFunc(element, e);
		}

		// Test mouse move
		else if (element->mHasHover && inside)
		{
			element->onMouseMove(e);
			if (element->mMouseMoveFunc)
				element->mMouseMoveFunc(element, e);
		}
	}

	// Mouse button events
	else if (e.type == sf::Event::MouseButtonPressed)
	{
		// Test if clicked inside element
		if (element->mHasHover)
		{
			// Increment mouse press
			++element->mNumMousePressed;

			// This element now has focus
			element->mHasFocus = true;
			if (mFocusedElement)
				mFocusedElement->mHasFocus = false;
			mFocusedElement = element;


			element->onMousePress(e);
			if (element->mMousePressFunc)
				element->mMousePressFunc(element, e);
		}
	}
	else if (e.type == sf::Event::MouseButtonReleased)
	{
		// Test if release button is valid
		if (element->mNumMousePressed > 0)
		{
			// Decrement mouse press
			--element->mNumMousePressed;

			element->onMouseRelease(e);
			if (element->mMouseReleaseFunc)
				element->mMouseReleaseFunc(element, e);
		}
	}

	// Key events
	else if (e.type == sf::Event::KeyPressed)
	{
		// Test if element has focus
		if (element->mHasFocus)
		{
			/* TODO : Change num key press to simple boolean */

			// Increment key press
			++element->mNumKeyPressed;

			element->onKeyPress(e);
			if (element->mKeyPressFunc)
				element->mKeyPressFunc(element, e);
		}
	}
	else if (e.type == sf::Event::KeyReleased)
	{
		// Test if key release is valid
		if (element->mNumKeyPressed > 0)
		{
			// Decrement key press
			--element->mNumKeyPressed;

			element->onKeyRelease(e);
			if (element->mKeyReleaseFunc)
				element->mKeyReleaseFunc(element, e);
		}
	}
}

void UI::handleEvent(const sf::Event& e)
{
	relayEvent(mRootElement, e);
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
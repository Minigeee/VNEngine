#ifndef UI_H
#define UI_H

#include <Engine/Resource.h>

#include <UI/UIElement.h>
#include <UI/UIAnimation.h>
#include <UI/TextCursor.h>

#include <SFML/Graphics.hpp>

namespace vne
{

// ============================================================================

class Engine;

class UI : public sf::Drawable
{
public:
	UI(Engine* engine);
	~UI();

	/// <summary>
	/// Create UI element of type T
	/// </summary>
	template <typename T>
	T* create(const sf::String& name)
	{
		T* element = Resource<T>::create(name);
		element->mEngine = mEngine;
		element->setName(name);
		element->onInit(this);
		return element;
	}

	/// <summary>
	/// Initialize a new UI element with name, using src as a template
	/// </summary>
	template <typename T>
	T* copy(const sf::String& name, const sf::String& src)
	{
		T* srcElement = Resource<T>::get(src);
		if (!srcElement) return 0;

		T* element = Resource<T>::create(name);
		*element = *srcElement;
		element->setName(name);
		// Reset parent
		element->mParent = 0;
		element->onInit(this);

		return element;
	}

	/// <summary>
	/// Get UI element of type T by name
	/// </summary>
	template <typename T>
	T* get(const sf::String& name) const { return Resource<T>::get(name); }

	/// <summary>
	/// Add UI element to the root element
	/// </summary>
	/// <param name="element">UI element to add</param>
	void addToRoot(UIElement* element);

	/// <summary>
	/// Add an animation
	/// </summary>
	void addAnimation(I_UIAnimation* anim);

	/// <summary>
	/// Set default font to use for text
	/// </summary>
	/// <param name="font">SFML font</param>
	void setDefaultFont(sf::Font* font);

	/// <summary>
	/// Get root UI element
	/// </summary>
	/// <returns>Root element</returns>
	UIElement* getRoot() const;

	/// <summary>
	/// Get default text font
	/// </summary>
	/// <returns>SFML font</returns>
	sf::Font* getDefaultFont() const;

	/// <summary>
	/// Get the UI text cursor
	/// </summary>
	/// <returns>Text cursor</returns>
	TextCursor& getTextCursor();

	/// <summary>
	/// Get the rectangle that highlights selected text
	/// </summary>
	/// <returns>Text highlight</returns>
	sf::RectangleShape& getTextHighlight();


	/// <summary>
	/// Put an element into focus
	/// </summary>
	/// <param name="element">Element to focus</param>
	void setFocus(UIElement* element);

	/// <summary>
	/// Handle input events
	/// </summary>
	/// <param name="e">SFML event</param>
	void handleEvent(const sf::Event& e);

	/// <summary>
	/// Update UI animations and transforms
	/// </summary>
	/// <param name="dt">Time elapsed since last frame</param>
	void update(float dt);

private:
	/// <summary>
	/// Relay mouse move event to all UI elements
	/// </summary>
	/// <param name="element">Element to relay event to</param>
	/// <param name="e">The event</param>
	/// <returns>True if event has been handled</returns>
	bool relayMouseEvent(UIElement* element, const sf::Event& e);

	/// <summary>
	/// Update element transform
	/// </summary>
	/// <param name="element">Element to update</param>
	void updateElement(UIElement* element, float dt);

	/// <summary>
	/// Draw all UI elements added to the system. Overriding Drawable
	/// </summary>
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	/// <summary>
	/// Access to engine
	/// </summary>
	Engine* mEngine;

	/// <summary>
	/// Default font
	/// </summary>
	sf::Font* mDefaultFont;

	/// <summary>
	/// Root element
	/// </summary>
	UIElement* mRootElement;

	/// <summary>
	/// The blinking text cursor that appears in text inputs
	/// </summary>
	TextCursor mTextCursor;

	/// <summary>
	/// The highlight box that appears when text is selected
	/// </summary>
	sf::RectangleShape mTextHighlight;

	/// <summary>
	/// List of UI animations
	/// </summary>
	std::vector<I_UIAnimation*> mAnimations;

	/// <summary>
	/// Current element with focus, only one element allowed to be focused at a time
	/// </summary>
	UIElement* mCurrentFocus;

	/// <summary>
	/// Current element with hover, only one element allowed to be hovered at a time
	/// </summary>
	UIElement* mCurrentHover;

	/// <summary>
	/// Current element being pressed, only one element allowed
	/// </summary>
	UIElement* mCurrentPress;

	/// <summary>
	/// Current element with key presses, only one allowed
	/// </summary>
	UIElement* mCurrentKeyPress;

	/// <summary>
	/// Manually keep track of keys pressed to prevent repeated keys for key press, but not text
	/// </summary>
	bool mIsKeyPressed[sf::Keyboard::KeyCount];
};

// ============================================================================

}

#endif
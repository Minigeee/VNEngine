#ifndef UI_H
#define UI_H

#include <Engine/Resource.h>

#include <UI/UIElement.h>

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
	T* create(const sf::String& name) const { return Resource<T>::create(name); }

	/// <summary>
	/// Get UI element of type T by name
	/// </summary>
	template <typename T>
	T* get(const sf::String& name) const { return Resource<T>::get(name); }

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

	/// <summary>
	/// Add UI element to the root element
	/// </summary>
	/// <param name="element">UI element to add</param>
	void addToRoot(UIElement* element);

	/// <summary>
	/// Get root UI element
	/// </summary>
	/// <returns>Root element</returns>
	UIElement* getRoot() const;

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
	void updateElement(UIElement* element);

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
	/// Root element
	/// </summary>
	UIElement* mRootElement;

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
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
	/// Add UI element to the root element
	/// </summary>
	/// <param name="element">UI element to add</param>
	void addToRoot(UIElement* element);

	/// <summary>
	/// Get root UI element
	/// </summary>
	/// <returns>Root element</returns>
	UIElement* getRoot() const;

protected:
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
};

// ============================================================================

}

#endif
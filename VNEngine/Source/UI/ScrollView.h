#ifndef SCROLL_VIEW_H
#define SCROLL_VIEW_H

#include <SFML/Graphics.hpp>

#include <UI/UIElement.h>
#include <UI/Slider.h>

namespace vne
{

// ============================================================================

class ScrollView : public UIElement
{
public:
	ScrollView();
	virtual ~ScrollView();

	/// <summary>
	/// Get background of the scroll view.
	/// All color values and outline values are modifiable.
	/// Position, rotation, size are not modifiable.
	/// </summary>
	/// <returns>SFML Rectangle Shape</returns>
	sf::RectangleShape& getBackground();

	/// <summary>
	/// Get background of the scroll view.
	/// </summary>
	/// <returns>SFML Rectangle Shape</returns>
	const sf::RectangleShape& getBackground() const;

	/// <summary>
	/// Get the scroll bar
	/// </summary>
	/// <returns>Slider element</returns>
	Slider* getScrollBar() const;

private:
	/// <summary>
	/// The background of the scroll view
	/// </summary>
	sf::RectangleShape mBackground;

	/// <summary>
	/// Scroll bar
	/// </summary>
	Slider* mScrollBar;
};

// ============================================================================

}

#endif
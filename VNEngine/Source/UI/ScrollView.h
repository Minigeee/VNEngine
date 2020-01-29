#ifndef SCROLL_VIEW_H
#define SCROLL_VIEW_H

#include <SFML/Graphics.hpp>

#include <UI/UIElement.h>
#include <UI/UIContainer.h>
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
	/// Get the scroll bar.
	/// The position, size.x, and scale of the slider are not modifiable.
	/// To set the width of the vertical scrollbar, use scrollBar.setSize(0, width)
	/// because scrollbar is rotated slider.
	/// All color and outline values are modifiable
	/// </summary>
	/// <returns>Slider element</returns>
	Slider* getScrollBar() const;

	/// <summary>
	/// Add a UI element to the scrollable container.
	/// Elements added using this function will be scrollable.
	/// Items added using <code>addChild()</code> will be overlayed over the scroll view
	/// </summary>
	/// <param name="element">UI Element</param>
	void addToView(UIElement* element);

protected:
	/// <summary>
	/// Create container and scroll bar
	/// </summary>
	/// <param name="ui">UI manager</param>
	virtual void onInit(UI* ui) override;

	/// <summary>
	/// Update element orientations
	/// </summary>
	/// <param name="dt">Time elapsed</param>
	virtual void update(float dt) override;

	/// <summary>
	/// Draw scroll view
	/// </summary>
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void onScrollBarMoved(Slider* slider, bool dragged);

	virtual bool onMouseScroll(const sf::Event& e) override;

protected:
	/// <summary>
	/// The background of the scroll view
	/// </summary>
	sf::RectangleShape mBackground;

	/// <summary>
	/// The container where everything inside the scroll view will be
	/// </summary>
	UIContainer* mContainer;

	/// <summary>
	/// Scroll bar
	/// </summary>
	Slider* mScrollBar;

	/// <summary>
	/// Min value of coordinates in view
	/// </summary>
	sf::Vector2f mMinVal;

	/// <summary>
	/// Max value of coordinates in view
	/// </summary>
	sf::Vector2f mMaxVal;

	float mScrollSpeed;
};

// ============================================================================

}

#endif
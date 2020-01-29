#ifndef SCROLL_VIEW_H
#define SCROLL_VIEW_H

#include <SFML/Graphics.hpp>

#include <UI/UIElement.h>
#include <UI/UIContainer.h>
#include <UI/Slider.h>

#include <functional>

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

	/// <summary>
	/// Set the speed the scroll view moves when the mouse wheel is scrolled (Default 50.0f)
	/// </summary>
	/// <param name="speed">Speed in coordinate space units per scroll tick</param>
	void setScrollSpeed(float speed);

	/// <summary>
	/// Set the view position
	/// Positive values scroll down
	/// </summary>
	/// <param name="y">Y-coordinate to scroll to</param>
	void setViewPos(float y);

	/// <summary>
	/// Get scroll speed
	/// </summary>
	/// <returns>Scroll speed in coordinate space units per scroll tick</returns>
	float getScrollSpeed() const;

	/// <summary>
	/// Get view position
	/// </summary>
	/// <returns>Y-coordinate of view</returns>
	float getViewPos() const;

	/// <summary>
	/// Set the function that is called when the scroll view moves, either manually or using setScrollPos()
	/// The first parameter to the function is the scroll view that was scrolled.
	/// The second parameter is a boolean indicating if the view was moved manually
	/// (Using mouse wheel, scroll bar, or dragged), or moved programatically
	/// </summary>
	/// <param name="func">Callback</param>
	void setViewMovedFunc(const std::function<void(ScrollView*, bool)>& func);

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

	virtual bool onMousePress(const sf::Event& e) override;
	virtual bool onMouseRelease(const sf::Event& e) override;
	virtual bool onMouseMove(const sf::Event& e, const sf::Vector2f& p) override;
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

	/// <summary>
	/// The y-coordinate of the previous mouse position.
	/// When the view is not being dragged, this value is negative.
	/// </summary>
	float mPrevMousePos;

	/// <summary>
	/// How much view moves per scroll tick
	/// </summary>
	float mScrollSpeed;

	/// <summary>
	/// Called when the scroll view moves, either manually or using setScrollPos()
	/// </summary>
	std::function<void(ScrollView*, bool)> mViewMovedFunc;
};

// ============================================================================

}

#endif
#ifndef SLIDER_H
#define SLIDER_H

#include <UI/UIElement.h>

#include <SFML/Graphics.hpp>

namespace vne
{

// ============================================================================

class Slider : public UIElement
{
public:
	Slider();
	virtual ~Slider();

	/// <summary>
	/// Get the body of the slider (background).
	/// All color and texture values, and outline values are modifiable.
	/// The position, rotation, and origin are not modifiable.
	/// </summary>
	/// <returns>SFML Rectangle</returns>
	sf::RectangleShape& getBody();

	/// <summary>
	/// Get the body of the slider (background)
	/// </summary>
	/// <returns>SFML Rectangle</returns>
	const sf::RectangleShape& getBody() const;

	/// <summary>
	/// Get the draggable part of the slider.
	/// The width of the rectangle, all color and texture values, and outline values are modifiable.
	/// The position, rotation, height, and origin are not modifiable.
	/// </summary>
	/// <returns>SFML Rectangle</returns>
	sf::RectangleShape& getSlider();

	/// <summary>
	/// Get the draggable part of the slider
	/// </summary>
	/// <returns>SFML Rectangle</returns>
	const sf::RectangleShape& getSlider() const;

	/// <summary>
	/// Set the width of the draggable part of the slider
	/// </summary>
	/// <param name="w">Width in coordinate space</param>
	void setSliderWidth(float w);

	/// <summary>
	/// Set the value of the slider from 0 to 1
	/// </summary>
	/// <param name="value">New value</param>
	void setValue(float value);

	/// <summary>
	/// Get value of the slider from 0 to 1
	/// </summary>
	/// <returns>Value</returns>
	float getValue() const;

	/// <summary>
	/// Set the function that gets called when value changes
	/// </summary>
	/// <param name="func">Callback</param>
	void setValueChangedFunc(const std::function<void(Slider*)>& func);

protected:
	void update(float dt) override;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void onMousePress(const sf::Event& e) override;
	void onMouseRelease(const sf::Event& e) override;
	void onMouseMove(const sf::Event& e, const sf::Vector2f& p) override;

protected:
	/// <summary>
	/// The background body of the slider
	/// </summary>
	sf::RectangleShape mBody;

	/// <summary>
	/// The thing that is dragged allong the line
	/// </summary>
	sf::RectangleShape mSlider;

	/// <summary>
	/// Value of the slider from 0 to 1
	/// </summary>
	float mValue;

	/// <summary>
	/// True if slider is being pressed
	/// </summary>
	bool mSliderPressed;

	/// <summary>
	/// The offset from when slider starts being dragged
	/// </summary>
	float mPressOffset;

	/// <summary>
	/// This function is called when the value of the slider changes
	/// </summary>
	std::function<void(Slider*)> mValueChangedFunc;
};

// ============================================================================

}

#endif
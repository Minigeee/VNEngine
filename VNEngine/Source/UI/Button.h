#ifndef BUTTON_H
#define BUTTON_H

#include <UI/UIElement.h>

namespace vne
{

// ============================================================================

class Button : public UIElement
{
	friend UI;

public:
	Button();
	virtual ~Button();

	/// <summary>
	/// Get rectangle body of button
	/// </summary>
	/// <returns>Rectangle shape</returns>
	sf::RectangleShape& getBody();

	/// <summary>
	/// Get label of button
	/// </summary>
	/// <returns>Button label</returns>
	sf::Text& getLabel();

protected:
	/// <summary>
	/// Update body and label transsforms
	/// </summary>
	virtual void update();

	/// <summary>
	/// Draw button
	/// </summary>
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	/// <summary>
	/// Set default font
	/// </summary>
	/// <param name="ui">UI system</param>
	virtual void onInit(UI* ui) override;

protected:
	/// <summary>
	/// Rectangle body of button
	/// </summary>
	sf::RectangleShape mBody;

	/// <summary>
	/// Label of button
	/// </summary>
	sf::Text mLabel;
};

// ============================================================================

}

#endif
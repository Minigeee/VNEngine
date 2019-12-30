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
	enum State
	{
		Default,
		Hover,
		Press
	};

	typedef std::function<void(Button*, State)> Callback;

public:
	Button();
	virtual ~Button();

	/// <summary>
	/// Get rectangle body of button
	/// </summary>
	/// <returns>Rectangle shape</returns>
	sf::RectangleShape& getBody();

	/// <summary>
	/// Get rectangle body of button
	/// </summary>
	/// <returns>Rectangle shape</returns>
	const sf::RectangleShape& getBody() const;

	/// <summary>
	/// Get label of button
	/// </summary>
	/// <returns>Button label</returns>
	sf::Text& getLabel();

	/// <summary>
	/// Get label of button
	/// </summary>
	/// <returns>Button label</returns>
	const sf::Text& getLabel() const;

	/// <summary>
	/// Get button state
	/// </summary>
	/// <returns>Button state</returns>
	State getState() const;

	/// <summary>
	/// Set default state function.
	/// The function is called right after it is set (to reset default state)
	/// </summary>
	/// <param name="func">This function receives the target button and its previous state as parameters</param>
	void setDefaultStateFunc(Callback func);

	/// <summary>
	/// Set hover state function
	/// </summary>
	/// <param name="func">This function receives the target button and its previous state as parameters</param>
	void setHoverStateFunc(Callback func);

	/// <summary>
	/// Set press state function
	/// </summary>
	/// <param name="func">This function receives the target button and its previous state as parameters</param>
	void setPressStateFunc(Callback func);

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

	virtual void onMouseEnter(const sf::Event& e) override;
	virtual void onMouseExit(const sf::Event& e) override;
	virtual void onMousePress(const sf::Event& e) override;
	virtual void onMouseRelease(const sf::Event& e) override;

protected:
	/// <summary>
	/// Rectangle body of button
	/// </summary>
	sf::RectangleShape mBody;

	/// <summary>
	/// Label of button
	/// </summary>
	sf::Text mLabel;

	/// <summary>
	/// Current button state
	/// </summary>
	State mState;

	/// <summary>
	/// Called when button goes to default state
	/// </summary>
	Callback mDefaultStateFunc;

	/// <summary>
	/// Called when button goes to hover state
	/// </summary>
	Callback mHoverStateFunc;

	/// <summary>
	/// Called when button goes to pressed state
	/// </summary>
	Callback mPressStateFunc;
};

// ============================================================================

}

#endif
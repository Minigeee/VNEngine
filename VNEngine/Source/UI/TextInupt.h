#ifndef TEXT_INPUT_H
#define TEXT_INPUT_H

#include <UI/UIElement.h>

namespace vne
{

// ============================================================================

class TextInput : public UIElement
{
	friend UI;

public:
	TextInput();
	virtual ~TextInput();

	/// <summary>
	/// Get body of text input
	/// </summary>
	/// <returns>SFML Rectangle shape</returns>
	sf::RectangleShape& getBody();

	/// <summary>
	/// Get body of text input
	/// </summary>
	/// <returns>SFML Rectangle shape</returns>
	const sf::RectangleShape& getBody() const;

	/// <summary>
	/// Get text object
	/// </summary>
	/// <returns>SFML Text</returns>
	sf::Text& getText();

	/// <summary>
	/// Get text object
	/// </summary>
	/// <returns>SFML Text</returns>
	const sf::Text& getText() const;

	/// <summary>
	/// Get text cursor object
	/// </summary>
	/// <returns>SFML Rectangle shape</returns>
	sf::RectangleShape& getTextCursor();

	/// <summary>
	/// Get text cursor object
	/// </summary>
	/// <returns>SFML Rectangle shape</returns>
	const sf::RectangleShape& getTextCursor() const;

	/// <summary>
	/// Set left text margin in pixels
	/// </summary>
	/// <param name="offset">The offset</param>
	void setTextOffset(float offset);

	/// <summary>
	/// Set the animation period of the blinking text cursor (in seconds)
	/// </summary>
	/// <param name="period">Period in seconds</param>
	void setCursorAnimPeriod(float period);

protected:
	virtual void update(float dt) override;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	/// <summary>
	/// Set text font
	/// </summary>
	virtual void onInit(UI* ui) override;

	virtual void onMouseEnter(const sf::Event& e) override;
	virtual void onMouseExit(const sf::Event& e) override;
	virtual void onMousePress(const sf::Event& e) override;
	virtual void onMouseRelease(const sf::Event& e) override;
	virtual void onKeyPress(const sf::Event& e) override;
	virtual void onTextEntered(const sf::Event& e) override;

protected:
	/// <summary>
	/// The body of the text input
	/// </summary>
	sf::RectangleShape mBody;

	/// <summary>
	/// The text that is displayed
	/// </summary>
	sf::Text mText;

	/// <summary>
	/// The blinking text cursor
	/// </summary>
	sf::RectangleShape mTextCursor;

	/// <summary>
	/// Left text margin
	/// </summary>
	float mTextOffset;

	/// <summary>
	/// Current time in animation cycle
	/// </summary>
	float mAnimationTime;

	/// <summary>
	/// Period of the text cursor animation in seconds
	/// </summary>
	float mAnimationPeriod;

	int mCursorPos;
	Uint32 mMaxChars;
};

// ============================================================================

}

#endif
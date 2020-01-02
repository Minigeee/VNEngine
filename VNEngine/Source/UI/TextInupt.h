#ifndef TEXT_INPUT_H
#define TEXT_INPUT_H

#include <UI/UIElement.h>

namespace vne
{

// ============================================================================

class TextCursor;

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
	/// Set left text margin in pixels
	/// </summary>
	/// <param name="offset">The offset</param>
	void setTextOffset(float offset);

protected:
	virtual void update(float dt) override;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	/// <summary>
	/// Gets character local position at a certain index
	/// </summary>
	/// <param name="index">Character index</param>
	/// <returns>Character x-position</returns>
	float getCharPos(Uint32 index);

	/// <summary>
	/// Set text font
	/// </summary>
	virtual void onInit(UI* ui) override;

	virtual void onFocus() override;
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
	TextCursor* mTextCursor;

	/// <summary>
	/// Left text margin
	/// </summary>
	float mTextOffset;

	/// <summary>
	/// Cursor index in string
	/// </summary>
	int mCursorIndex;

	/// <summary>
	/// Cursor position
	/// </summary>
	float mCursorPos;
	Uint32 mMaxChars;
};

// ============================================================================

}

#endif
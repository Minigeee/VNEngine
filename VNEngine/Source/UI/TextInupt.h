#ifndef TEXT_INPUT_H
#define TEXT_INPUT_H

#include <UI/UIElement.h>
#include <UI/Text.h>

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
	Text& getText();

	/// <summary>
	/// Get text object
	/// </summary>
	/// <returns>SFML Text</returns>
	const Text& getText() const;

	/// <summary>
	/// Set left text margin in pixels
	/// </summary>
	/// <param name="offset">The offset</param>
	void setTextOffset(float offset);

	/// <summary>
	/// Set the maximum number of characters allowed in input (Default: 30)
	/// </summary>
	/// <param name="max">Max</param>
	void setMaxChars(Uint32 max);

	/// <summary>
	/// Set the submit callback
	/// </summary>
	/// <param name="func">Callback function</param>
	void setSubmitFunc(const std::function<void(TextInput*)>& func);

	/// <summary>
	/// Get left text margin
	/// </summary>
	/// <returns>Offset of text in pixels</returns>
	float getTextOffset() const;

	/// <summary>
	/// Get the maximum number of characters allowed
	/// </summary>
	/// <returns>Max number</returns>
	Uint32 getMaxChars() const;

	/// <summary>
	/// Call the submit callback function
	/// </summary>
	void submit();

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
	virtual void onUnfocus() override;
	virtual void onMouseExit(const sf::Event& e) override;
	virtual bool onMousePress(const sf::Event& e) override;
	virtual bool onMouseMove(const sf::Event& e, const sf::Vector2f& localPos) override;
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
	Text mText;

	/// <summary>
	/// The blinking text cursor
	/// </summary>
	TextCursor* mTextCursor;

	/// <summary>
	/// Highlight that covers selected text
	/// </summary>
	sf::RectangleShape* mTextHighlight;

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

	/// <summary>
	/// Index of where text selection starts
	/// </summary>
	int mSelectIndex;

	/// <summary>
	/// Local position of selection box start
	/// </summary>
	float mSelectPos;

	/// <summary>
	/// The maximum number of characters allowed
	/// </summary>
	Uint32 mMaxChars;

	/// <summary>
	/// The function that is called on submit
	/// </summary>
	std::function<void(TextInput*)> mSubmitFunc;
};

// ============================================================================

}

#endif
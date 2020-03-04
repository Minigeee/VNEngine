#ifndef TEXT_BOX_H
#define TEXT_BOX_H

#include <UI/UIElement.h>
#include <UI/Text.h>

namespace vne
{

// ============================================================================

class TextBox : public UIElement
{
public:
	TextBox();
	virtual ~TextBox();

	/// <summary>
	/// Set the string to display
	/// </summary>
	/// <param name="str">Display string</param>
	void setString(const sf::String& str);

	/// <summary>
	/// Set text font
	/// </summary>
	/// <param name="font">Pointer to font</param>
	void setFont(const sf::Font* font);

	/// <summary>
	/// Set character size in coordinate space unites
	/// </summary>
	/// <param name="size">Character size</param>
	void setCharacterSize(Uint32 size);

	/// <summary>
	/// Set text fill color
	/// </summary>
	/// <param name="color">Color</param>
	void setFillColor(const sf::Color& color);

	/// <summary>
	/// Set text outline color
	/// </summary>
	/// <param name="color">Color</param>
	void setOutlineColor(const sf::Color& color);

	/// <summary>
	/// Set text outline thickness
	/// </summary>
	/// <param name="thickness"></param>
	void setOutlineThickness(float thickness);

	/// <summary>
	/// Set text style
	/// </summary>
	/// <param name="style">Text style flags</param>
	void setStyle(Uint32 style);

	/// <summary>
	/// Set word wrap width in coordinate space units
	/// </summary>
	/// <param name="width">Width to apply word wrap</param>
	void setWordWrap(float width);

	/// <summary>
	/// Get the string being displayed
	/// </summary>
	/// <returns>Display string</returns>
	const sf::String& getString() const;

	/// <summary>
	/// Get text font
	/// </summary>
	/// <returns>Font</returns>
	const sf::Font* getFont() const;

	/// <summary>
	/// Get character size in coordinate space units
	/// </summary>
	/// <returns>Char size</returns>
	Uint32 getCharacterSize() const;

	/// <summary>
	/// Get fill color of text
	/// </summary>
	/// <returns>Color</returns>
	const sf::Color& getFillColor() const;

	/// <summary>
	/// Get outline color of text
	/// </summary>
	/// <returns>Color</returns>
	const sf::Color& getOutlineColor() const;

	/// <summary>
	/// Get outline thickness
	/// </summary>
	/// <returns>Thickness of outline</returns>
	float getOutlineThickness() const;

	/// <summary>
	/// Get text style flags
	/// </summary>
	/// <returns>Style</returns>
	Uint32 getStyle() const;

	/// <summary>
	/// Get the length of a line of text (from newline or beggining to the next newline).
	/// This returns a negative number if the line number is out of bounds
	/// </summary>
	/// <param name="num">The line number</param>
	/// <returns>Length of line of text in coordinate space units</returns>
	float getLineLength(Uint32 num) const;

	/// <summary>
	/// Get the entire list of line lengths in coordinate space units
	/// </summary>
	/// <returns>List of text line lengths</returns>
	const std::vector<float>& getLineLengths() const;

protected:
	void onInit(UI* ui) override;
	void update(float dt) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	/// <summary>
	/// The text will have a certain offset from the actual position, so custom bounds are needed
	/// </summary>
	void updateBounds();

	/// <summary>
	/// Apply string and modify depending on word wrap
	/// </summary>
	/// <param name="str">String to apply word wrap</param>
	void applyString(sf::String str);

protected:
	/// <summary>
	/// Text to display
	/// </summary>
	Text mText;

	/// <summary>
	/// Width of word wrap region
	/// </summary>
	float mWordWrap;

	/// <summary>
	/// List of line lengths
	/// </summary>
	std::vector<float> mLineLengths;
};

// ============================================================================

}

#endif
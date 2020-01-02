#ifndef TEXT_CURSOR_H
#define TEXT_CURSOR_H

#include <SFML/Graphics.hpp>

namespace vne
{

// ============================================================================

class TextInput;

class TextCursor
{
	friend TextInput;

public:
	TextCursor();
	~TextCursor();

	/// <summary>
	/// Set the height relative to the text input box.
	/// Ex: A value of -10 means that the text cursor will be 10px smaller than whatever
	/// text input box it is occupying. (Default: -10)
	/// </summary>
	/// <param name="h">Relative height</param>
	void setRelHeight(float h);

	/// <summary>
	/// Set width of text cursor
	/// </summary>
	/// <param name="w">Width</param>
	void setWidth(float w);

	/// <summary>
	/// Set fill color of cursor
	/// </summary>
	/// <param name="color">Color</param>
	void setColor(const sf::Color& color);

	/// <summary>
	/// Set texture of cursor
	/// </summary>
	/// <param name="texture">Texture</param>
	void setTexture(sf::Texture* texture);

	/// <summary>
	/// Set the period of the blinking animation
	/// (How long it takes for the animation to go through a full cycle)
	/// </summary>
	/// <param name="period">Period</param>
	void setAnimationPeriod(float period);

	/// <summary>
	/// Get relative height of cursor
	/// </summary>
	float getRelHeight() const;

	/// <summary>
	/// Get width of cursor
	/// </summary>
	float getWidth() const;

	/// <summary>
	/// Get color of cursor
	/// </summary>
	const sf::Color& getColor() const;

	/// <summary>
	/// Get period of animation
	/// </summary>
	float getAnimationPeriod() const;

private:
	sf::RectangleShape mBody;

	float mRelHeight;
	float mAnimationTime;
	float mAnimationPeriod;
};

// ============================================================================

}

#endif
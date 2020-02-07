#ifndef VNE_IMAGE_H
#define VNE_IMAGE_H

#include <UI/UIElement.h>

#include <SFML/Graphics.hpp>

namespace vne
{

// ============================================================================

class ImageBox : public UIElement
{
public:
	ImageBox();
	virtual ~ImageBox();

	/// <summary>
	/// Set the texture to display.
	/// This will change the size of the element to be the size of the texture
	/// </summary>
	/// <param name="texture">Texture to display</param>
	void setTexture(const sf::Texture* texture);

	/// <summary>
	/// Set the portion of the texture to display.
	/// If the parameter resize is set to true, this element will be resized to the size of the
	/// sub-rectangle
	/// </summary>
	/// <param name="rect">Sub-rectangle to display</param>
	/// <param name="resize">Should the element be resized to the size of sub-rectangle</param>
	void setTextureRect(const sf::IntRect& rect, bool resize = false);

	/// <summary>
	/// Set the color of the image. This value will be multiplied with each color in the image.
	/// </summary>
	/// <param name="color">Color</param>
	void setColor(const sf::Color& color);

	/// <summary>
	/// Get pointer to texture
	/// </summary>
	/// <returns>SFML Texture</returns>
	const sf::Texture* getTexture() const;

	/// <summary>
	/// Get the sub-rectangle that is being displayed
	/// </summary>
	/// <returns>Int rectangle</returns>
	const sf::IntRect& getTextureRect() const;

	/// <summary>
	/// Get color of image
	/// </summary>
	/// <returns>SFML color</returns>
	const sf::Color& getColor() const;

protected:
	virtual void onInit(UI* ui) override;
	virtual void update(float dt) override;

	/// <summary>
	/// Draw sprite
	/// </summary>
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

protected:
	/// <summary>
	/// Uses sprite internally
	/// </summary>
	sf::Sprite mImage;
};

// ============================================================================

}

#endif
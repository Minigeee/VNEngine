#ifndef UI_ELEMENT_H
#define UI_ELEMENT_H

#include <Core/DataTypes.h>
#include <Core/ObjectPool.h>

#include <SFML/Graphics.hpp>

#include <vector>

namespace vne
{

// ============================================================================

class UIElement : public sf::Drawable
{
public:
	UIElement();
	virtual ~UIElement();

	/// <summary>
	/// Add a child element
	/// </summary>
	/// <param name="child">Pointer to child</param>
	void addChild(UIElement* child);

	/// <summary>
	/// Set local position in coordinate space pixels
	/// </summary>
	void setPosition(const sf::Vector2f& pos);

	/// <summary>
	/// Set local position in coordinate space pixels
	/// </summary>
	/// <param name="pos">Position</param>
	void setPosition(float x, float y);

	/// <summary>
	/// Set rotation in degrees
	/// </summary>
	/// <param name="rot">Rotation</param>
	void setRotation(float rot);

	/// <summary>
	/// Set absolute size in coordinate space pixels
	/// </summary>
	/// <param name="size">Size</param>
	void setSize(const sf::Vector2f& size);

	/// <summary>
	/// Set absolute size in coordinate space pixels
	/// </summary>
	void setSize(float w, float h);

	/// <summary>
	/// Set origin of the element from 0 to 1.
	/// (0, 0) is top-left and (1, 1) is bottom-right
	/// </summary>
	/// <param name="origin">Origin</param>
	void setOrigin(const sf::Vector2f& origin);

	/// <summary>
	/// Set origin of the element from 0 to 1.
	/// (0, 0) is top-left and (1, 1) is bottom-right
	/// </summary>
	void setOrigin(float x, float y);

	/// <summary>
	/// Set anchor position from 0 to 1.
	/// (0, 0) is top-left of parent element and (1, 1) is bottom-right of parent element.
	/// The anchor is considered the center of the coordinate system for this element
	/// </summary>
	/// <param name="anchor">Anchor</param>
	void setAnchor(const sf::Vector2f& anchor);

	/// <summary>
	/// Set anchor position from 0 to 1.
	/// (0, 0) is top-left of parent element and (1, 1) is bottom-right of parent element.
	/// The anchor is considered the center of the coordinate system for this element
	/// </summary>
	void setAnchor(float x, float y);

	/// <summary>
	/// Set texture to display on element
	/// </summary>
	/// <param name="texture"></param>
	void setTexture(sf::Texture* texture);

	/// <summary>
	/// Add offset to current local position
	/// </summary>
	/// <param name="pos">Offset</param>
	void move(const sf::Vector2f& pos);

	/// <summary>
	/// Add offset to current local position
	/// </summary>
	void move(float x, float y);

	/// <summary>
	/// Add offset to rotation in degrees
	/// </summary>
	/// <param name="rot">Rotation</param>
	void rotate(float rot);

	/// <summary>
	/// Multiply size by scale
	/// </summary>
	/// <param name="size">Scale</param>
	void scale(const sf::Vector2f& size);

	/// <summary>
	/// Multiply size by scale
	/// </summary>
	void scale(float w, float h);

	/// <summary>
	/// Get local position in coordinate space pixels
	/// </summary>
	/// <returns>Local position</returns>
	const sf::Vector2f& getRelPosition() const;

	/// <summary>
	/// Get absolute position in coordinate space pixels
	/// </summary>
	/// <returns>Absolute position</returns>
	const sf::Vector2f& getAbsPosition();

	/// <summary>
	/// Get local rotation
	/// </summary>
	/// <returns>Local rotation</returns>
	float getRelRotation() const;

	/// <summary>
	/// Get absolute rotation
	/// </summary>
	/// <returns>Absolute rotation</returns>
	float getAbsRotation();

	/// <summary>
	/// Get absolute size in coordinate space pixels
	/// </summary>
	/// <returns>Absolute size</returns>
	const sf::Vector2f& getSize() const;

	/// <summary>
	/// Get origin
	/// </summary>
	/// <returns>Origin</returns>
	const sf::Vector2f& getOrigin() const;

	/// <summary>
	/// Get anchor
	/// </summary>
	/// <returns>Anchor</returns>
	const sf::Vector2f& getAnchor() const;

	/// <summary>
	/// Get element texture
	/// </summary>
	/// <returns>Texture</returns>
	sf::Texture* getTexture() const;

	/// <summary>
	/// Get pointer to element parent
	/// </summary>
	/// <returns>Element parent</returns>
	UIElement* getParent() const;

	/// <summary>
	/// Get list of children elements
	/// </summary>
	/// <returns>List of children</returns>
	const std::vector<UIElement*>& getChildren() const;

protected:
	/// <summary>
	/// Update any absolute transforms
	/// </summary>
	void updateAbsTransforms();

protected:
	/// <summary>
	/// Parent UI element
	/// </summary>
	UIElement* mParent;

	/// <summary>
	/// Children UI element
	/// </summary>
	std::vector<UIElement*> mChildren;

	/// <summary>
	/// Local position of the element.
	/// This is in coordinate space pixels, not screen pixels
	/// </summary>
	sf::Vector2f mRelPosition;

	/// <summary>
	/// Absolute position of the element.
	/// This is in coordinate space pixels, not screen pixels
	/// </summary>
	sf::Vector2f mAbsPosition;

	/// <summary>
	/// Local rotation of the element.
	/// </summary>
	float mRelRotation;

	/// <summary>
	/// Absolute rotation of the element.
	/// </summary>
	float mAbsRotation;

	/// <summary>
	/// Absolute size of the element.
	/// This is in coordinate space pixels, not screen pixels
	/// </summary>
	sf::Vector2f mSize;

	/// <summary>
	/// Origin of element, from 0 to 1.
	/// 0 is the top or left.
	/// 1 is the bottom or right.
	/// Default origin is top left
	/// </summary>
	sf::Vector2f mOrigin;

	/// <summary>
	/// The "imaginary" origin of the parent element.
	/// This value is where the position of this element is based off of.
	/// It takes values from 0 to 1
	/// </summary>
	sf::Vector2f mAnchor;

	/// <summary>
	/// Texture to display on element
	/// </summary>
	sf::Texture* mTexture;

	/// <summary>
	/// This is true if element's orientation was changed
	/// </summary>
	bool mTransformChanged;
};

// ============================================================================

}

#endif
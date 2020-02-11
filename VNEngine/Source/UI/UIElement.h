#ifndef UI_ELEMENT_H
#define UI_ELEMENT_H

#include <Core/DataTypes.h>
#include <Core/ObjectPool.h>

#include <SFML/Graphics.hpp>

#include <vector>
#include <functional>

namespace vne
{

// ============================================================================

enum class UIPosition
{
	TopLeft,
	TopCenter,
	TopRight,
	Left,
	Center,
	Right,
	BotLeft,
	BotCenter,
	BotRight
};

// ============================================================================

class UI;
class Engine;

class UIElement : public sf::Drawable
{
	friend UI;

public:
	typedef std::function<void(UIElement*, const sf::Event&)> UICallback;

public:
	UIElement();
	virtual ~UIElement();

	UIElement(const UIElement& other);
	UIElement& operator=(const UIElement& other);

	/// <summary>
	/// Initialize UI element
	/// </summary>
	/// <param name="ui">Pointer to UI manager</param>
	/// <param name="name">Name of the element</param>
	void init(UI* ui, const sf::String& name);

	/// <summary>
	/// Set element name
	/// </summary>
	/// <param name="name">Element name string</param>
	void setName(const sf::String& name);

	/// <summary>
	/// Add a child element
	/// </summary>
	/// <param name="child">Pointer to child</param>
	void addChild(UIElement* child);

	/// <summary>
	/// Remove child using pointer to element
	/// </summary>
	/// <param name="child">Pointer to UI element</param>
	void removeChild(UIElement* child);

	/// <summary>
	/// Remove child using name
	/// </summary>
	/// <param name="name">Child element name</param>
	void removeChild(const sf::String& name);

	/// <summary>
	/// Remove all children from current element
	/// </summary>
	void removeAllChildren();

	/// <summary>
	/// Move element to be in front of all other siblings
	/// Sets z-index to max
	/// </summary>
	void moveToFront();

	/// <summary>
	/// Move element to be behind all other siblings
	/// Sets z-index to 0
	/// </summary>
	void moveToBack();

	/// <summary>
	/// Set z-index, which determines the order elements are displayed
	/// 0 is the back, (number of siblings) is front
	/// </summary>
	/// <param name="index">Z-Index</param>
	void setZIndex(Uint32 index);

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
	/// Set the origin using a predefined position
	/// </summary>
	/// <param name="pos">UI position</param>
	void setOrigin(UIPosition pos);

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
	/// Set the anchor using a predefined position
	/// </summary>
	/// <param name="pos">UI position</param>
	void setAnchor(UIPosition pos);

	/// <summary>
	/// Set visibility of element.
	/// Optionally set visibility for all children recursively
	/// </summary>
	/// <param name="visible"></param>
	void setVisible(bool visible, bool recursive = true);

	/// <summary>
	/// Enable or disable clipping for this element and all children
	/// </summary>
	/// <param name="enabled">Boolean for clip state</param>
	void setClipEnabled(bool enabled);

	/// <summary>
	/// Set the clipping boundaries for this element and all children.
	/// This is the area that will be rendered onto the screen.
	/// Any part of the element that is outside this area will be cut off and not rendered.
	/// The rectangle is defined in coordinate space, not pixels.
	/// </summary>
	/// <param name="bounds">Rectangle defining boundary</param>
	void setClipBounds(const sf::FloatRect& bounds);

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
	/// Returns element's global bounds
	/// </summary>
	/// <returns>Bounds of element</returns>
	const sf::FloatRect& getBounds();

	/// <summary>
	/// Returns true if element is visible
	/// </summary>
	/// <returns>Visibility</returns>
	bool isVisible() const;

	/// <summary>
	/// Returns true if clipping is enabled
	/// </summary>
	/// <returns>Clip state</returns>
	bool isClipEnabled() const;

	/// <summary>
	/// Returns clipping boundaries
	/// </summary>
	/// <returns>Clip bounds</returns>
	const sf::FloatRect& getClipBounds() const;

	/// <summary>
	/// Get element name
	/// </summary>
	/// <returns>Name string</returns>
	const sf::String& getName() const;

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

	/// <summary>
	/// Get the z-index
	/// </summary>
	/// <returns></returns>
	Uint32 getZIndex() const;


	/// <summary>
	/// Convert point from engine coordinate space to element local space
	/// </summary>
	/// <param name="p">Coordinate space point</param>
	/// <returns>Local space point</returns>
	sf::Vector2f coordToLocal(const sf::Vector2f& p);

	/// <summary>
	/// Convert point from screen space to element local space
	/// </summary>
	/// <param name="p">Coordinate space point</param>
	/// <returns>Local space point</returns>
	sf::Vector2f screenToLocal(const sf::Vector2i& p);


	/// <summary>
	/// Set callback for focus event
	/// </summary>
	/// <param name="func">Callback</param>
	void setFocusFunc(const std::function<void(UIElement*)>& func);

	/// <summary>
	/// Set callback for unfocus event
	/// </summary>
	/// <param name="func">Callback</param>
	void setUnfocusFunc(const std::function<void(UIElement*)>& func);

	/// <summary>
	/// Set callback for mouse enter event
	/// </summary>
	/// <param name="func">UI callback</param>
	void setMouseEnterFunc(const UICallback& func);

	/// <summary>
	/// Set callback for mouse exit event
	/// </summary>
	/// <param name="func">UI callback</param>
	void setMouseExitFunc(const UICallback& func);

	/// <summary>
	/// Set callback for mouse move event
	/// </summary>
	/// <param name="func">UI callback</param>
	void setMouseMoveFunc(const UICallback& func);

	/// <summary>
	/// Set callback for mouse press event
	/// </summary>
	/// <param name="func">UI callback</param>
	void setMousePressFunc(const UICallback& func);

	/// <summary>
	/// Set callback for mouse release event
	/// </summary>
	/// <param name="func">UI callback</param>
	void setMouseReleaseFunc(const UICallback& func);

	/// <summary>
	/// Set callback for mouse scroll event
	/// </summary>
	/// <param name="func">UI callback</param>
	void setMouseScrollFunc(const UICallback& func);

	/// <summary>
	/// Set callback for key press event
	/// </summary>
	/// <param name="func">UI callback</param>
	void setKeyPressFunc(const UICallback& func);

	/// <summary>
	/// Set callback for key release event
	/// </summary>
	/// <param name="func">UI callback</param>
	void setKeyReleaseFunc(const UICallback& func);

	/// <summary>
	/// Set callback for text entered event
	/// </summary>
	/// <param name="func">UI callback</param>
	void setTextEnteredFunc(const UICallback& func);

	/// <summary>
	/// Returns true if mouse is hovering over element
	/// </summary>
	bool hasHover() const;

	/// <summary>
	/// Returns true if element has focus
	/// </summary>
	bool hasFocus() const;

	/// <summary>
	/// Returns the number of mouse buttons pressed in element
	/// </summary>
	bool isMousePressed() const;

protected:
	/// <summary>
	/// Update SFML drawable transforms
	/// </summary>
	virtual void update(float dt) = 0;

	/// <summary>
	/// Update any absolute transforms
	/// </summary>
	void updateAbsTransforms();

	/// <summary>
	/// Recursively mark transform dirty for all children
	/// </summary>
	void transformDirty();


	/// <summary>
	/// Called right after element is created
	/// </summary>
	/// <param name="ui">UI system</param>
	virtual void onInit(UI* ui);

	/// <summary>
	/// Called when an element gains focus
	/// </summary>
	virtual void onFocus();

	/// <summary>
	/// Called when an element loses focus
	/// </summary>
	virtual void onUnfocus();

	/// <summary>
	/// Called when mouse enters element
	/// </summary>
	/// <param name="e">SFML event</param>
	virtual void onMouseEnter(const sf::Event& e);

	/// <summary>
	/// Called when mouse exits element
	/// </summary>
	/// <param name="e">SFML event</param>
	virtual void onMouseExit(const sf::Event& e);

	/// <summary>
	/// Called when mouse moves within element
	/// </summary>
	/// <param name="e">SFML event</param>
	/// <returns>True if event has been handled</returns>
	virtual bool onMouseMove(const sf::Event& e, const sf::Vector2f& localPos);

	/// <summary>
	/// Called when mouse button is pressed within element
	/// </summary>
	/// <param name="e">SFML event</param>
	/// <returns>True if event has been handled</returns>
	virtual bool onMousePress(const sf::Event& e);

	/// <summary>
	/// Called when mouse button is released
	/// </summary>
	/// <param name="e">SFML event</param>
	/// <returns>True if event has been handled</returns>
	virtual bool onMouseRelease(const sf::Event& e);

	/// <summary>
	/// Called when mouse is scrolled in element
	/// </summary>
	/// <param name="e">SFML event</param>
	/// <returns>True if event has been handled</returns>
	virtual bool onMouseScroll(const sf::Event& e);

	/// <summary>
	/// Called when key is pressed while element has hover or focus
	/// </summary>
	/// <param name="e">SFML event</param>
	virtual void onKeyPress(const sf::Event& e);

	/// <summary>
	/// Called when key is released
	/// </summary>
	/// <param name="e">SFML event</param>
	virtual void onKeyRelease(const sf::Event& e);

	/// <summary>
	/// Called when text is entered into element
	/// </summary>
	/// <param name="e">SFML event</param>
	virtual void onTextEntered(const sf::Event& e);

protected:
	/// <summary>
	/// Access to engine and window
	/// </summary>
	Engine* mEngine;

	/// <summary>
	/// Element name
	/// </summary>
	sf::String mName;

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
	/// Global bounds (includes position, rotation, size)
	/// </summary>
	sf::FloatRect mBounds;

	/// <summary>
	/// True if element is visible
	/// </summary>
	bool mIsVisible;

	/// <summary>
	/// True if element has clipping enabled
	/// </summary>
	bool mClipEnabled;

	/// <summary>
	/// The rectangle defining clip boundaries if it is enabled
	/// </summary>
	sf::FloatRect mClipBounds;

	/// <summary>
	/// This is true if element's orientation was changed
	/// </summary>
	bool mTransformChanged;

	/// <summary>
	/// This is true if transform changed and the transforms have been applied to drawables.
	/// Or the drawable properties (i.e. colors, textures) have changed
	/// </summary>
	bool mDrawablesChanged;


	/// <summary>
	/// Called when element gains focus
	/// </summary>
	std::function<void(UIElement*)> mFocusFunc;

	/// <summary>
	/// Called when element loses focus
	/// </summary>
	std::function<void(UIElement*)> mUnfocusFunc;

	/// <summary>
	/// Called when mouse enters element
	/// </summary>
	UICallback mMouseEnterFunc;

	/// <summary>
	/// Called when mouse exits element
	/// </summary>
	UICallback mMouseExitFunc;

	/// <summary>
	/// Called when mouse moves within element
	/// </summary>
	UICallback mMouseMoveFunc;

	/// <summary>
	/// Called when the mouse button is pressed within the element
	/// </summary>
	UICallback mMousePressFunc;

	/// <summary>
	/// Called when mouse button is released
	/// </summary>
	UICallback mMouseReleaseFunc;

	/// <summary>
	/// Called when mouse wheel is scrolled
	/// </summary>
	UICallback mMouseScrollFunc;

	/// <summary>
	/// Called when key is pressed and element has hover or focus
	/// </summary>
	UICallback mKeyPressFunc;

	/// <summary>
	/// Called when key is released
	/// </summary>
	UICallback mKeyReleaseFunc;

	/// <summary>
	/// Called when text is entered into element
	/// </summary>
	UICallback mTextEnteredFunc;

	/// <summary>
	/// True when element has hover
	/// </summary>
	bool mHasHover;

	/// <summary>
	/// True when element has focus
	/// </summary>
	bool mHasFocus;

	/// <summary>
	/// True if mouse is pressed on element
	/// </summary>
	bool mIsMousePressed;
};

// ============================================================================

}

#endif
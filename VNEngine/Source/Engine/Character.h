#ifndef CHARACTER_H
#define CHARACTER_H

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include <Core/DataTypes.h>

#include <Engine/Action.h>

#include <unordered_map>

namespace vne
{

// ============================================================================

class Scene;

class Character
{
public:
	Character();

	/// <summary>
	/// Initialize character with name
	/// </summary>
	/// <param name="name">Name string</param>
	Character(const sf::String& name);

	/// <summary>
	/// Set the current scene
	/// </summary>
	/// <param name="scene">Scene pointer</param>
	void setScene(Scene* scene);

	/// <summary>
	/// Set the character name
	/// </summary>
	/// <param name="name">Name string</param>
	void setName(const sf::String& name);

	/// <summary>
	/// Add a character image and a corresponding label.
	/// These images can include a subsection of the entire character that gets reused for several different poses.
	/// Ex: "happy" - has a happy image of the character,
	/// "hand_up_r" - an image of just the character's right hand up in the air.
	/// </summary>
	/// <param name="label">Image label</param>
	/// <param name="image">Pointer to the image texture</param>
	void addImage(const sf::String& label, sf::Texture* image);

	/// <summary>
	/// Add a character image by resource name. The texture will be loaded from the resource system and
	/// the image will be given a label that will be the same as its resource name.
	/// These images can include a subsection of the entire character that gets reused for several different poses.
	/// Ex: "happy" - has a happy image of the character,
	/// "hand_up_r" - an image of just the character's right hand up in the air.
	/// </summary>
	/// <param name="resName">Name of the texture as it was assigned in the resource system</param>
	void addImage(const sf::String& resName);

	/// <summary>
	/// Get the characters name
	/// </summary>
	/// <returns>Name string</returns>
	const sf::String& getName() const;

	/// <summary>
	/// Get a character image
	/// </summary>
	/// <param name="label">Label of image to retrieve</param>
	/// <returns>Pointer to image texture</returns>
	sf::Texture* getImage(const sf::String& label) const;

	/// <summary>
	/// Get the image box that is used to display image boxes
	/// </summary>
	/// <returns></returns>
	ImageBox* getImageBox() const;

	/// <summary>
	/// Convenience function to add a dialogue action for character.
	/// This will automatically enclose the dialogue message with quotation symbols
	/// </summary>
	/// <param name="dialogue">Dialogue string</param>
	void say(const sf::String& dialogue);

	/// <summary>
	/// Convenience function to add dialogue action for character.
	/// This will not add qutation symbols to the dialogue, and it will italicize
	/// the dialogue to indicate that the dialogue is happening in the character's head.
	/// </summary>
	/// <param name="dialogue">Dialogue string</param>
	void think(const sf::String& dialogue);

	/// <summary>
	/// Show a character image with a transition effect
	/// </summary>
	/// <param name="image">Name of the character image</param>
	/// <param name="effect">Transition effect</param>
	/// <param name="duration">Duration of the transition effect</param>
	void show(const sf::String& image, Transition effect = Transition::None, float duration = 1.0f);

	/// <summary>
	/// Hide the current character image with a transition effect
	/// </summary>
	/// <param name="effect">Transition effect</param>
	/// <param name="duration">Duration of the transition effect</param>
	void hide(Transition effect = Transition::None, float duration = 1.0f);

	/// <summary>
	/// Transform the character image
	/// </summary>
	/// <param name="pos">New position</param>
	/// <param name="rot">New rotation</param>
	/// <param name="scale">New scale</param>
	/// <param name="duration">Duration of transition in seconds</param>
	void transform(const sf::Vector2f& pos, float rot, float scale, float duration = 0.0f);

	/// <summary>
	/// Move the character image to a new position
	/// </summary>
	/// <param name="pos">New position</param>
	/// <param name="duration">Duration of transition in seconds</param>
	void move(const sf::Vector2f& pos, float duration = 0.0f);

	/// <summary>
	/// Rotate the character image
	/// </summary>
	/// <param name="rot">New rotation</param>
	/// <param name="duration">Duration of transition in seconds</param>
	void rotate(float rot, float duration = 0.0f);

	/// <summary>
	/// Scale the character image
	/// </summary>
	/// <param name="scale">New scale</param>
	/// <param name="duration">Duration of transition in seconds</param>
	void scale(float scale, float duration = 0.0f);

private:
	/// <summary>
	/// Add an action to the actions list or the top action group
	/// </summary>
	/// <param name="action">Action to add</param>
	void addAction(Action* action);

private:
	/// <summary>
	/// Access to current scene for all convenience functions
	/// </summary>
	Scene* mScene;

	/// <summary>
	/// The characters name
	/// </summary>
	sf::String mName;

	/// <summary>
	/// Map of character images
	/// </summary>
	std::unordered_map<std::basic_string<Uint32>, sf::Texture*> mImages;

	/// <summary>
	/// UI element used to display character
	/// </summary>
	ImageBox* mImageBox;
};

// ============================================================================

}

#endif
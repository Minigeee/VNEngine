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
	void show(const sf::String& image, Transition effect = Transition::None);

	/// <summary>
	/// Hide the current character image with a transition effect
	/// </summary>
	/// <param name="effect">Transition effect</param>
	void hide(Transition effect = Transition::None);

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
#ifndef CHARACTER_H
#define CHARACTER_H

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include <Core/DataTypes.h>

#include <unordered_map>

namespace vne
{

// ============================================================================

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

private:
	/// <summary>
	/// The characters name
	/// </summary>
	sf::String mName;

	/// <summary>
	/// Map of character images
	/// </summary>
	std::unordered_map<std::basic_string<Uint32>, sf::Texture*> mImages;
};

// ============================================================================

}

#endif
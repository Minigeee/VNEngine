#ifndef VNE_TEXT_H
#define VNE_TEXT_H

#include <Core/DataTypes.h>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

namespace vne
{

// ============================================================================

/// <summary>
/// Wrapper for sf::Text that scales character size to match desktop resolution.
/// Everything is the same except the scale functions are removed, and setCharacterSize is modified.
/// So for class documentation, check sf::Text documentation
/// </summary>
class Text : public sf::Text
{
public:
	Text();

	/// <summary>
	/// Override sf::Text::setCharacterSize.
	/// This adjusts the actual character size to best fit the current desktop resolution
	/// </summary>
	/// <param name="size">Character size</param>
	void setCharacterSize(Uint32 size);

	/// <summary>
	/// Get the character size
	/// </summary>
	/// <returns>Character size</returns>
	Uint32 getCharacterSize() const;

	/// <summary>
	/// Set the view used as reference to scale character size
	/// </summary>
	/// <param name="view">View pointer</param>
	void setView(const sf::View* view);

private:
	/// <summary>
	/// Active view used to scale character size
	/// </summary>
	const sf::View* mView;

	/// <summary>
	/// Target character size
	/// </summary>
	Uint32 mTargetSize;
};

// ============================================================================

}

#endif
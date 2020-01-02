#ifndef VNE_CURSOR_H
#define VNE_CURSOR_H

#include <SFML/Graphics.hpp>

namespace vne
{

// ============================================================================

class Cursor
{
public:
	static void init(sf::Window* window);

	/// <summary>
	/// Set cursor type to main window
	/// </summary>
	/// <param name="type">Cursor type</param>
	static void setCursor(sf::Cursor::Type type);

private:
	static sf::Window* mWindow;
	static sf::Cursor mCursors[13];
};

// ============================================================================

}

#endif
#include <Engine/Cursor.h>

using namespace vne;

sf::Window* Cursor::mWindow = 0;
sf::Cursor Cursor::mCursors[13];

// ============================================================================

void Cursor::init(sf::Window* window)
{
	mWindow = window;

	for (int i = 0; i < 13; ++i)
		mCursors[i].loadFromSystem((sf::Cursor::Type)i);
}

// ============================================================================

void Cursor::setCursor(sf::Cursor::Type type)
{
	mWindow->setMouseCursor(mCursors[type]);
}

// ============================================================================
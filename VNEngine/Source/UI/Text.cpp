#include <UI/Text.h>

#include <Core/Math.h>

using namespace vne;

// ============================================================================

Text::Text() :
	mView		(0),
	mTargetSize	(30)
{

}

// ============================================================================

void Text::setCharacterSize(Uint32 size)
{
	mTargetSize = size;

	if (mView)
	{
		// Get current desktop video mode
		sf::VideoMode mode = sf::VideoMode::getDesktopMode();
		const sf::Vector2f& viewSize = mView->getSize();

		// Calculate aspect ratios
		float desktopAR = (float)mode.width / mode.height;
		float viewAR = viewSize.x / viewSize.y;

		// Calculate font scale based on AR
		float fontScale = 1.0f;
		if (viewAR > desktopAR)
			fontScale = mode.width / viewSize.x;
		else
			fontScale = mode.height / viewSize.y;

		// Round font size to nearest int
		Uint32 fontSize = (Uint32)std::round(size * fontScale);
		float textScale = (float)size / fontSize;

		// Update metrics
		sf::Text::setCharacterSize(fontSize);
		sf::Text::setScale(textScale, textScale);
	}
	else
	{
		// Don't scale if no view is available
		sf::Text::setCharacterSize(size);
	}
}

Uint32 Text::getCharacterSize() const
{
	return mTargetSize;
}

// ============================================================================

void Text::setView(const sf::View* view)
{
	mView = view;

	// Update character size
	setCharacterSize(mTargetSize);
}

// ============================================================================
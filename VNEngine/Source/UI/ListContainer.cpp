#include <UI/ListContainer.h>

using namespace vne;

// ============================================================================
// ============================================================================

ListContainer::ListContainer() :
	mItemSpacing		(5.0f)
{

}

ListContainer::~ListContainer()
{

}

// ============================================================================
// ============================================================================

void ListContainer::setItemSpacing(float spacing)
{
	mItemSpacing = spacing;
}

// ============================================================================

void ListContainer::addChild(UIElement* element)
{
	const sf::FloatRect& bounds = element->getBounds();

	// Cacluate new size
	sf::Vector2f size(mSize);

	// Update horizontal bounds if needed
	float width = element->getRelPosition().x + bounds.width;
	if (width > size.x)
		size.x = width;

	// Set position of child element
	element->setPosition(element->getRelPosition().x, size.y + mItemSpacing);
	// Force origin to be at top-left
	element->setOrigin(0.0f, 0.0f);

	// Update vertical bounds
	size.y += mItemSpacing + bounds.height;

	// Set new size
	setSize(size);

	// Add element
	UIContainer::addChild(element);
}

// ============================================================================
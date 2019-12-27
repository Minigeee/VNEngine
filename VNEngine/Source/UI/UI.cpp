#include <UI/UI.h>
#include <UI/UIContainer.h>

using namespace vne;

// ============================================================================
// ============================================================================

UI::UI(Engine* engine) :
	mEngine			(engine),
	mRootElement	(0)
{
	mRootElement = Resource<UIContainer>::create("RootElement");
}

UI::~UI()
{
	Resource<UIContainer>::free();
}

// ============================================================================
// ============================================================================

void UI::addToRoot(UIElement* element)
{
	mRootElement->addChild(element);
}

UIElement* UI::getRoot() const
{
	return mRootElement;
}

// ============================================================================

void drawElement(UIElement* element, sf::RenderTarget& target, const sf::RenderStates& states)
{
	// Render element
	target.draw(*element, states);

	// Render children elements
	const std::vector<UIElement*>& children = element->getChildren();
	for (Uint32 i = 0; i < children.size(); ++i)
		drawElement(children[i], target, states);
}

void UI::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// Draw entire tree
	drawElement(mRootElement, target, states);
}

// ============================================================================
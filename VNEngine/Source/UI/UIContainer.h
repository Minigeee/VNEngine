#ifndef UI_CONTAINER_H
#define UI_CONTAINER_H

#include <UI/UIElement.h>

namespace vne
{

// ============================================================================

class UIContainer : public UIElement
{
public:
	UIContainer() { }
	virtual ~UIContainer() { }

protected:
	/// <summary>
	/// This function does nothing because base container has no renderable component
	/// </summary>
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override { }
};

// ============================================================================

}

#endif
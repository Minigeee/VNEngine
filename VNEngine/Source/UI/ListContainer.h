#ifndef LIST_CONTAINER_H
#define LIST_CONTAINER_H

#include <UI/UIContainer.h>

namespace vne
{

// ============================================================================

class ListContainer : public UIContainer
{
public:
	ListContainer();
	virtual ~ListContainer();

	/// <summary>
	/// Append UI element to the list container vertically.
	/// Note that this forces the child element origin to be at the top-left corner,
	/// and it will only modify the element's y-position, not the x-position
	/// </summary>
	/// <param name="element">Child element</param>
	void addChild(UIElement* element);

	/// <summary>
	/// Set the amount of spacing between each element item
	/// </summary>
	/// <param name="spacing">Space in coordinate space unites</param>
	void setItemSpacing(float spacing);

protected:
	/// <summary>
	/// Amount of space between each item
	/// </summary>
	float mItemSpacing;
};

// ============================================================================

}

#endif
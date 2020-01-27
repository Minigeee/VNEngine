#include <UI/ScrollView.h>

#include <Core/Math.h>
#include <Engine/Engine.h>

#include <UI/UI.h>

using namespace vne;

// ============================================================================

ScrollView::ScrollView() :
	mContainer		(0),
	mScrollBar		(0)
{

}

ScrollView::~ScrollView()
{

}

// ============================================================================
// ============================================================================

void ScrollView::onInit(UI* ui)
{
	mContainer = ui->create<UIContainer>(mName + ".Container");
	mScrollBar = ui->create<Slider>(mName + ".ScrollBar");

	// Default settings
	mScrollBar->setSize(0.0f, 20.0f);
	mScrollBar->setSliderWidth(200.0f);
	mScrollBar->getBody().setFillColor(sf::Color::Transparent);

	// Add as children
	addChild(mContainer);
	addChild(mScrollBar);
}

// ============================================================================

sf::RectangleShape& ScrollView::getBackground()
{
	mDrawablesChanged = true;
	return mBackground;
}

const sf::RectangleShape& ScrollView::getBackground() const
{
	return mBackground;
}

UIContainer* ScrollView::getContainer() const
{
	return mContainer;
}

Slider* ScrollView::getScrollBar() const
{
	return mScrollBar;
}

// ============================================================================

void ScrollView::update(float dt)
{
	if (mDrawablesChanged)
	{
		updateAbsTransforms();

		// Update clip bounds
		mContainer->setClipBounds(sf::FloatRect(mAbsPosition, mSize));

		// Set background position and size
		mBackground.setPosition(mAbsPosition);
		mBackground.setRotation(mAbsRotation);
		mBackground.setSize(mSize);
		mBackground.setOrigin(mOrigin * mSize);

		mScrollBar->setPosition(0.0f, 0.0f);
		mScrollBar->setOrigin(0.0f, 0.0f);
		mScrollBar->setAnchor(1.0f, 0.0f);
		mScrollBar->setRotation(90.0f);
		mScrollBar->setSize(mSize.y, mScrollBar->getSize().y);

		mDrawablesChanged = false;
	}
}

// ============================================================================

void ScrollView::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mBackground);
}

// ============================================================================
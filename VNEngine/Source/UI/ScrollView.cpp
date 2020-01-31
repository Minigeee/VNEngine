#include <UI/ScrollView.h>

#include <Core/Math.h>
#include <Engine/Engine.h>

#include <UI/UI.h>

using namespace vne;

// ============================================================================

ScrollView::ScrollView() :
	mContainer		(0),
	mScrollBar		(0),
	mClipMargins	(5.0f, 5.0f),
	mScrollSpeed	(50.0f),
	mPrevMousePos	(-1.0f),
	mMinVal			(0.0f, 0.0f),
	mMaxVal			(0.0f, 0.0f)
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

	// Bind callbacks
	using namespace std::placeholders;
	mScrollBar->setValueChangedFunc(std::bind(&ScrollView::onScrollBarMoved, this, _1, _2));

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

Slider* ScrollView::getScrollBar() const
{
	return mScrollBar;
}

void ScrollView::addToView(UIElement* element)
{
	mContainer->addChild(element);

	// Update bounding box
	updateAbsTransforms();

	sf::FloatRect bounds = element->getBounds();
	bounds.left -= mAbsPosition.x;
	bounds.top -= mAbsPosition.y;

	if (bounds.left < mMinVal.x)
		mMinVal.x = bounds.left;
	if (bounds.left + bounds.width > mMaxVal.x)
		mMaxVal.x = bounds.left + bounds.width;

	if (bounds.top < mMinVal.y)
		mMinVal.y = bounds.top;
	if (bounds.top + bounds.height > mMaxVal.y)
		mMaxVal.y = bounds.top + bounds.height;
}

// ============================================================================

void ScrollView::setClipMargins(const sf::Vector2f& margins)
{
	mClipMargins = margins;
	mDrawablesChanged = true;
}

void ScrollView::setScrollSpeed(float speed)
{
	mScrollSpeed = speed;
}

void ScrollView::setViewPos(float y)
{
	if (mSize.y < mMaxVal.y - mMinVal.y)
	{
		sf::Vector2f pos = mContainer->getRelPosition();
		pos.y = -y;

		// Clamp value
		if (-pos.y < mMinVal.y)
			pos.y = -mMinVal.y;
		else if (-pos.y > mMaxVal.y - mSize.y)
			pos.y = -mMaxVal.y + mSize.y;

		// Move container
		mContainer->setPosition(pos);
		// Update scrollbar
		mScrollBar->setValue(-(pos.y + mMinVal.y) / (mMaxVal.y - mMinVal.y - mSize.y));

		// Callback
		if (mViewMovedFunc)
			mViewMovedFunc(this, false);

		mDrawablesChanged = true;
	}
}

float ScrollView::getScrollSpeed() const
{
	return mScrollSpeed;
}

float ScrollView::getViewPos() const
{
	return -mContainer->getRelPosition().y;
}

void ScrollView::setViewMovedFunc(const std::function<void(ScrollView*, bool)>& func)
{
	mViewMovedFunc = func;
}

// ============================================================================

void ScrollView::update(float dt)
{
	if (mDrawablesChanged)
	{
		updateAbsTransforms();

		// Update clip bounds
		mContainer->setClipBounds(sf::FloatRect(mAbsPosition + mClipMargins, mSize - 2.0f * mClipMargins));

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

		if (mSize.y > mMaxVal.y - mMinVal.y)
		{
			// Scroll bar shouldn't be visible
			mScrollBar->setVisible(false);
		}
		else
		{
			mScrollBar->setVisible(true);
			// Set the size of the scrollbar slider
			mScrollBar->setSliderWidth(mSize.y * mSize.y / (mMaxVal.y - mMinVal.y));
		}

		mDrawablesChanged = false;
	}
}

// ============================================================================

void ScrollView::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mBackground);
}

// ============================================================================
// ============================================================================

void ScrollView::onScrollBarMoved(Slider* slider, bool dragged)
{
	// Only move scroll view if manually dragged
	if (dragged)
	{
		float value = slider->getValue();

		const sf::Vector2f& p = mContainer->getRelPosition();
		float offset = value * (mMaxVal.y - mSize.y);
		mContainer->setPosition(p.x, -offset);

		// Callback
		if (mViewMovedFunc)
			mViewMovedFunc(this, true);

		mDrawablesChanged = true;
	}
}

// ============================================================================

bool ScrollView::onMousePress(const sf::Event& e)
{
	mPrevMousePos = screenToLocal(sf::Vector2i(e.mouseButton.x, e.mouseButton.y)).y;

	// Event handled
	return true;
}

bool ScrollView::onMouseRelease(const sf::Event& e)
{
	// Reset state
	mPrevMousePos = -1.0f;

	// Event handled
	return true;
}

bool ScrollView::onMouseMove(const sf::Event& e, const sf::Vector2f& p)
{
	// Only move if there is more stuff to see
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && mSize.y < mMaxVal.y - mMinVal.y)
	{
		sf::Vector2f pos = mContainer->getRelPosition();
		pos.y += p.y - mPrevMousePos;

		// Update mouse position
		mPrevMousePos = p.y;

		// Clamp value
		if (-pos.y < mMinVal.y)
			pos.y = -mMinVal.y;
		else if (-pos.y > mMaxVal.y - mSize.y)
			pos.y = -mMaxVal.y + mSize.y;

		// Move container
		mContainer->setPosition(pos);
		// Update scrollbar
		mScrollBar->setValue(-(pos.y + mMinVal.y) / (mMaxVal.y - mMinVal.y - mSize.y));

		// Callback
		if (mViewMovedFunc)
			mViewMovedFunc(this, true);

		mDrawablesChanged = true;
	}

	// Event handled
	return true;
}

// ============================================================================

bool ScrollView::onMouseScroll(const sf::Event& e)
{
	if (mSize.y < mMaxVal.y - mMinVal.y)
	{
		sf::Vector2f pos = mContainer->getRelPosition();
		pos.y += mScrollSpeed * e.mouseWheelScroll.delta;

		// Clamp value
		if (-pos.y < mMinVal.y)
			pos.y = -mMinVal.y;
		else if (-pos.y > mMaxVal.y - mSize.y)
			pos.y = -mMaxVal.y + mSize.y;

		// Move container
		mContainer->setPosition(pos);
		// Update scrollbar
		mScrollBar->setValue(-(pos.y + mMinVal.y) / (mMaxVal.y - mMinVal.y - mSize.y));

		// Callback
		if (mViewMovedFunc)
			mViewMovedFunc(this, true);

		mDrawablesChanged = true;
	}

	// Event handled
	return true;
}

// ============================================================================
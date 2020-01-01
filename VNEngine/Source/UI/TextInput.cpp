#include <UI/TextInupt.h>
#include <UI/UI.h>

#include <Core/Math.h>

using namespace vne;

// ============================================================================
// ============================================================================

TextInput::TextInput() :
	mTextOffset				(10.0f),
	mAnimationTime			(0.0f),
	mAnimationPeriod		(1.0f)
{

}

TextInput::~TextInput()
{

}

// ============================================================================
// ============================================================================

sf::RectangleShape& TextInput::getBody()
{
	mDrawablesChanged = true;
	return mBody;
}

const sf::RectangleShape& TextInput::getBody() const
{
	return mBody;
}

sf::Text& TextInput::getText()
{
	mDrawablesChanged = true;
	return mText;
}

const sf::Text& TextInput::getText() const
{
	return mText;
}

sf::RectangleShape& TextInput::getTextCursor()
{
	mDrawablesChanged = true;
	return mTextCursor;
}

const sf::RectangleShape& TextInput::getTextCursor() const
{
	return mTextCursor;
}

void TextInput::setTextOffset(float offset)
{
	mDrawablesChanged = true;
	mTextOffset = offset;
}

void TextInput::setCursorAnimPeriod(float period)
{
	mAnimationPeriod = period;
}

// ============================================================================

void TextInput::update(float dt)
{
	if (mDrawablesChanged)
	{
		updateAbsTransforms();

		mBody.setPosition(mAbsPosition);
		mBody.setRotation(mAbsRotation);
		mBody.setSize(mSize);
		mBody.setOrigin(mOrigin * mSize);

		const sf::FloatRect& xBounds =
			mText.getFont()->getGlyph(
				L'X',
				mText.getCharacterSize(),
				mText.getStyle() & sf::Text::Bold
			).bounds;
		const sf::FloatRect& box = mText.getLocalBounds();
		sf::Vector2f origin = sf::Vector2f(-mTextOffset, -xBounds.top * 0.5f + box.top - mSize.y * 0.5f);

		mText.setOrigin(origin);
		mText.setPosition(mAbsPosition - mBody.getOrigin());
		mText.setRotation(mAbsRotation);

		// Scale text cursor
		mTextCursor.setOrigin(-mTextOffset, -5.0f);
		mTextCursor.setPosition(mAbsPosition - mBody.getOrigin());
		mTextCursor.setSize(sf::Vector2f(2.0f, mSize.y - 10.0f));
		mTextCursor.setRotation(mAbsRotation);

		mDrawablesChanged = false;
	}

	// Update time
	if (mHasFocus)
	{
		mAnimationTime += dt;
		if (mAnimationTime > mAnimationPeriod)
			mAnimationTime -= mAnimationPeriod;
	}
	else
		mAnimationTime = 0.0f;
}

void TextInput::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mBody);
	target.draw(mText);

	if (mHasFocus)
	{
		if (mAnimationTime < 0.5f * mAnimationPeriod)
			target.draw(mTextCursor);
	}
}

// ============================================================================

void TextInput::onInit(UI* ui)
{
	if (ui->getDefaultFont())
		mText.setFont(*ui->getDefaultFont());
}

void TextInput::onMousePress(const sf::Event& e)
{

}

// ============================================================================
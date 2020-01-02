#include <UI/TextInupt.h>
#include <UI/UI.h>
#include <UI/TextCursor.h>

#include <Core/Math.h>

#include <Engine/Engine.h>
#include <Engine/Cursor.h>

using namespace vne;

// ============================================================================
// ============================================================================

TextInput::TextInput() :
	mTextCursor				(0),
	mTextOffset				(10.0f),
	mCursorIndex			(0),
	mCursorPos				(0.0f)
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

void TextInput::setTextOffset(float offset)
{
	mDrawablesChanged = true;
	mTextOffset = offset;
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
		if (mHasFocus)
		{
			mTextCursor->mBody.setOrigin(-mTextOffset - mCursorPos, -5.0f);
			mTextCursor->mBody.setPosition(mAbsPosition - mBody.getOrigin());
			mTextCursor->mBody.setSize(sf::Vector2f(mTextCursor->mBody.getSize().x, mSize.y + mTextCursor->mRelHeight));
			mTextCursor->mBody.setRotation(mAbsRotation);
		}

		mDrawablesChanged = false;
	}

	// Update time
	if (mHasFocus)
	{
		mTextCursor->mAnimationTime += dt;
		if (mTextCursor->mAnimationTime > mTextCursor->mAnimationPeriod)
			mTextCursor->mAnimationTime -= mTextCursor->mAnimationPeriod;
	}

	if (mHasHover)
		Cursor::setCursor(sf::Cursor::Text);
}

void TextInput::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mBody);
	target.draw(mText);

	if (mHasFocus)
	{
		if (mTextCursor->mAnimationTime < 0.5f * mTextCursor->mAnimationPeriod)
			target.draw(mTextCursor->mBody);
	}
}

// ============================================================================

void TextInput::onInit(UI* ui)
{
	if (ui->getDefaultFont())
		mText.setFont(*ui->getDefaultFont());

	mTextCursor = &ui->getTextCursor();
}

void TextInput::onFocus()
{
	// To update text cursor
	mDrawablesChanged = true;
}

void TextInput::onMouseExit(const sf::Event& e)
{
	Cursor::setCursor(sf::Cursor::Arrow);
}

void TextInput::onMousePress(const sf::Event& e)
{
	// Make sure transforms are updated
	updateAbsTransforms();

	// Get coordinate space point
	sf::Vector2i mousePos(e.mouseButton.x, e.mouseButton.y);
	sf::Vector2f p = mEngine->getWindow().mapPixelToCoords(mousePos);
	// Adjust for translation
	p -= mAbsPosition;
	// Adjust for rotation
	float angle = -toRadians(mAbsRotation);
	float ca = cos(angle);
	float sa = sin(angle);
	// Position relative to top-left
	p = sf::Vector2f(p.x * ca - p.y * sa, p.x * sa + p.y * ca) + mBody.getOrigin();
	p.x -= mTextOffset;

	const sf::String& textStr = mText.getString();
	const sf::Font* font = mText.getFont();
	Uint32 charSize = mText.getCharacterSize();
	bool isBold = mText.getStyle() & sf::Text::Bold;

	float charPos = 0.0f;

	// Get character positions
	Uint32 i;
	for (i = 0; i < textStr.getSize(); ++i)
	{
		float advance = font->getGlyph(textStr[i], charSize, isBold).advance;
		float localPos = p.x - charPos;

		// If this position is correct, break
		if (localPos < 0.5f * advance) break;

		charPos += advance;
	}

	mCursorIndex = i;
	mCursorPos = charPos;

	// Needs update
	mTextCursor->mAnimationTime = 0.0f;
	mDrawablesChanged = true;
}

void TextInput::onMouseRelease(const sf::Event& e)
{

}

void TextInput::onKeyPress(const sf::Event& e)
{
	if (e.key.code == sf::Keyboard::Left)
	{
		mCursorIndex = std::max(0, mCursorIndex - 1);
		mCursorPos = getCharPos(mCursorIndex);
		mTextCursor->mAnimationTime = 0.0f;
		mDrawablesChanged = true;
	}
	else if (e.key.code == sf::Keyboard::Right)
	{
		mCursorIndex = std::min((int)mText.getString().getSize(), mCursorIndex + 1);
		mCursorPos = getCharPos(mCursorIndex);
		mTextCursor->mAnimationTime = 0.0f;
		mDrawablesChanged = true;
	}
	else if (e.key.code == sf::Keyboard::Delete)
	{
		const sf::String& oldStr = mText.getString();

		if (mCursorIndex < oldStr.getSize())
		{
			sf::String newStr = oldStr.substring(0, mCursorIndex) + oldStr.substring(mCursorIndex + 1);
			mText.setString(newStr);

			mTextCursor->mAnimationTime = 0.0f;
			mDrawablesChanged = true;
		}
	}
}

void TextInput::onTextEntered(const sf::Event& e)
{
	const sf::String& oldStr = mText.getString();

	if (e.text.unicode == '\b')
	{
		if (mCursorIndex)
		{
			sf::String newStr = oldStr.substring(0, mCursorIndex - 1) + oldStr.substring(mCursorIndex);
			mText.setString(newStr);

			mCursorPos = getCharPos(--mCursorIndex);

			mTextCursor->mAnimationTime = 0.0f;
			mDrawablesChanged = true;
		}
	}
	else
	{
		sf::String newStr = oldStr.substring(0, mCursorIndex) + e.text.unicode + oldStr.substring(mCursorIndex);
		mText.setString(newStr);

		// Increment cursor position
		mCursorPos = getCharPos(++mCursorIndex);

		mTextCursor->mAnimationTime = 0.0f;
		mDrawablesChanged = true;
	}
}

// ============================================================================

float TextInput::getCharPos(Uint32 index)
{
	const sf::String& textStr = mText.getString();
	const sf::Font* font = mText.getFont();
	Uint32 charSize = mText.getCharacterSize();
	bool isBold = mText.getStyle() & sf::Text::Bold;

	float charPos = 0.0f;

	for (Uint32 i = 0; i < index; ++i)
		charPos += font->getGlyph(textStr[i], charSize, isBold).advance;

	return charPos;
}

// ============================================================================
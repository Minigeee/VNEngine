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
	mTextHighlight			(0),
	mTextOffset				(10.0f),
	mCursorIndex			(0),
	mSelectIndex			(-1),
	mCursorPos				(0.0f),
	mMaxChars				(30)
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

void TextInput::setMaxChars(Uint32 max)
{
	mMaxChars = max;
}

void TextInput::setSubmitFunc(const std::function<void(TextInput*)>& func)
{
	mSubmitFunc = func;
}

float TextInput::getTextOffset() const
{
	return mTextOffset;
}

Uint32 TextInput::getMaxChars() const
{
	return mMaxChars;
}

void TextInput::submit()
{
	if (mSubmitFunc)
		mSubmitFunc(this);
}

// ============================================================================

void TextInput::update(float dt)
{
	if (mDrawablesChanged)
	{
		updateAbsTransforms();

		// Make sure string length is less than max
		if (mText.getString().getSize() > mMaxChars)
			mText.setString(mText.getString().substring(0, mMaxChars));

		mBody.setPosition(mAbsPosition);
		mBody.setRotation(mAbsRotation);
		mBody.setSize(mSize);
		mBody.setOrigin(mOrigin * mSize);

		float charSize = (float)mText.getCharacterSize();
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

		// Setup text cursor
		if (mHasFocus)
		{
			mTextCursor->mBody.setOrigin(-mTextOffset - mCursorPos, 0.5f * mTextCursor->mRelHeight);
			mTextCursor->mBody.setPosition(mAbsPosition - mBody.getOrigin());
			mTextCursor->mBody.setSize(sf::Vector2f(mTextCursor->mBody.getSize().x, mSize.y + mTextCursor->mRelHeight));
			mTextCursor->mBody.setRotation(mAbsRotation);
		}

		// Setup text highlight
		if (mSelectIndex >= 0)
		{
			float boxStart = mCursorPos < mSelectPos ? mCursorPos : mSelectPos;
			float boxEnd = mCursorPos > mSelectPos ? mCursorPos : mSelectPos;

			mTextHighlight->setOrigin(-mTextOffset - boxStart, 0.5f * mTextCursor->mRelHeight);
			mTextHighlight->setPosition(mAbsPosition - mBody.getOrigin());
			mTextHighlight->setSize(sf::Vector2f(boxEnd - boxStart, mSize.y + mTextCursor->mRelHeight));
			mTextHighlight->setRotation(mAbsRotation);
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

	if (mSelectIndex >= 0)
		target.draw(*mTextHighlight);
}

// ============================================================================

void TextInput::onInit(UI* ui)
{
	if (ui->getDefaultFont())
		mText.setFont(*ui->getDefaultFont());

	mTextCursor = &ui->getTextCursor();
	mTextHighlight = &ui->getTextHighlight();
}

void TextInput::onFocus()
{
	// To update text cursor
	mDrawablesChanged = true;
}

void TextInput::onUnfocus()
{
	// Remove any selections
	mSelectIndex = -1;
}

void TextInput::onMouseExit(const sf::Event& e)
{
	Cursor::setCursor(sf::Cursor::Arrow);
}

void TextInput::onMousePress(const sf::Event& e)
{
	// Reset selection
	mSelectIndex = -1;

	// Make sure transforms are updated
	updateAbsTransforms();

	// Get local space point
	sf::Vector2f p = screenToLocal(sf::Vector2i(e.mouseButton.x, e.mouseButton.y));
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

void TextInput::onMouseMove(const sf::Event& e, const sf::Vector2f& p)
{
	if (mIsMousePressed)
	{
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
			if (localPos < advance) break;

			charPos += advance;
		}

		if (i != mCursorIndex)
		{
			// Create highlight box
			if (mSelectIndex < 0)
			{
				mSelectIndex = mCursorIndex;
				mSelectPos = mCursorPos;
			}

			// Update cursor position
			mCursorIndex = i;
			mCursorPos = charPos;

			mTextCursor->mAnimationTime = 0.0f;
			mDrawablesChanged = true;
		}
	}
}

void TextInput::onKeyPress(const sf::Event& e)
{
	if (e.key.shift)
	{
		if (e.key.code == sf::Keyboard::Left)
		{
			if (mSelectIndex < 0)
			{
				mSelectIndex = mCursorIndex;
				mSelectPos = getCharPos(mSelectIndex);
			}

			mCursorIndex = std::max(0, mCursorIndex - 1);
			mCursorPos = getCharPos(mCursorIndex);
			mTextCursor->mAnimationTime = 0.0f;
			mDrawablesChanged = true;
		}
		else if (e.key.code == sf::Keyboard::Right)
		{
			if (mSelectIndex < 0)
			{
				mSelectIndex = mCursorIndex;
				mSelectPos = getCharPos(mSelectIndex);
			}

			mCursorIndex = std::min((int)mText.getString().getSize(), mCursorIndex + 1);
			mCursorPos = getCharPos(mCursorIndex);
			mTextCursor->mAnimationTime = 0.0f;
			mDrawablesChanged = true;
		}
	}
	else if (e.key.control)
	{
		if (e.key.code == sf::Keyboard::C)
		{
			int selectStart = mCursorIndex < mSelectIndex ? mCursorIndex : mSelectIndex;
			int selectEnd = mCursorIndex > mSelectIndex ? mCursorIndex : mSelectIndex;
			if (selectStart < 0)
				selectStart = selectEnd;

			// Copy selection
			sf::Clipboard::setString(mText.getString().substring(selectStart, selectEnd));

			mSelectIndex = -1;
		}
		else if (e.key.code == sf::Keyboard::V)
		{
			int selectStart = mCursorIndex < mSelectIndex ? mCursorIndex : mSelectIndex;
			int selectEnd = mCursorIndex > mSelectIndex ? mCursorIndex : mSelectIndex;
			if (selectStart < 0)
				selectStart = selectEnd;

			const sf::String& oldStr = mText.getString();
			sf::String newStr =
				oldStr.substring(0, selectStart) +
				sf::Clipboard::getString() +
				oldStr.substring(selectEnd);
			mText.setString(newStr);

			mCursorIndex = selectStart + sf::Clipboard::getString().getSize();
			mCursorPos = getCharPos(mCursorIndex);
			mTextCursor->mAnimationTime = 0.0f;
			mDrawablesChanged = true;

			mSelectIndex = -1;
		}
		else if (e.key.code == sf::Keyboard::X)
		{
			int selectStart = mCursorIndex < mSelectIndex ? mCursorIndex : mSelectIndex;
			int selectEnd = mCursorIndex > mSelectIndex ? mCursorIndex : mSelectIndex;
			if (selectStart < 0)
				selectStart = selectEnd;

			sf::Clipboard::setString(mText.getString().substring(selectStart, selectEnd));
			const sf::String& oldStr = mText.getString();
			mText.setString(oldStr.substring(0, selectStart) + oldStr.substring(selectEnd));

			mCursorIndex = selectStart;
			mCursorPos = getCharPos(mCursorIndex);
			mTextCursor->mAnimationTime = 0.0f;
			mDrawablesChanged = true;

			mSelectIndex = -1;
		}
	}
	else
	{
		if (e.key.code == sf::Keyboard::Left)
		{
			// Make sure selection is disabled
			mSelectIndex = -1;

			mCursorIndex = std::max(0, mCursorIndex - 1);
			mCursorPos = getCharPos(mCursorIndex);
			mTextCursor->mAnimationTime = 0.0f;
			mDrawablesChanged = true;
		}
		else if (e.key.code == sf::Keyboard::Right)
		{
			// Make sure selection is disabled
			mSelectIndex = -1;

			mCursorIndex = std::min((int)mText.getString().getSize(), mCursorIndex + 1);
			mCursorPos = getCharPos(mCursorIndex);
			mTextCursor->mAnimationTime = 0.0f;
			mDrawablesChanged = true;
		}
		else if (e.key.code == sf::Keyboard::Backspace)
		{
			if (mCursorIndex || (mCursorIndex == 0 && mSelectIndex > 0))
			{
				const sf::String& oldStr = mText.getString();

				if (mSelectIndex >= 0)
				{
					int selectStart = mCursorIndex < mSelectIndex ? mCursorIndex : mSelectIndex;
					int selectEnd = mCursorIndex > mSelectIndex ? mCursorIndex : mSelectIndex;

					sf::String newStr = oldStr.substring(0, selectStart) + oldStr.substring(selectEnd);
					mText.setString(newStr);
					mCursorIndex = selectStart;
					mCursorPos = getCharPos(mCursorIndex);

					// Reset selection box
					mSelectIndex = -1;
				}
				else
				{
					sf::String newStr = oldStr.substring(0, mCursorIndex - 1) + oldStr.substring(mCursorIndex);
					mText.setString(newStr);
					mCursorPos = getCharPos(--mCursorIndex);
				}

				mTextCursor->mAnimationTime = 0.0f;
				mDrawablesChanged = true;
			}
		}
		else if (e.key.code == sf::Keyboard::Delete)
		{
			const sf::String& oldStr = mText.getString();

			if (mCursorIndex < oldStr.getSize())
			{
				if (mSelectIndex >= 0)
				{
					int selectStart = mCursorIndex < mSelectIndex ? mCursorIndex : mSelectIndex;
					int selectEnd = mCursorIndex > mSelectIndex ? mCursorIndex : mSelectIndex;

					sf::String newStr = oldStr.substring(0, selectStart) + oldStr.substring(selectEnd);
					mText.setString(newStr);
					mCursorIndex = selectStart;
					mCursorPos = getCharPos(mCursorIndex);

					// Reset selection box
					mSelectIndex = -1;
				}
				else
				{
					sf::String newStr = oldStr.substring(0, mCursorIndex) + oldStr.substring(mCursorIndex + 1);
					mText.setString(newStr);
				}

				mTextCursor->mAnimationTime = 0.0f;
				mDrawablesChanged = true;
			}
		}
		else if (e.key.code == sf::Keyboard::Enter)
		{
			if (mSubmitFunc)
				mSubmitFunc(this);
		}
	}
}

void TextInput::onTextEntered(const sf::Event& e)
{
	const sf::String& oldStr = mText.getString();

	// Make sure size doesn't go over max
	if (oldStr.getSize() == mMaxChars) return;

	if (e.text.unicode >= 32 && e.text.unicode != 127 && e.text.unicode != '\n')
	{
		if (mSelectIndex >= 0)
		{
			int selectStart = mCursorIndex < mSelectIndex ? mCursorIndex : mSelectIndex;
			int selectEnd = mCursorIndex > mSelectIndex ? mCursorIndex : mSelectIndex;

			sf::String newStr =	oldStr.substring(0, selectStart) + e.text.unicode + oldStr.substring(selectEnd);
			mText.setString(newStr);

			// Update cursor position
			mCursorIndex = selectStart + 1;
			mCursorPos = getCharPos(mCursorIndex);

			// Remove selection
			mSelectIndex = -1;
		}
		else
		{
			sf::String newStr = oldStr.substring(0, mCursorIndex) + e.text.unicode + oldStr.substring(mCursorIndex);
			mText.setString(newStr);

			// Increment cursor position
			mCursorPos = getCharPos(++mCursorIndex);
		}

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
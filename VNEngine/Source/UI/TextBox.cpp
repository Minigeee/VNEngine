#include <UI/TextBox.h>
#include <UI/UI.h>

#include <Core/Math.h>

#include <Engine/Engine.h>

using namespace vne;

// ============================================================================
// ============================================================================

TextBox::TextBox() :
	mWordWrap			(-1.0f)
{

}

TextBox::~TextBox()
{

}

// ============================================================================
// ============================================================================

void TextBox::setString(const sf::String& str)
{
	applyString(str);
}

void TextBox::setFont(const sf::Font* font)
{
	mText.setFont(*font);
	applyString(mText.getString());
}

void TextBox::setCharacterSize(Uint32 size)
{
	mText.setCharacterSize(size);
	applyString(mText.getString());
}

void TextBox::setFillColor(const sf::Color& c)
{
	mText.setFillColor(c);
}

void TextBox::setOutlineColor(const sf::Color& c)
{
	mText.setOutlineColor(c);
}

void TextBox::setOutlineThickness(float thickness)
{
	mText.setOutlineThickness(thickness);
	applyString(mText.getString());
}

void TextBox::setStyle(Uint32 style)
{
	mText.setStyle(style);
	applyString(mText.getString());
}

void TextBox::setWordWrap(float width)
{
	mWordWrap = width;
	applyString(mText.getString());
}

// ============================================================================

const sf::String& TextBox::getString() const
{
	return mText.getString();
}

const sf::Font* TextBox::getFont() const
{
	return mText.getFont();
}

Uint32 TextBox::getCharacterSize() const
{
	return mText.getCharacterSize();
}

const sf::Color& TextBox::getFillColor() const
{
	return mText.getFillColor();
}

const sf::Color& TextBox::getOutlineColor() const
{
	return mText.getOutlineColor();
}

float TextBox::getOutlineThickness() const
{
	return mText.getOutlineThickness();
}

Uint32 TextBox::getStyle() const
{
	return mText.getStyle();
}

// ============================================================================

void TextBox::applyString(sf::String str)
{
	// Add newlines if word wrap is enabled
	if (mWordWrap > 0.0f)
	{
		const sf::Font* font = mText.getFont();
		Uint32 characterSize = mText.getCharacterSize();
		bool bold = mText.getStyle() & sf::Text::Bold;
		float outlineThickness = mText.getOutlineThickness();
		float scale = mText.getScale().x;

		// Keep track of current local x
		float x = 0.0f;

		for (Uint32 i = 0; i < str.getSize(); ++i)
		{
			Uint32 c = str[i];
			const sf::Glyph& glyph = font->getGlyph(c, characterSize, bold, outlineThickness);

			if (c == L'\n')
				x = 0.0f;
			else
				x += glyph.advance * scale;

			// Once at the end of a word, and if length of line is over word wrap, the find previous space
			if ((c == L' ' || i == str.getSize() - 1) && x > mWordWrap)
			{
				int j = i - 1;
				while (j >= 0 && str[j] != L' ') --j;

				if (j < 0)
				{
					if (i < str.getSize() - 1)
						// If couldn't find a space, then set the current space to a newline and continue
						str[i] = L'\n';
				}
				else
				{
					// Set previous space to new line, and reset iterator to this word
					str[j] = L'\n';
					i = j;
				}

				// Reset line
				x = 0.0f;
			}
		}
	}

	// Set new string
	mText.setString(str);

	// Update size and bounds
	sf::FloatRect bounds = mText.getLocalBounds();
	const sf::Vector2f& s = mText.getScale();

	mSize.x = (bounds.left + bounds.width) * s.x;
	mSize.y = (bounds.top + bounds.height) * s.y;

	transformDirty();
}

// ============================================================================
// ============================================================================

void TextBox::onInit(UI* ui)
{
	if (ui->getDefaultFont())
		mText.setFont(*ui->getDefaultFont());
	mText.setView(&mEngine->getWindow().getView());
}

// ============================================================================

void TextBox::update(float dt)
{
	if (mDrawablesChanged)
	{
		updateAbsTransforms();

		mText.setPosition(mAbsPosition);
		mText.setRotation(mAbsRotation);
		mText.setOrigin(mOrigin * mSize);

		mDrawablesChanged = false;
	}
}

// ============================================================================

void TextBox::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mText);
}

// ============================================================================
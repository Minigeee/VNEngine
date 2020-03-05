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
	if (font != mText.getFont() && mText.getString().getSize())
	{
		mText.setFont(*font);
		applyString(mText.getString());
	}
}

void TextBox::setCharacterSize(Uint32 size)
{
	if (size != mText.getCharacterSize() && mText.getString().getSize())
	{
		mText.setCharacterSize(size);
		applyString(mText.getString());
	}
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
	if (thickness != mText.getOutlineThickness() && mText.getString().getSize())
	{
		mText.setOutlineThickness(thickness);
		applyString(mText.getString());
	}
}

void TextBox::setStyle(Uint32 style)
{
	if (style != mText.getStyle() && mText.getString().getSize())
	{
		mText.setStyle(style);
		applyString(mText.getString());
	}
}

void TextBox::setWordWrap(float width)
{
	if (width != mWordWrap && mText.getString().getSize())
	{
		mWordWrap = width;
		applyString(mText.getString());
	}
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

float TextBox::getLineLength(Uint32 num) const
{
	return num >= mLineLengths.size() ? -1.0f : mLineLengths[num];
}

const std::vector<float>& TextBox::getLineLengths() const
{
	return mLineLengths;
}

// ============================================================================

void TextBox::applyString(sf::String str)
{
	// Clear list of line lengths
	mLineLengths.clear();

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

			x += glyph.advance * scale;

			if (c == L'\n')
			{
				// Record line length
				mLineLengths.push_back(x);
				// Reset x
				x = 0.0f;
			}

			// Once at the end of a word, and if length of line is over word wrap, the find previous space
			if ((c == L' ' || i == str.getSize() - 1) && x > mWordWrap)
			{
				int j = i - 1;
				while (j >= 0 && str[j] != L' ') --j;

				if (j < 0)
				{
					if (i < str.getSize() - 1)
					{
						// If couldn't find a space, then set the current space to a newline and continue
						str[i] = L'\n';

						mLineLengths.push_back(x);
					}
				}
				else
				{
					// Set previous space to new line, and reset iterator to this word
					str[j] = L'\n';
					i = j;

					mLineLengths.push_back(x);
				}

				// Reset line
				x = 0.0f;
			}
		}

		// Add final line length
		mLineLengths.push_back(x);
	}
	else
	{
		// If word wrap is off, go through string and search for new lines
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

			x += glyph.advance * scale;

			if (c == L'\n')
			{
				// Record line length
				mLineLengths.push_back(x);
				// Reset x
				x = 0.0f;
			}
		}

		// Add final line length
		mLineLengths.push_back(x);
	}

	// Add outline thickness to lin lengths
	for (Uint32 i = 0; i < mLineLengths.size(); ++i)
		mLineLengths[i] += 2.0f * getOutlineThickness();

	// Set new string
	mText.setString(str);

	// Update size and bounds
	sf::FloatRect bounds = mText.getLocalBounds();
	const sf::Vector2f& s = mText.getScale();

	mSize.x = ((bounds.left > 0.0f ? bounds.left : 0.0f) + bounds.width) * s.x;
	mSize.y = ((bounds.top > 0.0f ? bounds.top : 0.0f) + bounds.height) * s.y;

	transformDirty();
}

// ============================================================================

void TextBox::updateBounds()
{
	// Create transform, based on SFML Transformable
	float angle = -mAbsRotation * 3.141592654f / 180.f;
	float c = static_cast<float>(std::cos(angle));
	float s = static_cast<float>(std::sin(angle));
	float ox = mOrigin.x * mSize.x;
	float oy = mOrigin.y * mSize.y;
	float tx = -ox * c - oy * s + mAbsPosition.x;
	float ty = ox * s - oy * c + mAbsPosition.y;

	sf::Transform t(c, s, tx,
		-s, c, ty,
		0.f, 0.f, 1.f);

	// Update bounds
	mBounds = t.transformRect(mText.getLocalBounds());
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
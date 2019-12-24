#include <Resource/Resource.h>

// ============================================================================
// ============================================================================

Loadable::Loadable() :
	mFileName		(""),
	mIsLoaded		(false)
{

}

// ============================================================================
// ============================================================================

void Loadable::SetFileName(const sf::String& fname)
{
	mFileName = fname;
}

const sf::String& Loadable::GetFileName() const
{
	return mFileName;
}

bool Loadable::IsLoaded() const
{
	return mIsLoaded;
}

// ============================================================================
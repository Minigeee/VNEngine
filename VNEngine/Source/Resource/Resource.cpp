#include <Resource/Resource.h>

using namespace vne;

// ============================================================================
// ============================================================================

Loadable::Loadable() :
	mFileName		(""),
	mIsLoaded		(false)
{

}

// ============================================================================
// ============================================================================

void Loadable::setFileName(const sf::String& fname)
{
	mFileName = fname;
}

const sf::String& Loadable::getFileName() const
{
	return mFileName;
}

bool Loadable::isLoaded() const
{
	return mIsLoaded;
}

// ============================================================================
#ifndef LOADABLE_H
#define LOADABLE_H

#include <Core/DataTypes.h>

#include <SFML/System.hpp>

namespace vne
{

// ============================================================================

class Resource
{
public:
	/// <summary>
	/// Free any resource data
	/// </summary>
	virtual void free() = 0;
};

// ============================================================================

class Loadable : public Resource
{
public:
	Loadable();

	/// <summary>
	/// Load resource from memory
	/// </summary>
	/// <returns>True if loaded without errors</returns>
	virtual bool load(Uint8* data, Uint32 size) = 0;

	/// <summary>
	/// Set file to load from
	/// </summary>
	/// <param name="fname">File path</param>
	void setFileName(const sf::String& fname);

	/// <summary>
	/// Returns true if Load() has been called
	/// </summary>
	/// <returns>Returns true if loaded</returns>
	bool isLoaded() const;

	/// <summary>
	/// Get file name
	/// </summary>
	/// <returns>File name</returns>
	const sf::String& getFileName() const;

protected:
	/// <summary>
	/// The file to load resource from
	/// </summary>
	sf::String mFileName;

	/// <summary>
	/// True if resource has been loaded
	/// </summary>
	bool mIsLoaded;
};

// ============================================================================

}

#endif
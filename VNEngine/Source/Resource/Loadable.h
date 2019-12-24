#ifndef LOADABLE_H
#define LOADABLE_H

#include <SFML/System.hpp>

// ============================================================================

/// <summary>
/// Base class for resources
/// </summary>
class Resource
{
public:
	/// <summary>
	/// Free resource data
	/// </summary>
	virtual void Free() = 0;
};

// ============================================================================

class Loadable
{
public:
	Loadable();

	/// <summary>
	/// Load resource from file name
	/// </summary>
	/// <returns>True if loaded without errors</returns>
	virtual bool Load() = 0;

	/// <summary>
	/// Set file to load from
	/// </summary>
	/// <param name="fname">File path</param>
	void SetFileName(const sf::String& fname);

	/// <summary>
	/// Returns true if Load() has been called
	/// </summary>
	/// <returns>Returns true if loaded</returns>
	bool IsLoaded() const;

	/// <summary>
	/// Get file name
	/// </summary>
	/// <returns>File name</returns>
	const sf::String& GetFileName() const;

private:
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

#endif
#ifndef RESOURCE_H
#define RESOURCE_H

#include <Core/ObjectPool.h>
#include <Core/Macros.h>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include <unordered_map>
#include <unordered_set>
#include <aes.hpp>

// ============================================================================

#ifdef _WIN32
/// <summary>
/// Wrapper for opening files using UTF-16 or UTF-8 encoding.
/// fname = sf::String containing the file name.
/// mode = string literal containing mode
/// </summary>
#define FOPEN(fname, mode) _wfopen(fname.toWideString().c_str(), CONCAT(L, mode))
#else
/// <summary>
/// Wrapper for opening files using UTF-16 or UTF-8 encoding.
/// fname = sf::String containing the file name.
/// mode = string literal containing mode
/// </summary>
#define FOPEN(fname, mode) fopen(fname.toAnsiString().c_str(), mode)
#endif

namespace vne
{

// ============================================================================

struct ResourceInfo
{
	ResourceInfo();

	/// <summary>
	/// Pointer to resource object
	/// </summary>
	void* mResource;

	/// <summary>
	/// File name of resource to load. Empty if resource was created instead of loaded
	/// </summary>
	sf::String mFileName;

	/// <summary>
	/// Data the resource uses that must be freed after use
	/// </summary>
	Uint8* mData;
};

// ============================================================================

/// <summary>
/// Handles opening files if using normal files system.
/// Decrypt and uncompresses if using packed resource file
/// </summary>
class ResourceFolder
{
public:
	/// <summary>
	/// Set path to resource folder.
	/// This can be a directory or the packed resource folder
	/// </summary>
	/// <param name="path">Path to resource folder</param>
	static void setPath(const sf::String& path);

	/// <summary>
	/// Set resource key used to encrypt / decrypt resource folder
	/// </summary>
	/// <param name="key">Pointer to 16 byte key (128 bits)</param>
	static void setKey(const Uint8* key);

	/// <summary>
	/// Open and load file from resource folder and return pointer to to beggining of data, this memory has to be freed after use.
	/// It also returns the size of the data that was loaded
	/// </summary>
	/// <param name="fname">Path to file to load</param>
	/// <param name="size">Refernce to int to retrieve data size</param>
	/// <returns>Pointer to loaded data</returns>
	static Uint8* open(const sf::String& fname, Uint32& size);

	/// <summary>
	/// Pack current directory into packed folder with options for encryption
	/// </summary>
	/// <param name="dst">Output file</param>
	/// <param name="key">Pointer to key buffer</param>
	static void pack(const sf::String& dst);

private:
	static Uint8* openPacked(const sf::String& fname, Uint32& size);
	static Uint8* openNormal(const sf::String& fname, Uint32& size);

private:
	/// <summary>
	/// Path to resource folder
	/// </summary>
	static sf::String sResourcePath;

	/// <summary>
	/// File pointer for packed folder
	/// </summary>
	static FILE* sPackedFolder;

	/// <summary>
	/// Maps file names to file offsets in a packed folder
	/// </summary>
	static std::unordered_map<std::basic_string<Uint32>, Uint32> sPackedFolderMap;

	/// <summary>
	/// Keep a pointer to encryption key
	/// </summary>
	static const Uint8* sResourceKey;
};

// ============================================================================

/// <summary>
/// Handles the lifetime of resources
/// </summary>
template <typename T>
class Resource
{
public:
	/// <summary>
	/// Create an object of type T and return a pointer to it.
	/// Returns existing resource if it already exists
	/// </summary>
	/// <param name="name">The name of the object</param>
	/// <returns>A pointer to the new object</returns>
	static T* create(const sf::String& name)
	{
		// Get resource info
		ResourceInfo& info = sResourceMap[name.toUtf32()];
		
		// Return object if it exists
		if (info.mResource)
			return (T*)info.mResource;

		// Create object from pool
		info.mResource = sResourcePool.create();

		return (T*)info.mResource;
	}

	/// <summary>
	/// Add file location with a resource name to mark it for loading at a later time
	/// </summary>
	/// <param name="path">File path to the resource being loaded</param>
	/// <param name="name">Name to assign the resource</param>
	static void addLocation(const sf::String& path, const sf::String& name)
	{
		// Get resource info
		ResourceInfo& info = sResourceMap[name.toUtf32()];

		// Only set file name if object doesn't exist
		if (!info.mResource)
			info.mFileName = path;
	}

	/// <summary>
	/// Get resource by name.
	/// For loadable resources, the resource is loaded if it hasn't been loaded.
	/// Returns NULL if it doesn't exist
	/// </summary>
	/// <param name="name">Name of resource to retrieve</param>
	/// <returns>Pointer to resource</returns>
	static T* get(const sf::String& name)
	{
		// Get resource info
		ResourceInfo& info = sResourceMap[name.toUtf32()];

		// If there is a file name and resource hasn't been created yet, load file
		if (info.mFileName.getSize() && !info.mResource)
		{
			info.mResource = sResourcePool.create();
			if (!load((T*)info.mResource, info.mFileName, info.mData))
			{
				// If failed to load, free object and return NULL
				sResourcePool.free((T*)info.mResource);
				info.mResource = 0;
				info.mData = 0;

				return 0;
			}
		}

		return (T*)info.mResource;
	}

	/// <summary>
	/// Free a resource by name
	/// </summary>
	/// <param name="name">Name of the resource to free</param>
	static void free(const sf::String& name)
	{
		// Get resource info
		ResourceInfo& info = sResourceMap[name.toUtf32()];

		// If object exists, free and reset object
		if (info.mResource)
		{
			if (info.mData)
				std::free(info.mData);

			sResourcePool.free((T*)info.mResource);
			info.mResource = 0;
			info.mData = 0;
		}
	}

	/// <summary>
	/// Free all resources
	/// </summary>
	static void free()
	{
		// Free all resource data
		for (auto it = sResourceMap.begin(); it != sResourceMap.end(); ++it)
		{
			if (it->second.mData)
				std::free(it->second.mData);
		}

		sResourcePool.free();
		sResourceMap.clear();
	}

private:
	/// <summary>
	/// Load resource from file.
	/// This function is meant to be specialized for each type
	/// </summary>
	/// <param name="object">The object to load</param>
	/// <returns>True if there were no errors</returns>
	static bool load(T* object, const sf::String& fname, Uint8*& data)
	{
		// Default nonloadable
		return false;
	}

private:
	/// <summary>
	/// Data pool that holds all resources of type T
	/// </summary>
	static ObjectPool<T> sResourcePool;

	/// <summary>
	/// Maps resource name to object pointers
	/// </summary>
	static std::unordered_map<std::basic_string<Uint32>, ResourceInfo> sResourceMap;
};

template <typename T>
ObjectPool<T> Resource<T>::sResourcePool;

template <typename T>
std::unordered_map<std::basic_string<Uint32>, ResourceInfo> Resource<T>::sResourceMap;

// ============================================================================



// ============================================================================

/// <summary>
/// Load SFML texture
/// </summary>
template <>
inline bool Resource<sf::Texture>::load(sf::Texture* object, const sf::String& fname, Uint8*& data)
{
	Uint32 size = 0;
	data = ResourceFolder::open(fname, size);
	if (! data || !size) return false;

	bool success = object->loadFromMemory(data, size);
	std::free(data);
	data = 0;

	if (success)
	{
		// Enable smooth filter
		object->setSmooth(true);
	}

	return success;
}

// ============================================================================

template <>
inline bool Resource<sf::Font>::load(sf::Font* object, const sf::String& fname, Uint8*& data)
{
	Uint32 size = 0;
	data = ResourceFolder::open(fname, size);
	if (!data || !size) return false;

	return object->loadFromMemory(data, size);
}

// ============================================================================


}

#endif
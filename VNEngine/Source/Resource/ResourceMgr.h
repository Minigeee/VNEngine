#ifndef RESOURCE_H
#define RESOURCE_H

#include <Core/ObjectPool.h>
#include <Core/Macros.h>

#include <Resource/Resource.h>

#include <SFML/System.hpp>

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

// ============================================================================

namespace vne
{


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
class ResourceMgr
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
		// Check if resource with same name exists
		auto it = sResourceMap.find(name.toUtf32());
		if (it != sResourceMap.end())
			return it->second;

		// Create resource
		T* object = sResourcePool.New();

		// Add to resource map
		sResourceMap[name.toUtf32()] = object;

		return object;
	}

	/// <summary>
	/// Add file location with a resource name to mark it for loading at a later time
	/// </summary>
	/// <param name="path">File path to the resource being loaded</param>
	/// <param name="name">Name to assign the resource</param>
	static void addLocation(const sf::String& path, const sf::String& name)
	{
		// Create resource
		T* object = create(name);
		if (!object) return;

		// Set file name
		object->setFileName(path);
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
		// If resource doesn't exist, return
		auto it = sResourceMap.find(name.toUtf32());
		if (it == sResourceMap.end()) return 0;
		T* object = it->second;

		// Load object if not loaded
		if (std::is_base_of<Loadable, T>::value && !object->isLoaded())
		{
			if (!load(object))
				return 0;
		}

		return object;
	}

	/// <summary>
	/// Free a resource by name
	/// </summary>
	/// <param name="name">Name of the resource to free</param>
	static void free(const sf::String& name)
	{
		// If resource doesn't exist, return
		auto it = sResourceMap.find(name.toUtf32());
		if (it == sResourceMap.end()) return;
		T* object = it->second;

		// Call free
		object->free();

		// If it is not a loadable, then remove it from object pool
		if (!std::is_base_of<Loadable, T>::value)
		{
			sResourcePool.free(object);
			sResourceMap[name.toUtf32()] = 0;
		}
	}

private:
	/// <summary>
	/// Load resource from file
	/// </summary>
	/// <param name="object">The object to load</param>
	/// <returns>True if there were no errors</returns>
	static bool load(Loadable* object)
	{
		// Load bytes into memory
		Uint32 size = 0;
		Uint8* data = ResourceFolder::open(object->getFileName(), size);

		// Load data from memory
		return data && object->load(data, size);
	}

private:
	/// <summary>
	/// Data pool that holds all resources of type T
	/// </summary>
	static ObjectPool<T> sResourcePool;

	/// <summary>
	/// Maps resource name to object pointers
	/// </summary>
	static std::unordered_map<std::basic_string<Uint32>, T*> sResourceMap;
};

// ============================================================================

template <typename T>
ObjectPool<T> ResourceMgr<T>::sResourcePool;

template <typename T>
std::unordered_map<std::basic_string<Uint32>, T*> ResourceMgr<T>::sResourceMap;

// ============================================================================


}

#endif
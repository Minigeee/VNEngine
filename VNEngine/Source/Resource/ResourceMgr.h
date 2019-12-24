#ifndef RESOURCE_H
#define RESOURCE_H

#include <Engine/ObjectPool.h>
#include <Resource/Resource.h>

#include <SFML/System.hpp>

#include <unordered_map>
#include <unordered_set>

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
	static T* Create(const sf::String& name)
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
	static void AddLocation(const sf::String& path, const sf::String& name)
	{
		// Create resource
		T* object = Create(name);
		if (!object) return;

		// Set file name
		object->SetFileName(path);
	}

	/// <summary>
	/// Get resource by name.
	/// For loadable resources, the resource is loaded if it hasn't been loaded.
	/// Returns NULL if it doesn't exist
	/// </summary>
	/// <param name="name">Name of resource to retrieve</param>
	/// <returns>Pointer to resource</returns>
	static T* Get(const sf::String& name)
	{
		// If resource doesn't exist, return
		auto it = sResourceMap.find(name.toUtf32());
		if (it == sResourceMap.end()) return 0;
		T* object = it->second;

		// Load object if not loaded
		if (std::is_base_of<Loadable, T>::value && !object->IsLoaded())
		{
			if (!Load(object))
				return 0;
		}

		return object;
	}

	/// <summary>
	/// Free a resource by name
	/// </summary>
	/// <param name="name">Name of the resource to free</param>
	static void Free(const sf::String& name)
	{
		// If resource doesn't exist, return
		auto it = sResourceMap.find(name.toUtf32());
		if (it == sResourceMap.end()) return;
		T* object = it->second;

		// Call free
		object->Free();

		// If it is not a loadable, then remove it from object pool
		if (!std::is_base_of<Loadable, T>::value)
		{
			sResourcePool.Free(object);
			sResourceMap[name.toUtf32()] = 0;
		}
	}

private:
	/// <summary>
	/// Load resource from file
	/// </summary>
	/// <param name="object">The object to load</param>
	/// <returns>True if there were no errors</returns>
	bool Load(Loadable* object)
	{
		/* TODO : Implement loading */
		return object->Load(0);
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

#endif
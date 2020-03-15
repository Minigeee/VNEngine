#ifndef SOUND_MGR_H
#define SOUND_MGR_H

#include <Core/ObjectPool.h>

#include <SFML/Audio.hpp>

#include <unordered_map>

namespace vne
{

// ============================================================================

class SoundMgr
{
public:
	/// <summary>
	/// Play a sound from a sound buffer
	/// </summary>
	/// <param name="buffer">Pointer to a sound buffer</param>
	static void playSound(sf::SoundBuffer* buffer, float volume = 100.0f);

private:
	/// <summary>
	/// Sound object pool
	/// </summary>
	static ObjectPool<sf::Sound> sObjectPool;

	/// <summary>
	/// Maps sound buffers to sound objects
	/// </summary>
	static std::unordered_map<sf::SoundBuffer*, sf::Sound*> sSoundMap;
};

// ============================================================================

}

#endif
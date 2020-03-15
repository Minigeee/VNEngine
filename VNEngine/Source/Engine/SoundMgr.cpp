#include <Engine/SoundMgr.h>

using namespace vne;

ObjectPool<sf::Sound> SoundMgr::sObjectPool;
std::unordered_map<sf::SoundBuffer*, sf::Sound*> SoundMgr::sSoundMap;

// ============================================================================

void SoundMgr::playSound(sf::SoundBuffer* buffer, float volume)
{
	sf::Sound*& sound = sSoundMap[buffer];
	if (!sound)
		sound = sObjectPool.create();

	// Set buffer and play
	sound->setBuffer(*buffer);
	sound->setVolume(volume);
	sound->play();
}

// ============================================================================
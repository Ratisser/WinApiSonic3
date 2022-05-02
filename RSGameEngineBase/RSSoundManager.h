#pragma once

#pragma warning(disable:26812)

#include "../ThirdParty/FMOD/inc/fmod.hpp"

#ifdef WIN32
#pragma comment(lib, "../ThirdParty/FMOD/lib/Win32/fmod_vc.lib")
#else
#pragma comment(lib, "../ThirdParty/FMOD/lib/x64/fmod_vc.lib")
#endif

#include <unordered_map>

class RSSoundPlayer;
class RSSoundManager
{
	friend RSSoundPlayer;
public:
	~RSSoundManager();

	RSSoundManager(const RSSoundManager& _other) = delete;
	RSSoundManager(RSSoundManager&& _other) = delete;

	RSSoundManager& operator=(const RSSoundManager& _other) = delete;
	RSSoundManager& operator=(const RSSoundManager&& _other) = delete;

	static RSSoundManager& GetInstance() { return *instance_; }
	static void Destroy();

	void Initialize();
	void Update();

	void CreateSound(const std::string& _name, const std::string& _path, bool _bLoop = false);
	
	void PlaySoundByName(const std::string& _name);
	void SetVolume(unsigned int _volume);
	void StopSound();

private:
	RSSoundManager();

	FMOD::Sound* getSound(const std::string& _name);

private:
	static RSSoundManager* instance_;

private:
	FMOD::System* system_;
	FMOD::Channel* channel_;

	std::unordered_map<std::string, FMOD::Sound*> allSounds_;
};


#pragma once

#include "RSSoundManager.h"

class RSSoundPlayer
{
public:
	RSSoundPlayer(const std::string& _soundName);
	~RSSoundPlayer();

	RSSoundPlayer(const RSSoundPlayer& _other) = delete;
	RSSoundPlayer(RSSoundPlayer&& _other) = delete;

	RSSoundPlayer& operator=(const RSSoundPlayer& _other) = delete;
	RSSoundPlayer& operator=(const RSSoundPlayer&& _other) = delete;

	void ChangeSound(const std::string& _soundName);
	void Play();
	void Stop();

	bool IsPlaying();
	bool IsPaused();
	void SetPaused(bool _bPause);
	void SetVolume(float _volume);
	void SetPitch(float _pitch);
	void SetPosition(unsigned int _positionMilliSec);
	unsigned int GetPositionMillisecond();
	unsigned int GetLengthMillisecond();

private:
	FMOD::Sound* sound_;
	FMOD::Channel* channel_;
};


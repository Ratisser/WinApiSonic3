#include "RSSoundPlayer.h"

RSSoundPlayer::RSSoundPlayer(const std::string& _soundName)
	: sound_(nullptr)
	, channel_(nullptr)
{
	sound_ = RSSoundManager::GetInstance().getSound(_soundName);
}

RSSoundPlayer::~RSSoundPlayer()
{
}

void RSSoundPlayer::ChangeSound(const std::string& _soundName)
{
	sound_ = RSSoundManager::GetInstance().getSound(_soundName);
}

void RSSoundPlayer::Play()
{
	RSSoundManager::GetInstance().system_->playSound(sound_, nullptr, false, &channel_);
}

void RSSoundPlayer::Stop()
{
	FMOD_RESULT result = channel_->stop();
}

bool RSSoundPlayer::IsPlaying()
{
	bool bReturn;
	FMOD_RESULT result = channel_->isPlaying(&bReturn);
	return bReturn;
}

bool RSSoundPlayer::IsPaused()
{
	bool bReturn = true;
	if (channel_ != nullptr)
	{
		FMOD_RESULT result = channel_->getPaused(&bReturn);
	}
	return bReturn;
}

void RSSoundPlayer::SetPaused(bool _bPause)
{
	if (channel_ != nullptr)
	{
		FMOD_RESULT result = channel_->setPaused(_bPause);
	}
}

void RSSoundPlayer::SetVolume(float _volume)
{
	if (channel_ != nullptr)
	{
		FMOD_RESULT result = channel_->setVolume(_volume);
	}
}

void RSSoundPlayer::SetPitch(float _pitch)
{
	if (channel_ != nullptr)
	{
		FMOD_RESULT result = channel_->setPitch(_pitch);
	}
}

void RSSoundPlayer::SetPosition(unsigned int _positionMilliSec)
{
	if (channel_ != nullptr)
	{
		FMOD_RESULT result = channel_->setPosition(_positionMilliSec, FMOD_TIMEUNIT_MS);
	}
}

unsigned int RSSoundPlayer::GetPositionMillisecond()
{
	unsigned int returnPosition;
	FMOD_RESULT result = channel_->getPosition(&returnPosition, FMOD_TIMEUNIT_MS);
	return returnPosition;
}

unsigned int RSSoundPlayer::GetLengthMillisecond()
{
	unsigned int returnLength;
	FMOD_RESULT result = sound_->getLength(&returnLength, FMOD_TIMEUNIT_MS);
	return returnLength;
}

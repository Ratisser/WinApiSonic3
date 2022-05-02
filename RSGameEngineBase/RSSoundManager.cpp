#include "RSSoundManager.h"
#include "RSDebug.h"

#include <Windows.h>

RSSoundManager* RSSoundManager::instance_ = new RSSoundManager;

RSSoundManager::RSSoundManager()
	: system_(nullptr)
	, channel_(nullptr)
{
}

FMOD::Sound* RSSoundManager::getSound(const std::string& _name)
{
	std::unordered_map<std::string, FMOD::Sound*>::iterator findIter = allSounds_.find(_name);
	std::unordered_map<std::string, FMOD::Sound*>::iterator endIter = allSounds_.end();

	if (findIter == endIter)
	{
		RSDebug::MsgBoxError("No Sounds.");
		return nullptr;
	}

	return findIter->second;
}

RSSoundManager::~RSSoundManager()
{
	std::unordered_map<std::string, FMOD::Sound*>::iterator startIter = allSounds_.begin();
	std::unordered_map<std::string, FMOD::Sound*>::iterator endIter = allSounds_.end();

	while (startIter != endIter)
	{
		startIter->second->release();
		++startIter;
	}

	allSounds_.clear();

	system_->close();
	system_->release();
}

void RSSoundManager::Initialize()
{
	FMOD_RESULT result;

	result = FMOD::System_Create(&system_);
	if (FMOD_OK != result)
	{
		RSDebug::MsgBoxError("FMOD system create failed.");
	}

	result = system_->init(32, FMOD_INIT_NORMAL, nullptr);
	if (FMOD_OK != result)
	{
		RSDebug::MsgBoxError("FMOD system initialize failed.");
	}
}

void RSSoundManager::Update()
{
	if (nullptr == system_)
	{
		RSDebug::AssertFalse();
	}

	system_->update();
}

void RSSoundManager::CreateSound(const std::string& _name, const std::string& _path, bool _bLoop)
{
	std::unordered_map<std::string, FMOD::Sound*>::iterator findIter = allSounds_.find(_name);
	std::unordered_map<std::string, FMOD::Sound*>::iterator endIter = allSounds_.end();

	if (findIter != endIter)
	{
		RSDebug::AssertFalse();
	}

	FMOD_RESULT result;
	FMOD::Sound* newSound;
	FMOD_MODE mode = FMOD_DEFAULT;

	if (_bLoop)
	{
		mode = FMOD_LOOP_NORMAL;
	}

	char buffer[256]{ 0, };
	GetCurrentDirectoryA(256, buffer);
	std::string path = buffer;
	if (-1 == path.find("Build"))
	{
		path += "\\..\\Build";
	}

	path += "\\";
	path += _path;
	result = system_->createSound(path.c_str(), mode, nullptr, &newSound);

	if (FMOD_OK != result)
	{
		RSDebug::MsgBoxError("createSound failed.");
	}

	allSounds_[_name] = newSound;
}

void RSSoundManager::PlaySoundByName(const std::string& _name)
{
	std::unordered_map<std::string, FMOD::Sound*>::iterator findIter = allSounds_.find(_name);
	std::unordered_map<std::string, FMOD::Sound*>::iterator endIter = allSounds_.end();

	if (findIter == endIter)
	{
		RSDebug::MsgBoxError("No Sounds.");
	}

	system_->playSound(findIter->second, 0, false, &channel_);
}

void RSSoundManager::SetVolume(unsigned int _volume)
{
}

void RSSoundManager::StopSound()
{
	channel_->stop();
}

void RSSoundManager::Destroy()
{

	if (nullptr != instance_)
	{
		delete instance_;
		instance_ = nullptr;
	}
}

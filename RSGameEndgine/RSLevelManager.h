#pragma once

#include <string>
#include <map>

#include <RSDebug.h>
#include <RSNameHelper.h>

// 분류 :
// 용도 :
// 설명 :
class RSLevel;
class RSLevelManager : public RSNameHelper
{
public:
	static RSLevelManager& GetInstance()
	{
		return *instance_;
	}

	static void Destroy()
	{
		if (nullptr != instance_)
		{
			delete instance_;
			instance_ = nullptr;
		}
	}

public:
	template<typename LevelType>
	void CreateLevel(const std::string& _levelName);
	void ChangeLevel(const std::string& _levelName);

	void Update();
	void Render();
	 
public:
	~RSLevelManager();

	RSLevelManager(const RSLevelManager& _other) = delete;
	RSLevelManager(RSLevelManager&& _other) = delete;

	RSLevelManager& operator=(const RSLevelManager& _other) = delete;
	RSLevelManager& operator=(const RSLevelManager&& _other) = delete;

private:
	RSLevelManager();

private:
	static RSLevelManager* instance_;

	std::map<std::string, RSLevel*> allLevels_;
	RSLevel* currentLevel_;
	RSLevel* nextLevel_;

};

/*----------------------------------------*/

template<typename LevelType>
inline void RSLevelManager::CreateLevel(const std::string& _levelName)
{
	std::map<std::string, RSLevel*>::iterator findIter = allLevels_.find(_levelName);

	if (findIter != allLevels_.end())
	{
		RSDebug::AssertFalse();
		return;
	}

	RSLevel* level = new LevelType();
	level->LoadLevel();
	level->SetName(_levelName);

	allLevels_.insert(std::pair<std::string, RSLevel*>(_levelName, level));
}

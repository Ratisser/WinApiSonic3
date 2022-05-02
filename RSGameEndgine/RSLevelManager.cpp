#include "RSLevelManager.h"
#include "RSLevel.h"
#include "RSResourceManager.h"
#include "RSImage.h"
#include "RSCollision.h"

#include <RSSoundManager.h>
#include <RSDebug.h>

RSLevelManager* RSLevelManager::instance_ = new RSLevelManager();

RSLevelManager::RSLevelManager()
	: currentLevel_(nullptr)
	, allLevels_()
	, nextLevel_(nullptr)
{
}


void RSLevelManager::Update()
{
	if (nullptr == currentLevel_)
	{
		RSDebug::AssertFalse();
		return;
	}

	currentLevel_->ActorUpdateOrderCheck();
	currentLevel_->CollisionGroupCheck();
	currentLevel_->ActorRenderOrderCheck();

	currentLevel_->LevelUpdateBefore();
	currentLevel_->UpdateBefore();

	currentLevel_->LevelUpdate();
	currentLevel_->Update();

	currentLevel_->LevelUpdateAfter();
	currentLevel_->UpdateAfter();

	currentLevel_->ReleaseUpdate();

	if (nullptr != nextLevel_)
	{
		currentLevel_ = nextLevel_;
		nextLevel_ = nullptr;
	}
}

void RSLevelManager::Render()
{
	if (nullptr == currentLevel_)
	{
		RSDebug::AssertFalse();
		return;
	}

	RSResourceManager::GetInstance().ClearBackBuffer();
	currentLevel_->Render();
	RSResourceManager::GetInstance().DoubleBuffering();
}

void RSLevelManager::ChangeLevel(const std::string& _levelName)
{
	std::map<std::string, RSLevel*>::iterator findIter = allLevels_.find(_levelName);

	if (findIter == allLevels_.end())
	{
		RSDebug::AssertFalse();
		return;
	}

	RSCollision::collisionCheckFunctionInitialize_();
	RSSoundManager::GetInstance().StopSound();

	if (nullptr != currentLevel_)
	{
		currentLevel_->Cleanup();
	}
	currentLevel_ = findIter->second;
}

RSLevelManager::~RSLevelManager()
{
	std::map<std::string, RSLevel*>::iterator startIter = allLevels_.begin();
	auto endIter = allLevels_.end();

	while (startIter != endIter)
	{
		delete startIter->second;
		startIter++;
	}

	allLevels_.clear();
}
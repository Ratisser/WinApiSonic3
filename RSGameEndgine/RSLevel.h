#pragma once

#include <list>
#include <map>
#include <unordered_map>
#include <string>
#include <RSNameHelper.h>
#include <RSMath.h>
#include <RSDebug.h>

#include "eCollisionCheckGroup.h"

class RSActor;
class RSCollision;
class RSLevelManager;
class RSLevel : public RSNameHelper
{
	friend RSLevelManager;
public:
	RSLevel(const RSLevel& _other) = delete;
	RSLevel(RSLevel&& _other) = delete;

	RSLevel& operator=(const RSLevel& _other) = delete;
	RSLevel& operator=(const RSLevel&& _other) = delete;

	void SetCamPos(const float4 _camPos) { camPos_ = _camPos; }
	void SetCamMovement(const float4 _movement) { camPos_ = camPos_ + _movement; }
	void SetCamSize(const float4 _camSize) { camSize_ = _camSize; }
	float4 GetCamPos() { return camPos_; }
	float4 GetCamSize() { return camSize_; }

	void LoadLevel();
	void Cleanup();

	void UpdateBefore();
	void Update();
	void UpdateAfter();

	void Render();

	void ReleaseUpdate();

	void ActorUpdateOrderCheck();
	void ActorRenderOrderCheck();
	void CollisionGroupCheck();

	void ReleaseActorAtRenderOrder(RSActor* _actor);

	template<typename ActorType>
	ActorType* AddActor();

	template<typename ActorType>
	ActorType* AddActor(const std::string& _actorName);

	template<typename ActorType>
	ActorType* FindActor(const std::string& _actorName);

	void PushCollision(RSCollision* _collision);
	std::list<RSCollision*>& GetCollisionList(int _groupIndex);

public:
	virtual void LevelUpdateBefore() = 0;
	virtual void LevelUpdate() = 0;
	virtual void LevelUpdateAfter() = 0;

protected:
	// 레벨의 초기화를 수행합니다.
	virtual void loading() = 0;
	virtual ~RSLevel();

protected:
	RSLevel();



protected:
	std::map<int, std::list<RSActor*>> updateOrderActors_;
	std::map<int, std::list<RSActor*>> renderOrderActors_;
	std::unordered_map<std::string, RSActor*> allActors_;
	std::map<int, std::list<RSCollision*>> collisions_;

	bool bLoadingComplete;

	float4 camPos_;
	float4 camSize_;
};

/*----------------------------------------*/













template<typename ActorType>
inline ActorType* RSLevel::AddActor()
{
	ActorType* newActor = new ActorType();
	newActor->SetName("Actor");
	newActor->SetLevel(this);
	newActor->Start();
	updateOrderActors_[0].push_back(newActor);
	renderOrderActors_[0].push_back(newActor);
	return newActor;
}

template<typename ActorType>
inline ActorType* RSLevel::AddActor(const std::string& _actorName)
{
	if (allActors_.find(_actorName) != allActors_.end())
	{
		RSDebug::MsgBoxError("Actor already exist");
		return nullptr;
	}

	ActorType* newActor = new ActorType();
	newActor->SetName(_actorName);
	newActor->SetLevel(this);
	newActor->Start();

	allActors_[_actorName] = newActor;
	updateOrderActors_[0].push_back(newActor);
	renderOrderActors_[0].push_back(newActor);

	return newActor;
}

template<typename ActorType>
inline ActorType* RSLevel::FindActor(const std::string& _actorName)
{
	std::unordered_map<std::string, RSActor*>::iterator findIter = allActors_.find(_actorName);

	if (findIter == allActors_.end())
	{
		return nullptr;
	}

	ActorType* ret = dynamic_cast<ActorType*>(findIter->second);
	if (nullptr == ret)
	{
		RSDebug::MsgBoxError("Actor convert failed");
	}
	return ret;
}

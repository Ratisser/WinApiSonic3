#include "RSLevel.h"
#include "RSCollision.h"
#include "RSActor.h"

RSLevel::RSLevel()
	: updateOrderActors_()
	, bLoadingComplete(false)
{
}

void RSLevel::ActorUpdateOrderCheck()
{
	std::map<int, std::list<RSActor*>>::iterator mapIter = updateOrderActors_.begin();
	std::map<int, std::list<RSActor*>>::iterator mapEnd = updateOrderActors_.end();

	while (mapIter != mapEnd)
	{
		std::list<RSActor*>::iterator listIter = mapIter->second.begin();
		std::list<RSActor*>::iterator listEnd = mapIter->second.end();

		while (listIter != listEnd)
		{
			RSActor* currentActor = *listIter;
			int updateOrder = currentActor->GetUpdateOrder();
			if (updateOrder != mapIter->first)
			{
				updateOrderActors_[updateOrder].push_back(currentActor);
				listIter = mapIter->second.erase(listIter);
			}
			else
			{
				listIter++;
			}
		}

		if (0 == mapIter->second.size())
		{
			mapIter = updateOrderActors_.erase(mapIter);
		}
		else
		{
			mapIter++;
		}
	}
}

void RSLevel::ActorRenderOrderCheck()
{
	std::map<int, std::list<RSActor*>>::iterator mapIter = renderOrderActors_.begin();
	std::map<int, std::list<RSActor*>>::iterator mapEnd = renderOrderActors_.end();

	while (mapIter != mapEnd)
	{
		std::list<RSActor*>::iterator listIter = mapIter->second.begin();
		std::list<RSActor*>::iterator listEnd = mapIter->second.end();

		while (listIter != listEnd)
		{
			RSActor* currentActor = *listIter;
			int updateOrder = currentActor->GetRenderOrder();
			if (updateOrder != mapIter->first)
			{
				renderOrderActors_[updateOrder].push_back(currentActor);
				listIter = mapIter->second.erase(listIter);
			}
			else
			{
				listIter++;
			}
		}

		if (0 == mapIter->second.size())
		{
			mapIter = renderOrderActors_.erase(mapIter);
		}
		else
		{
			mapIter++;
		}
	}
}

void RSLevel::CollisionGroupCheck()
{
	std::map<int, std::list<RSCollision*>>::iterator mapIter = collisions_.begin();
	std::map<int, std::list<RSCollision*>>::iterator mapEnd = collisions_.end();

	while (mapIter != mapEnd)
	{
		std::list<RSCollision*>::iterator listIter = mapIter->second.begin();
		std::list<RSCollision*>::iterator listEnd = mapIter->second.end();

		while (listIter != listEnd)
		{
			RSCollision* currentCollision = *listIter;
			int groupIndex = static_cast<int>(currentCollision->GetGroupIndex());
			if (groupIndex != mapIter->first)
			{
				collisions_[groupIndex].push_back(currentCollision);
				listIter = mapIter->second.erase(listIter);
			}
			else
			{
				listIter++;
			}
		}

		if (0 == mapIter->second.size())
		{
			mapIter = collisions_.erase(mapIter);
		}
		else
		{
			mapIter++;
		}
	}
}

void RSLevel::ReleaseActorAtRenderOrder(RSActor* _actor)
{
	if (nullptr != _actor)
	{
		renderOrderActors_[_actor->GetRenderOrder()].remove(_actor);
	}
}

RSLevel::~RSLevel()
{
	std::map<int, std::list<RSActor*>>::iterator mapIter = updateOrderActors_.begin();
	std::map<int, std::list<RSActor*>>::iterator mapEnd = updateOrderActors_.end();

	while (mapIter != mapEnd)
	{
		std::list<RSActor*>::iterator listIter = mapIter->second.begin();
		std::list<RSActor*>::iterator listEnd = mapIter->second.end();

		while (listIter != listEnd)
		{
			delete (*listIter);
			listIter++;
		}

		mapIter->second.clear();
		mapIter++;
	}

	updateOrderActors_.clear();
	renderOrderActors_.clear();
}

void RSLevel::LoadLevel()
{
	if (false == bLoadingComplete)
	{
		loading();
		bLoadingComplete = true;
	}
}

void RSLevel::Cleanup()
{
	std::map<int, std::list<RSActor*>>::iterator mapIter = updateOrderActors_.begin();
	std::map<int, std::list<RSActor*>>::iterator mapEnd = updateOrderActors_.end();

	while (mapIter != mapEnd)
	{
		std::list<RSActor*>::iterator listIter = mapIter->second.begin();
		std::list<RSActor*>::iterator listEnd = mapIter->second.end();

		while (listIter != listEnd)
		{
			(*listIter)->Cleanup();
			listIter++;
		}

		mapIter++;
	}
}

void RSLevel::UpdateBefore()
{
	std::map<int, std::list<RSActor*>>::iterator mapIter = updateOrderActors_.begin();
	std::map<int, std::list<RSActor*>>::iterator mapEnd = updateOrderActors_.end();

	while (mapIter != mapEnd)
	{
		std::list<RSActor*>::iterator listIter = mapIter->second.begin();
		std::list<RSActor*>::iterator listEnd = mapIter->second.end();

		while (listIter != listEnd)
		{
			(*listIter)->UpdateBefore();
			listIter++;
		}

		mapIter++;
	}
}

void RSLevel::Update()
{
	std::map<int, std::list<RSActor*>>::iterator mapIter = updateOrderActors_.begin();
	std::map<int, std::list<RSActor*>>::iterator mapEnd = updateOrderActors_.end();

	while (mapIter != mapEnd)
	{
		std::list<RSActor*>::iterator listIter = mapIter->second.begin();
		std::list<RSActor*>::iterator listEnd = mapIter->second.end();

		while (listIter != listEnd)
		{
			(*listIter)->Update();
			listIter++;
		}

		mapIter++;
	}
}

void RSLevel::UpdateAfter()
{
	std::map<int, std::list<RSActor*>>::iterator mapIter = updateOrderActors_.begin();
	std::map<int, std::list<RSActor*>>::iterator mapEnd = updateOrderActors_.end();

	while (mapIter != mapEnd)
	{
		std::list<RSActor*>::iterator listIter = mapIter->second.begin();
		std::list<RSActor*>::iterator listEnd = mapIter->second.end();

		while (listIter != listEnd)
		{
			(*listIter)->UpdateAfter();
			listIter++;
		}

		mapIter++;
	}
}

void RSLevel::Render()
{
	std::map<int, std::list<RSActor*>>::iterator mapIter = renderOrderActors_.begin();
	std::map<int, std::list<RSActor*>>::iterator mapEnd = renderOrderActors_.end();

	while (mapIter != mapEnd)
	{
		std::list<RSActor*>::iterator listIter = mapIter->second.begin();
		std::list<RSActor*>::iterator listEnd = mapIter->second.end();

		while (listIter != listEnd)
		{
			(*listIter)->Render();
			listIter++;
		}

		mapIter++;
	}
}

void RSLevel::ReleaseUpdate()
{
	std::map<int, std::list<RSActor*>>::iterator mapIter = updateOrderActors_.begin();
	std::map<int, std::list<RSActor*>>::iterator mapEnd = updateOrderActors_.end();

	while (mapIter != mapEnd)
	{
		std::list<RSActor*>::iterator listIter = mapIter->second.begin();
		std::list<RSActor*>::iterator listEnd = mapIter->second.end();

		while (listIter != listEnd)
		{
			if ((*listIter)->bRelease_)
			{
				ReleaseActorAtRenderOrder(*listIter);
				delete (*listIter);
				*listIter = nullptr;
				listIter = mapIter->second.erase(listIter);
			}
			else
			{
				listIter++;
			}
		}

		mapIter++;
	}
}

void RSLevel::PushCollision(RSCollision* _collision)
{
	if (nullptr == _collision)
	{
		RSDebug::AssertFalse();
		return;
	}

	collisions_[0].push_back(_collision);
}

std::list<RSCollision*>& RSLevel::GetCollisionList(int _groupIndex)
{
	std::map<int, std::list<RSCollision*>>::iterator findIter = collisions_.find(_groupIndex);

	if (findIter == collisions_.end())
	{
		RSDebug::MsgBoxError("no collision group list");
	}

	return findIter->second;
}

#include "PlayLevelMap.h"

#include "BlindMap.h"

#include <RSCollision.h>
#include <RSRenderer.h>
#include <RSResourceManager.h>


PlayLevelMap::PlayLevelMap()
	: mainRender_(nullptr)
	, rollLeftRender_(nullptr)
	, rollRightRender_(nullptr)
	, rollUpRender_(nullptr)
	, mapCollisionRender_(nullptr)
	, mapCollision_(nullptr)
	, rollLeftCollision_(nullptr)
	, rollRightCollision_(nullptr)
	, rollUpCollision_(nullptr)
	, bShowCollision_(false)
{
}

PlayLevelMap::~PlayLevelMap()
{
	SAFE_DELETE(mainRender_);
	SAFE_DELETE(mapCollisionRender_);
	SAFE_DELETE(rollLeftRender_);
	SAFE_DELETE(rollRightRender_);
	SAFE_DELETE(rollUpRender_);
	SAFE_DELETE(mapCollision_);
	SAFE_DELETE(rollLeftCollision_);
	SAFE_DELETE(rollRightCollision_);
	SAFE_DELETE(rollUpCollision_);
}

void PlayLevelMap::Start()
{
	RSImage* background = RSResourceManager::GetInstance().FindImage("stage1-1_dry.bmp");
	SetLocation(background->GetSize().halffloat4());
	SetRenderOrder(100);

	mainRender_ = new RSRenderer(this);
	mainRender_->SetImage(background);

	mapCollisionRender_ = new RSRenderer(this);
	mapCollisionRender_->SetImage("stage1-1_collision.bmp");

	mapCollision_ = new RSCollision(this, eCollisionType::IMAGE, static_cast<int>(eCollisionCheckGroup::MAP));
	mapCollision_->SetImage("stage1-1_collision.bmp");

	rollLeftRender_ = new RSRenderer(this);
	rollLeftRender_->SetImage("stage1-1_rollLeft.bmp");

	rollLeftCollision_ = new RSCollision(this, eCollisionType::IMAGE, static_cast<int>(eCollisionCheckGroup::ROLL_LEFT));
	rollLeftCollision_->SetImage("stage1-1_rollLeft.bmp");

	rollRightRender_ = new RSRenderer(this);
	rollRightRender_->SetImage("stage1-1_rollRight.bmp");

	rollRightCollision_ = new RSCollision(this, eCollisionType::IMAGE, static_cast<int>(eCollisionCheckGroup::ROLL_RIGHT));
	rollRightCollision_->SetImage("stage1-1_rollRight.bmp");

	rollUpRender_ = new RSRenderer(this);
	rollUpRender_->SetImage("stage1-1_rollUp.bmp");

	rollUpCollision_ = new RSCollision(this, eCollisionType::IMAGE, static_cast<int>(eCollisionCheckGroup::ROLL_UP));
	rollUpCollision_->SetImage("stage1-1_rollUp.bmp");

	if (false == RSInput::GetInstance().IsKeyAvailable("P"))
	{
		RSInput::GetInstance().CreateKey("P", 'P');
	}

	level_->PushCollision(mapCollision_);
	level_->PushCollision(rollLeftCollision_);
	level_->PushCollision(rollRightCollision_);
	level_->PushCollision(rollUpCollision_);
}

void PlayLevelMap::UpdateBefore()
{
}

void PlayLevelMap::Update()
{
#ifdef _DEBUG
	if (RSInput::GetInstance().IsKeyDown("P") && level_->GetName() == "PlayLevel")
	{
		bShowCollision_ = !bShowCollision_;

		BlindMap* blindMap = level_->FindActor<BlindMap>("BlindMap");
		if (bShowCollision_)
		{
			blindMap->SetRenderOrder(99);
		}
		else
		{
			blindMap->SetRenderOrder(2000);
		}
	}
#endif
}

void PlayLevelMap::UpdateAfter()
{
}

void PlayLevelMap::Render()
{
	mainRender_->Render();
	if (bShowCollision_)
	{
		mapCollisionRender_->Render();
		rollLeftRender_->Render();
		rollRightRender_->Render();
		rollUpRender_->Render();
	}
}

void PlayLevelMap::Cleanup()
{
}


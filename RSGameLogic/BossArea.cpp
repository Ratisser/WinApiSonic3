#include "BossArea.h"

#include <RSCollision.h>

#include "Player.h"
#include "FireBreath.h"

BossArea::BossArea()
	: bossArea_(nullptr)
	, bossAreaWallLeft_(nullptr)
	, bossAreaWallRight_(nullptr)
	, bTriggered_(false)
	, bTriggering_(false)
	, delay_(2.0f)
{
}

BossArea::~BossArea()
{
	SAFE_DELETE(bossArea_);
	SAFE_DELETE(bossAreaWallLeft_);
	SAFE_DELETE(bossAreaWallRight_);
}

void BossArea::Start()
{
	SetLocation({ 9976.f, 696.f });
	SetRenderOrder(101);
	bossArea_ = new RSCollision(this, eCollisionType::RECT, static_cast<int>(eCollisionCheckGroup::LOCATION));
	bossArea_->SetRect(0.0f, 0.0f, 320.f, 240.f);

	bossAreaWallLeft_ = new RSCollision(this, eCollisionType::RECT, static_cast<int>(eCollisionCheckGroup::MAP));
	bossAreaWallLeft_->SetRect(-50.f, -50.f, 0.0f, 240.f + 50.f);

	bossAreaWallRight_ = new RSCollision(this, eCollisionType::RECT, static_cast<int>(eCollisionCheckGroup::MAP));
	bossAreaWallRight_->SetRect(320.f, -50.f, 320.f + 50.f, 240.f + 50.f);
	level_->PushCollision(bossAreaWallRight_);
}

void BossArea::UpdateBefore()
{
}

void BossArea::Update()
{
	float deltaTime = RSTimer::GetInstance().GetDeltaTime();
	if (bTriggered_)
	{
		return;
	}

	if (bTriggering_)
	{
		delay_ -= deltaTime;
		if (delay_ < 0.f)
		{
			Player* player = level_->FindActor<Player>("Player");
			FireBreath* fireBreath = level_->FindActor<FireBreath>("FireBreath");
			fireBreath->Appear();
			player->ChangeBGMBossArea();
			player->StartBGM();
			level_->PushCollision(bossAreaWallLeft_);
			bTriggered_ = true;
		}
		return;
	}

	if (nullptr != bossArea_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::PLAYER)))
	{
		Player* player = level_->FindActor<Player>("Player");
		player->EnterBossArea();
		bTriggering_ = true;
	}
}

void BossArea::UpdateAfter()
{
}

void BossArea::Render()
{
	//HDC hdc = RSResourceManager::GetInstance().GetBackBuffer()->GetDC();
	//bossArea_->DebugRender(hdc);
	//if (bTriggered_)
	//{
	//	bossAreaWallLeft_->DebugRender(hdc);
	//}

	//bossAreaWallRight_->DebugRender(hdc);

}

void BossArea::Cleanup()
{
}

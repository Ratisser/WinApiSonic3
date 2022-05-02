#include "Ring.h"

#include <RSRenderer.h>
#include <RSCollision.h>
#include <RSSoundPlayer.h>

#include "eCollisionCheckColor.h"
#include "PlayLevelUI.h"
#include "Player.h"

Ring::Ring()
	: mainRender_(nullptr)
	, collision_(nullptr)
	, groundCollision_(nullptr)
	, soundPlayer_(nullptr)
	, ui_(nullptr)
	, state_(eRingState::ACTIVATE)
	, dropSpeed_(float4::ZERO)
	, dropStartDelay_(0.0f)
	, dropUpdateDelay_(0.0f)
	, dropAfterDelay_(0.0f)
	, blinkTimer_(0.0f)
{
}

Ring::~Ring()
{
	SAFE_DELETE(mainRender_);
	SAFE_DELETE(collision_);
	SAFE_DELETE(groundCollision_);
	SAFE_DELETE(soundPlayer_);
}

void Ring::Start()
{
	mainRender_ = new RSRenderer(this);
	mainRender_->CreateAnimation("Ring", "ring.bmp", 0, 3, 0.2f, true);
	mainRender_->CreateAnimation("Disappear", "ring.bmp", 4, 7, 0.1f);
	mainRender_->ChangeAnimation("Ring", "ring.bmp");

	collision_ = new RSCollision(this, eCollisionType::CIRCLE, static_cast<int>(eCollisionCheckGroup::RING));
	collision_->SetRadius(15.f);
	level_->PushCollision(collision_);

	groundCollision_ = new RSCollision(this, eCollisionType::POINT, static_cast<int>(eCollisionCheckGroup::RING));
	groundCollision_->SetPivot({ 0.0f, 15.f });
	groundCollision_->SetCheckColor(static_cast<DWORD>(eCollisionCheckColor::MAP));

	soundPlayer_ = new RSSoundPlayer("ring.wav");

	ui_ = level_->FindActor<PlayLevelUI>("PlayLevelUI");

	SetRenderOrder(999);
}

void Ring::UpdateBefore()
{
	mainRender_->UpdateAnimation();
}

void Ring::Update()
{
	float deltaTime = RSTimer::GetInstance().GetDeltaTime();

	switch (state_)
	{
	case eRingState::ACTIVATE:
	{
		RSCollision* collision = collision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::PLAYER));
		if (nullptr != collision)
		{
			Player* player = dynamic_cast<Player*>(collision->GetActor());
			state_ = eRingState::DISAPPEAR;
			player->PushRingToDrop(this);
			soundPlayer_->Play();
			ui_->AddRing();
			mainRender_->ChangeAnimation("Disappear", "ring.bmp");
		}
	}
	break;
	case eRingState::DISAPPEAR:
		if (mainRender_->IsAnimationEnd())
		{
			state_ = eRingState::DEACTIVATE;
			mainRender_->Deactivate();
		}
		break;
	case eRingState::DROP_START:
		dropStart(deltaTime);
		break;
	case eRingState::DROP_UPDATE:
		dropUpdate(deltaTime);
		break;
	case eRingState::DROP_AFTER:
		dropAfter(deltaTime);
		break;
	case eRingState::DEACTIVATE:
		// Deactivate do nothing
		break;
	default:
		break;
	}
}

void Ring::UpdateAfter()
{
}

void Ring::Render()
{
	mainRender_->Render();
}

void Ring::Cleanup()
{
}

void Ring::Deactivate()
{
	state_ = eRingState::DEACTIVATE;
}

bool Ring::IsCollected()
{
	return state_ == eRingState::DEACTIVATE;
}

void Ring::Drop(const float4& _location)
{
	RSRandom random;

	dropSpeed_.x = random.RandomFloat(0.f, 200.f);
	if (static_cast<int>(dropSpeed_.x) & 0x1)
	{
		dropSpeed_.x *= -1;
	}

	dropSpeed_.y = random.RandomFloat(50.f, 100.f);
	dropSpeed_.y *= -1;

	location_ = _location;
	mainRender_->Activate();
	mainRender_->ChangeAnimation("Ring", "ring.bmp", true);

	dropStartDelay_ = 0.5f;
	dropUpdateDelay_ = 1.5f;
	dropAfterDelay_ = 1.5f;

	state_ = eRingState::DROP_START;
}

void Ring::dropStart(float _deltaTime)
{
	dropStartDelay_ -= _deltaTime;
	SetMovement(dropSpeed_ * _deltaTime);

	if (dropStartDelay_ < 0.0f)
	{
		state_ = eRingState::DROP_UPDATE;
	}
}

void Ring::dropUpdate(float _deltaTime)
{
	dropUpdateDelay_ -= _deltaTime;
	RSCollision* collision = collision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::PLAYER));
	if (nullptr != collision)
	{
		Player* player = dynamic_cast<Player*>(collision->GetActor());
		state_ = eRingState::DISAPPEAR;
		player->PushRingToDrop(this);
		soundPlayer_->Play();
		ui_->AddRing();
		mainRender_->ChangeAnimation("Disappear", "ring.bmp");
	}

	if (nullptr == groundCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
	{
		dropSpeed_.y += 400.f * _deltaTime;
	}
	else
	{
		dropSpeed_.y *= -0.7f;
		SetMovement({ 0.0f, -2.0f });
	}

	if (dropSpeed_.x < 0)
	{
		dropSpeed_.x += 50.f * _deltaTime;
	}
	else
	{
		dropSpeed_.x -= 50.f * _deltaTime;
	}

	SetMovement(dropSpeed_ * _deltaTime);
	
	if (dropUpdateDelay_ < 0.0f)
	{
		blinkTimer_ = 0.05f;
		state_ = eRingState::DROP_AFTER;
	}
}

void Ring::dropAfter(float _deltaTime)
{
	dropAfterDelay_ -= _deltaTime;
	blinkTimer_ -= _deltaTime;
	RSCollision* collision = collision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::PLAYER));
	if (nullptr != collision)
	{
		Player* player = dynamic_cast<Player*>(collision->GetActor());
		state_ = eRingState::DISAPPEAR;
		player->PushRingToDrop(this);
		soundPlayer_->Play();
		ui_->AddRing();
		mainRender_->ChangeAnimation("Disappear", "ring.bmp");
	}

	if (nullptr == groundCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
	{
		dropSpeed_.y += 400.f * _deltaTime;
	}
	else
	{
		dropSpeed_.y *= -0.7f;
		SetMovement({ 0.0f, -2.0f });
	}

	if (dropSpeed_.x < 0)
	{
		dropSpeed_.x += 50.f * _deltaTime;
	}
	else
	{
		dropSpeed_.x -= 50.f * _deltaTime;
	}

	SetMovement(dropSpeed_ * _deltaTime);

	if (blinkTimer_ < 0.0f)
	{
		mainRender_->SwitchActivation();
		blinkTimer_ = 0.05f;
	}

	if (dropAfterDelay_ < 0.0f)
	{
		mainRender_->Deactivate();
		state_ = eRingState::DEACTIVATE;
	}
}

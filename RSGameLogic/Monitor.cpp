#include "Monitor.h"

#include <RSRenderer.h>
#include <RSCollision.h>
#include <RSSoundPlayer.h>

#include "Player.h"
#include "PlayLevelUI.h"

Monitor::Monitor()
	: mainRender_(nullptr)
	, popRender_(nullptr)
	, collision_(nullptr)
	, popSound_(nullptr)
	, fireSound_(nullptr)
	, electricSound_(nullptr)
	, bubbleSound_(nullptr)
	, lifeSound_(nullptr)
	, state_(eMonitorState::ACTIVATE)
	, monitorType_(eMonitor::RING)
{
}

Monitor::~Monitor()
{
	SAFE_DELETE(mainRender_);
	SAFE_DELETE(popRender_);
	SAFE_DELETE(collision_);
	SAFE_DELETE(popSound_);
	SAFE_DELETE(fireSound_);
	SAFE_DELETE(electricSound_);
	SAFE_DELETE(bubbleSound_);
	SAFE_DELETE(lifeSound_);
}

void Monitor::Start()
{
	mainRender_ = new RSRenderer(this);

	mainRender_->CreateAnimation("Ring", "monitors.bmp", 0, 1, 0.1f, true);
	mainRender_->CreateAnimation("Electric", "monitors.bmp", 4, 5, 0.1f, true);
	mainRender_->CreateAnimation("Fire", "monitors.bmp", 6, 7, 0.1f, true);
	mainRender_->CreateAnimation("Bubble", "monitors.bmp", 8, 9, 0.1f, true);
	mainRender_->CreateAnimation("Life", "monitors.bmp", 12, 13, 0.1f, true);
	mainRender_->CreateAnimation("Break", "monitors.bmp", 16, 16, 1.0f);
	mainRender_->ChangeAnimation("Ring", "monitors.bmp");

	popRender_ = new RSRenderer(this);
	popRender_->CreateAnimation("Pop", "pop.bmp", 0, 3, 0.1f, false);
	popRender_->ChangeAnimation("Pop", "pop.bmp");
	popRender_->Deactivate();

	collision_ = new RSCollision(this, eCollisionType::CIRCLE, static_cast<int>(eCollisionCheckGroup::MONITOR));
	collision_->SetRadius(17.f);
	level_->PushCollision(collision_);

	popSound_ = new RSSoundPlayer("pop.wav");
	fireSound_ = new RSSoundPlayer("fireShield.wav");
	electricSound_ = new RSSoundPlayer("electricShield.wav");
	bubbleSound_ = new RSSoundPlayer("bubbleShield.wav");
	lifeSound_ = new RSSoundPlayer("life.mid");
	SetRenderOrder(990);
}

void Monitor::UpdateBefore()
{
	mainRender_->UpdateAnimation();
	popRender_->UpdateAnimation();
}

void Monitor::Update()
{
	switch (state_)
	{
	case Monitor::eMonitorState::ACTIVATE:
	{
		RSCollision* playerCollision = collision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::PLAYER));
		if (playerCollision != nullptr)
		{
			Player* player = level_->FindActor<Player>("Player");
			ePlayerState state = player->GetState();

			switch (state)
			{
			case ePlayerState::JUMP_LEFT:
				__fallthrough;
			case ePlayerState::JUMP_RIGHT:
			{
				float4 playerSpeed = player->GetSpeed();
				float4 gravitySpeed = player->GetGravitySpeed();
				gravitySpeed = gravitySpeed * -1.f;
				if (gravitySpeed.y < -300.f)
				{
					gravitySpeed.y = -300.f;
				}
				player->SetGravitySpeed(gravitySpeed);
				Pop();
			}
			break;
			case ePlayerState::SPIN_LEFT:
				__fallthrough;
			case ePlayerState::SPIN_RIGHT:
			{
				float4 playerSpeed = player->GetSpeed();
				playerSpeed.x *= 0.9f;
				player->SetSpeed(playerSpeed);
				Pop();
			}
			break;
			default:
				break;
			}
		}
	}
	break;
	case Monitor::eMonitorState::POP:
		if (popRender_->IsAnimationEnd())
		{
			Break();
		}
		break;
	case Monitor::eMonitorState::BREAK:
		if (eMonitor::LIFE == monitorType_)
		{
			if (false == lifeSound_->IsPlaying())
			{
				Player* player = level_->FindActor<Player>("Player");
				player->StartBGM();
				state_ = eMonitorState::DONE;
			}
		}
		else
		{
			state_ = eMonitorState::DONE;
		}
		break;
	case Monitor::eMonitorState::DONE:
		break;
	default:
		break;
	}
}

void Monitor::UpdateAfter()
{
}

void Monitor::Render()
{
	mainRender_->Render();
	popRender_->Render();
}

void Monitor::Cleanup()
{
}

void Monitor::Break()
{
	if (eMonitorState::BREAK == state_)
	{
		return;
	}
	popRender_->Deactivate();

	state_ = eMonitorState::BREAK;

	PlayLevelUI* ui = level_->FindActor<PlayLevelUI>("PlayLevelUI");
	Player* player = level_->FindActor<Player>("Player");
	switch (monitorType_)
	{
	case eMonitor::RING:
		RSSoundManager::GetInstance().PlaySoundByName("ring.wav");
		ui->AddRing(10);
		break;
	case eMonitor::ELECTRIC:
		electricSound_->Play();
		player->SetSkillType(monitorType_);
		break;
	case eMonitor::FIRE:
		fireSound_->Play();
		player->SetSkillType(monitorType_);
		break;
	case eMonitor::BUBBLE:
		bubbleSound_->Play();
		player->SetSkillType(monitorType_);
		break;
	case eMonitor::LIFE:
		player->StopBGM();
		lifeSound_->Play();
		ui->AddLife();
		break;
	default:
		break;
	}
}

void Monitor::Pop()
{
	popRender_->Activate();
	popRender_->ChangeAnimation("Pop", "pop.bmp");
	mainRender_->ChangeAnimation("Break", "monitors.bmp");
	popSound_->Play();
	state_ = eMonitorState::POP;
}

void Monitor::SetMonitorType(eMonitor _monitorType)
{
	monitorType_ = _monitorType;
	if (eMonitorState::BREAK == state_)
	{
		return;
	}

	switch (monitorType_)
	{
	case eMonitor::RING:
		mainRender_->ChangeAnimation("Ring", "monitors.bmp");
		break;
	case eMonitor::ELECTRIC:
		mainRender_->ChangeAnimation("Electric", "monitors.bmp");
		break;
	case eMonitor::FIRE:
		mainRender_->ChangeAnimation("Fire", "monitors.bmp");
		break;
	case eMonitor::BUBBLE:
		mainRender_->ChangeAnimation("Bubble", "monitors.bmp");
		break;
	case eMonitor::LIFE:
		mainRender_->ChangeAnimation("Life", "monitors.bmp");
		break;
	default:
		break;
	}
}

bool Monitor::IsActivate()
{
	return state_ == eMonitorState::ACTIVATE;
}

eMonitor Monitor::GetMonitorEffect()
{
	return monitorType_;
}

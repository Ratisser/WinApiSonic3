#include "FireBreath.h"

#include <RSCollision.h>
#include <RSSoundManager.h>
#include <RSSoundPlayer.h>

#include "ePlayerState.h"
#include "Player.h"
#include "GoalPost.h"
#include "PlayLevelUI.h"

FireBreath::FireBreath()
	: mainRender_(nullptr)
	, splashRender1_(nullptr)
	, splashRender2_(nullptr)
	, splashRender3_(nullptr)
	, splashRender4_(nullptr)
	, flameRender_(nullptr)
	, flameRender1_(nullptr)
	, flameRender2_(nullptr)
	, flameRender3_(nullptr)
	, flameRender4_(nullptr)
	, bodyCollision_(nullptr)
	, flameCollision_(nullptr)
	, destroySound_(nullptr)
	, state_(eFireBreathState::WAIT)
	, bCanHit_(true)
	, bDestroy_(false)
	, hitDelay_(0.0f)
	, hoveringFactor_(0.0f)
	, nextStateDelay_(0.0f)
	, destroyEffectDelay_(0.1f)
	, destroyTime_(2.0f)
	, blinkTime_(0.03f)
	, splashTime_(1.0f)
	, hp_(10)
{
}

FireBreath::~FireBreath()
{
	SAFE_DELETE(mainRender_);
	SAFE_DELETE(splashRender1_);
	SAFE_DELETE(splashRender2_);
	SAFE_DELETE(splashRender3_);
	SAFE_DELETE(splashRender4_);

	SAFE_DELETE(flameRender_);
	SAFE_DELETE(flameRender1_);
	SAFE_DELETE(flameRender2_);
	SAFE_DELETE(flameRender3_);
	SAFE_DELETE(flameRender4_);

	SAFE_DELETE(bodyCollision_);
	SAFE_DELETE(flameCollision_);

	SAFE_DELETE(destroySound_);
}

void FireBreath::Start()
{
	SetRenderOrder(950);
	SetLocation({ 10240.f, 727.f });
	mainRender_ = new RSRenderer(this);
	mainRender_->CreateAnimation("IdleLeft", "middleBossLeft.bmp", 1, 2, 0.033f, true);
	mainRender_->CreateAnimation("IdleRight", "middleBossRight.bmp", 1, 2, 0.033f, true);
	mainRender_->CreateAnimation("DamagedLeft", "middleBossLeft.bmp", 3, 6, 0.033f, true);
	mainRender_->CreateAnimation("DamagedRight", "middleBossRight.bmp", 3, 6, 0.033f, true);
	mainRender_->CreateAnimation("DestroyLeft", "middleBossLeft.bmp", 0, 0, 1.0f, true);
	mainRender_->CreateAnimation("DestroyRight", "middleBossRight.bmp", 0, 0, 1.0f, true);
	mainRender_->ChangeAnimation("IdleLeft", "middleBossLeft.bmp");

	splashRender1_ = new RSRenderer(this);
	splashRender1_->CreateAnimation("SplashLeft1", "middleBossLeft.bmp", 7, 7, 1.0f, true);
	splashRender1_->CreateAnimation("SplashRight1", "middleBossRight.bmp", 7, 7, 1.0f, true);
	splashRender1_->ChangeAnimation("SplashLeft1", "middleBossLeft.bmp");
	splashRender1_->Deactivate();

	splashRender2_ = new RSRenderer(this);
	splashRender2_->CreateAnimation("SplashLeft2", "middleBossLeft.bmp", 8, 8, 1.0f, true);
	splashRender2_->CreateAnimation("SplashRight2", "middleBossRight.bmp", 8, 8, 1.0f, true);
	splashRender2_->ChangeAnimation("SplashLeft2", "middleBossLeft.bmp");
	splashRender2_->Deactivate();

	splashRender3_ = new RSRenderer(this);
	splashRender3_->CreateAnimation("SplashLeft3", "middleBossLeft.bmp", 9, 9, 1.0f, true);
	splashRender3_->CreateAnimation("SplashRight3", "middleBossRight.bmp", 9, 9, 1.0f, true);
	splashRender3_->ChangeAnimation("SplashLeft3", "middleBossLeft.bmp");
	splashRender3_->Deactivate();

	splashRender4_ = new RSRenderer(this);
	splashRender4_->CreateAnimation("SplashLeft4", "middleBossLeft.bmp", 10, 10, 1.0f, true);
	splashRender4_->CreateAnimation("SplashRight4", "middleBossRight.bmp", 10, 10, 1.0f, true);
	splashRender4_->ChangeAnimation("SplashLeft4", "middleBossLeft.bmp");
	splashRender4_->Deactivate();

	mainRender_->ChangeAnimation("IdleLeft", "middleBossLeft.bmp");
	mainRender_->Deactivate();

	flameRender_ = new RSRenderer(this);
	flameRender_->CreateAnimation("FrontFlameLeft", "flameLeft.bmp", 0, 4, 0.1f);
	flameRender_->CreateAnimation("FrontFlameRight", "flameRight.bmp", 0, 4, 0.1f);
	flameRender_->ChangeAnimation("FrontFlameLeft", "flameLeft.bmp");
	flameRender_->Deactivate();

	flameRender1_ = new RSRenderer(this);
	flameRender1_->CreateAnimation("FlameLeft", "flameLeft.bmp", 5, 10, 0.09f);
	flameRender1_->CreateAnimation("FlameRight", "flameRight.bmp", 5, 10, 0.09f);
	flameRender1_->ChangeAnimation("FlameLeft", "flameLeft.bmp");
	flameRender1_->Deactivate();

	flameRender2_ = new RSRenderer(this);
	flameRender2_->CreateAnimation("FlameLeft", "flameLeft.bmp", 5, 10, 0.075f);
	flameRender2_->CreateAnimation("FlameRight", "flameRight.bmp", 5, 10, 0.075f);
	flameRender2_->ChangeAnimation("FlameLeft", "flameLeft.bmp");
	flameRender2_->Deactivate();

	flameRender3_ = new RSRenderer(this);
	flameRender3_->CreateAnimation("FlameLeft", "flameLeft.bmp", 5, 10, 0.06f);
	flameRender3_->CreateAnimation("FlameRight", "flameRight.bmp", 5, 10, 0.06f);
	flameRender3_->ChangeAnimation("FlameLeft", "flameLeft.bmp");
	flameRender3_->Deactivate();

	flameRender4_ = new RSRenderer(this);
	flameRender4_->CreateAnimation("FlameLeft", "flameLeft.bmp", 5, 10, 0.045f);
	flameRender4_->CreateAnimation("FlameRight", "flameRight.bmp", 5, 10, 0.045f);
	flameRender4_->ChangeAnimation("FlameLeft", "flameLeft.bmp");
	flameRender4_->Deactivate();


	bodyCollision_ = new RSCollision(this, eCollisionType::CIRCLE, static_cast<int>(eCollisionCheckGroup::ENEMY));
	bodyCollision_->SetRadius(40.f);

	flameCollision_ = new RSCollision(this, eCollisionType::RECT, static_cast<int>(eCollisionCheckGroup::SPIKE));
	flameCollision_->SetRect(0.0f, 0.0f, 0.0f, 0.0f);

	destroySound_ = new RSSoundPlayer("machineAttacked.wav");
}

void FireBreath::UpdateBefore()
{
	mainRender_->UpdateAnimation();
	flameRender_->UpdateAnimation();
	flameRender1_->UpdateAnimation();
	flameRender2_->UpdateAnimation();
	flameRender3_->UpdateAnimation();
	flameRender4_->UpdateAnimation();

	if (bDestroy_)
	{
		splashRender1_->UpdateAnimation();
		splashRender2_->UpdateAnimation();
		splashRender3_->UpdateAnimation();
		splashRender4_->UpdateAnimation();
	}

	for (RSRenderer* obj : destroyEffectList_)
	{
		obj->UpdateAnimation();
	}
}

void FireBreath::Update()
{
	float deltaTime = RSTimer::GetInstance().GetDeltaTime();
	switch (state_)
	{
	case eFireBreathState::WAIT:
		break;
	case eFireBreathState::APPEAR:
		onAppear(deltaTime);
		break;
	case eFireBreathState::DOWN_LEFT:
		onDownLeft(deltaTime);
		break;
	case eFireBreathState::ATTACK_LEFT:
		onAttackLeft(deltaTime);
		break;
	case eFireBreathState::UP_LEFT:
		onUpLeft(deltaTime);
		break;
	case eFireBreathState::MOVE_LEFT:
		onMoveLeft(deltaTime);
		break;
	case eFireBreathState::DOWN_RIGHT:
		onDownRight(deltaTime);
		break;
	case eFireBreathState::ATTACK_RIGHT:
		onAttackRight(deltaTime);
		break;
	case eFireBreathState::UP_RIGHT:
		onUpRight(deltaTime);
		break;
	case eFireBreathState::MOVE_RIGHT:
		onMoveRight(deltaTime);
		break;
	case eFireBreathState::DESTROY_LEFT:
		onDestroyLeft(deltaTime);
		break;
	case eFireBreathState::DESTROY_RIGHT:
		onDestroyRight(deltaTime);
		break;
	case eFireBreathState::SPLASH_LEFT:
		onSplashLeft(deltaTime);
		break;
	case eFireBreathState::SPLASH_RIGHT:
		onSplashRight(deltaTime);
		break;
	case eFireBreathState::SPLASH_AFTER:
		onSplashAfter(deltaTime);
		break;
	case eFireBreathState::DEACTIVATE:

		break;
	default:
		break;
	}

	hitDelay_ -= deltaTime;
	// 공격받는 판정
	if (false == bDestroy_ && hitDelay_ < 0)
	{
		RSCollision* playerCollision = bodyCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::PLAYER));
		if (playerCollision != nullptr)
		{
			Player* player = level_->FindActor<Player>("Player");
			ePlayerState state = player->GetState();

			switch (state)
			{
			case ePlayerState::JUMP_LEFT:
				[[fallthrough]];
			case ePlayerState::JUMP_RIGHT:
			{
				float4 playerSpeed = player->GetSpeed();
				float4 gravitySpeed = player->GetGravitySpeed();
				playerSpeed.x *= -0.7f;
				gravitySpeed.y *= -1.f;
				if (gravitySpeed.y < -300.f)
				{
					gravitySpeed.y = -300.f;
				}
				player->SetSpeed(playerSpeed);
				player->SetGravitySpeed(gravitySpeed);
				Attacked();
			}
			break;
			case ePlayerState::SPIN_LEFT:
			{
				float4 playerSpeed = player->GetSpeed();
				playerSpeed.x *= -0.7f;
				player->SetMovement(playerSpeed * deltaTime);
				player->SetSpeed(playerSpeed);
				player->ChangeState(ePlayerState::SPIN_RIGHT);
				Attacked();
			}
			break;
			case ePlayerState::SPIN_RIGHT:
			{
				float4 playerSpeed = player->GetSpeed();
				playerSpeed.x *= -0.7f;
				player->SetMovement(playerSpeed * deltaTime);
				player->SetSpeed(playerSpeed);
				player->ChangeState(ePlayerState::SPIN_LEFT);
				Attacked();
			}
			break;
			default:
				player->Attacked();
				break;
			}
		}
	}

	if (false == bDestroy_)
	{
		if (hitDelay_ > 0)
		{
			if (IsLeftSide())
			{
				mainRender_->ChangeAnimation("DamagedLeft", "middleBossLeft.bmp");
			}
			else
			{
				mainRender_->ChangeAnimation("DamagedRight", "middleBossRight.bmp");
			}
		}
		else
		{
			if (IsLeftSide())
			{
				mainRender_->ChangeAnimation("IdleLeft", "middleBossLeft.bmp");
			}
			else
			{
				mainRender_->ChangeAnimation("IdleRight", "middleBossRight.bmp");
			}
		}
	}
}

void FireBreath::UpdateAfter()
{
	std::list<RSRenderer*>::iterator startIter = destroyEffectList_.begin();
	std::list<RSRenderer*>::iterator endIter = destroyEffectList_.end();

	while (startIter != endIter)
	{
		RSRenderer* currentObject = *startIter;
		if (currentObject->IsAnimationEnd())
		{
			delete currentObject;
			currentObject = nullptr;
			startIter = destroyEffectList_.erase(startIter);
		}
		else
		{
			++startIter;
		}
	}
}

void FireBreath::Render()
{
	mainRender_->Render();
	flameRender_->Render();
	flameRender1_->Render();
	flameRender2_->Render();
	flameRender3_->Render();
	flameRender4_->Render();


	if (bDestroy_)
	{
		splashRender1_->Render();
		splashRender2_->Render();
		splashRender3_->Render();
		splashRender4_->Render();
	}

	for (RSRenderer* obj : destroyEffectList_)
	{
		obj->Render();
	}
}

void FireBreath::Cleanup()
{
}

void FireBreath::Appear()
{
	mainRender_->Activate();
	mainRender_->ChangeAnimation("IdleLeft", "middleBossLeft.bmp");
	nextStateDelay_ = 3.0f;
	state_ = eFireBreathState::APPEAR;
}

void FireBreath::Attacked()
{
	RSSoundManager::GetInstance().PlaySoundByName("machineAttacked.wav");
	hitDelay_ = 0.7f;
	--hp_;

	if (hp_ < 1)
	{
		bDestroy_ = true;
		destroyTime_ = 2.5f;
		destroyEffectDelay_ = 0.1f;

		if (IsLeftSide())
		{
			mainRender_->ChangeAnimation("DestroyLeft", "middleBossLeft.bmp", true);
			splashRender1_->ChangeAnimation("SplashLeft1", "middleBossLeft.bmp", true);
			splashRender2_->ChangeAnimation("SplashLeft2", "middleBossLeft.bmp", true);
			splashRender3_->ChangeAnimation("SplashLeft3", "middleBossLeft.bmp", true);
			splashRender4_->ChangeAnimation("SplashLeft4", "middleBossLeft.bmp", true);
			state_ = eFireBreathState::DESTROY_LEFT;
		}
		else
		{
			mainRender_->ChangeAnimation("DestroyRight", "middleBossRight.bmp", true);
			splashRender1_->ChangeAnimation("SplashRight1", "middleBossRight.bmp", true);
			splashRender2_->ChangeAnimation("SplashRight2", "middleBossRight.bmp", true);
			splashRender3_->ChangeAnimation("SplashRight3", "middleBossRight.bmp", true);
			splashRender4_->ChangeAnimation("SplashRight4", "middleBossRight.bmp", true);
			state_ = eFireBreathState::DESTROY_RIGHT;
		}
	}
}

bool FireBreath::IsLeftSide()
{
	switch (state_)
	{
	case eFireBreathState::APPEAR:
		[[fallthrough]];
	case eFireBreathState::DOWN_LEFT:
		[[fallthrough]];
	case eFireBreathState::ATTACK_LEFT:
		[[fallthrough]];
	case eFireBreathState::UP_LEFT:
		[[fallthrough]];
	case eFireBreathState::MOVE_LEFT:
		return true;
		break;
	default:
		break;
	}
	return false;
}

void FireBreath::onAppear(float _deltaTime)
{
	nextStateDelay_ -= _deltaTime;
	SetMovement(float4::DOWN * 53.f * _deltaTime);

	if (nextStateDelay_ <= 0)
	{
		state_ = eFireBreathState::ATTACK_LEFT;
		hoveringFactor_ = 0.0f;
		nextStateDelay_ = 6.0f;
	}
}

void FireBreath::onDownLeft(float _deltaTime)
{
	nextStateDelay_ -= _deltaTime;
	SetMovement(float4::DOWN * 53.f * _deltaTime);

	if (nextStateDelay_ <= 0)
	{
		state_ = eFireBreathState::ATTACK_LEFT;
		hoveringFactor_ = 0.0f;
		nextStateDelay_ = 6.0f;
	}
}

void FireBreath::onAttackLeft(float _deltaTime)
{
	hoveringFactor_ += TORAD(360.f) * _deltaTime;
	nextStateDelay_ -= _deltaTime;
	SetMovement({ 0.0f, cosf(hoveringFactor_) * 30.f * _deltaTime });

	if (nextStateDelay_ <= 4.f && nextStateDelay_ > 3.95f)
	{
		flameRender_->SetPivot({ -45.f, 0.0f });
		flameRender_->ChangeAnimation("FrontFlameLeft", "flameLeft.bmp");
		flameRender_->Activate();

		flameCollision_->SetRect(flameRender_->GetPivot().x - 16.f, -16.0f, 0.0f, 16.0f);
	}

	if (nextStateDelay_ <= 3.95f && nextStateDelay_ > 3.90f)
	{
		flameRender_->SetPivot({ -65.f, 0.0f });
		flameRender1_->SetPivot({ -45.f, 0.0f });
		flameRender1_->ChangeAnimation("FlameLeft", "flameLeft.bmp");
		flameRender1_->Activate();

		flameCollision_->SetRect(flameRender_->GetPivot().x - 16.f, -16.0f, 0.0f, 16.0f);
	}

	if (nextStateDelay_ <= 3.90f && nextStateDelay_ > 3.85f)
	{
		flameRender_->SetPivot({ -85, 0.0f });
		flameRender1_->SetPivot({ -65.f, 0.0f });
		flameRender2_->SetPivot({ -45.f, 0.0f });
		flameRender2_->ChangeAnimation("FlameLeft", "flameLeft.bmp");
		flameRender2_->Activate();

		flameCollision_->SetRect(flameRender_->GetPivot().x - 16.f, -16.0f, 0.0f, 16.0f);
	}

	if (nextStateDelay_ <= 3.85f && nextStateDelay_ > 3.80f)
	{
		flameRender_->SetPivot({ -105.f, 0.0f });
		flameRender1_->SetPivot({ -85, 0.0f });
		flameRender2_->SetPivot({ -65.f, 0.0f });
		flameRender3_->SetPivot({ -45.f, 0.0f });
		flameRender3_->ChangeAnimation("FlameLeft", "flameLeft.bmp");
		flameRender3_->Activate();

		flameCollision_->SetRect(flameRender_->GetPivot().x - 16.f, -16.0f, 0.0f, 16.0f);
	}

	if (nextStateDelay_ <= 3.80f && nextStateDelay_ > 3.75f)
	{
		flameRender_->SetPivot({ -125.f, 0.0f });
		flameRender1_->SetPivot({ -105.f, 0.0f });
		flameRender2_->SetPivot({ -85, 0.0f });
		flameRender3_->SetPivot({ -65.f, 0.0f });
		flameRender4_->SetPivot({ -45.f, 0.0f });
		flameRender4_->ChangeAnimation("FlameLeft", "flameLeft.bmp");
		flameRender4_->Activate();

		flameCollision_->SetRect(flameRender_->GetPivot().x - 16.f, -16.0f, 0.0f, 16.0f);
	}

	if (nextStateDelay_ <= 3.4f)
	{
		if (flameRender_->IsAnimationEnd())
		{
			flameRender_->Deactivate();
		}
		if (flameRender1_->IsAnimationEnd())
		{
			flameRender1_->Deactivate();
		}
		if (flameRender2_->IsAnimationEnd())
		{
			flameRender2_->Deactivate();
		}
		if (flameRender3_->IsAnimationEnd())
		{
			flameRender3_->Deactivate();
		}
		if (flameRender4_->IsAnimationEnd())
		{
			flameRender4_->Deactivate();
		}

		flameCollision_->SetRect(0.0f, 0.0f, 0.0f, 0.0f);
	}

	if (nextStateDelay_ <= 0)
	{
		state_ = eFireBreathState::UP_LEFT;
		hoveringFactor_ = 0.0f;
		nextStateDelay_ = 2.0f;
	}

	RSCollision* playerCollision = flameCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::PLAYER));
	if (nullptr != playerCollision)
	{
		Player* player = dynamic_cast<Player*>(playerCollision->GetActor());

		player->Attacked();
	}
}

void FireBreath::onUpLeft(float _deltaTime)
{
	nextStateDelay_ -= _deltaTime;
	SetMovement(float4::UP * 53.f * _deltaTime);

	if (nextStateDelay_ <= 0)
	{
		state_ = eFireBreathState::MOVE_LEFT;
		hoveringFactor_ = 0.0f;
		nextStateDelay_ = 3.0f;
	}
}

void FireBreath::onMoveLeft(float _deltaTime)
{
	hoveringFactor_ += TORAD(360.f) * _deltaTime;
	nextStateDelay_ -= _deltaTime;
	SetMovement(float4::LEFT * 70.f * _deltaTime);
	SetMovement({ 0.0f, cosf(hoveringFactor_) * 30.f * _deltaTime });


	if (nextStateDelay_ <= 0)
	{
		state_ = eFireBreathState::DOWN_RIGHT;
		mainRender_->ChangeAnimation("IdleRight", "middleBossRight.bmp");
		hoveringFactor_ = 0.0f;
		nextStateDelay_ = 2.0f;
	}
}

void FireBreath::onDownRight(float _deltaTime)
{
	nextStateDelay_ -= _deltaTime;
	SetMovement(float4::DOWN * 53.f * _deltaTime);

	if (nextStateDelay_ <= 0)
	{
		state_ = eFireBreathState::ATTACK_RIGHT;
		hoveringFactor_ = 0.0f;
		nextStateDelay_ = 6.0f;
	}
}

void FireBreath::onAttackRight(float _deltaTime)
{
	hoveringFactor_ += TORAD(360.f) * _deltaTime;
	nextStateDelay_ -= _deltaTime;
	SetMovement({ 0.0f, cosf(hoveringFactor_) * 30.f * _deltaTime });

	if (nextStateDelay_ <= 4.f && nextStateDelay_ > 3.95f)
	{
		flameRender_->SetPivot({ 45.f, 0.0f });
		flameRender_->ChangeAnimation("FrontFlameRight", "flameRight.bmp");
		flameRender_->Activate();

		flameCollision_->SetRect(0.0f, -16.0f, flameRender_->GetPivot().x + 16.f, 16.0f);
	}

	if (nextStateDelay_ <= 3.95f && nextStateDelay_ > 3.90f)
	{
		flameRender_->SetPivot({ 65.f, 0.0f });
		flameRender1_->SetPivot({ 45.f, 0.0f });
		flameRender1_->ChangeAnimation("FlameRight", "flameRight.bmp");
		flameRender1_->Activate();

		flameCollision_->SetRect(0.0f, -16.0f, flameRender_->GetPivot().x + 16.f, 16.0f);
	}

	if (nextStateDelay_ <= 3.90f && nextStateDelay_ > 3.85f)
	{
		flameRender_->SetPivot({ 85, 0.0f });
		flameRender1_->SetPivot({ 65.f, 0.0f });
		flameRender2_->SetPivot({ 45.f, 0.0f });
		flameRender2_->ChangeAnimation("FlameRight", "flameRight.bmp");
		flameRender2_->Activate();

		flameCollision_->SetRect(0.0f, -16.0f, flameRender_->GetPivot().x + 16.f, 16.0f);
	}

	if (nextStateDelay_ <= 3.85f && nextStateDelay_ > 3.80f)
	{
		flameRender_->SetPivot({ 105.f, 0.0f });
		flameRender1_->SetPivot({ 85, 0.0f });
		flameRender2_->SetPivot({ 65.f, 0.0f });
		flameRender3_->SetPivot({ 45.f, 0.0f });
		flameRender3_->ChangeAnimation("FlameRight", "flameRight.bmp");
		flameRender3_->Activate();

		flameCollision_->SetRect(0.0f, -16.0f, flameRender_->GetPivot().x + 16.f, 16.0f);
	}

	if (nextStateDelay_ <= 3.80f && nextStateDelay_ > 3.75f)
	{
		flameRender_->SetPivot({ 125.f, 0.0f });
		flameRender1_->SetPivot({ 105.f, 0.0f });
		flameRender2_->SetPivot({ 85, 0.0f });
		flameRender3_->SetPivot({ 65.f, 0.0f });
		flameRender4_->SetPivot({ 45.f, 0.0f });
		flameRender4_->ChangeAnimation("FlameRight", "flameRight.bmp");
		flameRender4_->Activate();

		flameCollision_->SetRect(0.0f, -16.0f, flameRender_->GetPivot().x + 16.f, 16.0f);
	}

	if (nextStateDelay_ <= 3.4f)
	{
		if (flameRender_->IsAnimationEnd())
		{
			flameRender_->Deactivate();
		}
		if (flameRender1_->IsAnimationEnd())
		{
			flameRender1_->Deactivate();
		}
		if (flameRender2_->IsAnimationEnd())
		{
			flameRender2_->Deactivate();
		}
		if (flameRender3_->IsAnimationEnd())
		{
			flameRender3_->Deactivate();
		}
		if (flameRender4_->IsAnimationEnd())
		{
			flameRender4_->Deactivate();
		}

		flameCollision_->SetRect(0.0f, 0.0f, 0.0f, 0.0f);
	}

	if (nextStateDelay_ <= 0)
	{
		state_ = eFireBreathState::UP_RIGHT;
		hoveringFactor_ = 0.0f;
		nextStateDelay_ = 2.0f;
	}

	RSCollision* playerCollision = flameCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::PLAYER));
	if (nullptr != playerCollision)
	{
		Player* player = dynamic_cast<Player*>(playerCollision->GetActor());

		player->Attacked();
	}

}

void FireBreath::onUpRight(float _deltaTime)
{
	nextStateDelay_ -= _deltaTime;
	SetMovement(float4::UP * 53.f * _deltaTime);

	if (nextStateDelay_ <= 0)
	{
		state_ = eFireBreathState::MOVE_RIGHT;
		hoveringFactor_ = 0.0f;
		nextStateDelay_ = 3.0f;
	}
}

void FireBreath::onMoveRight(float _deltaTime)
{
	hoveringFactor_ += TORAD(360.f) * _deltaTime;
	nextStateDelay_ -= _deltaTime;
	SetMovement(float4::RIGHT * 70.f * _deltaTime);
	SetMovement({ 0.0f, cosf(hoveringFactor_) * 30.f * _deltaTime });

	if (nextStateDelay_ <= 0)
	{
		state_ = eFireBreathState::DOWN_LEFT;
		mainRender_->ChangeAnimation("IdleLeft", "middleBossLeft.bmp");
		hoveringFactor_ = 0.0f;
		nextStateDelay_ = 2.0f;
	}
}

void FireBreath::onDestroyLeft(float _deltaTime)
{
	mainRender_->ChangeAnimation("DestroyLeft", "middleBossLeft.bmp", true);

	destroyEffectDelay_ -= _deltaTime;
	destroyTime_ -= _deltaTime;

	if (destroyEffectDelay_ < 0)
	{
		destroySound_->Stop();
		destroySound_->Play();
		RSRandom random;
		float x = random.RandomFloat(0.0f, 80.f) - 40.f;
		float y = random.RandomFloat(0.0f, 80.f) - 40.f;
		RSRenderer* effect = new RSRenderer(this);
		effect->SetPivot({ x, y });
		effect->CreateAnimation("flame", "flameLeft.bmp", 6, 10, 0.03f);
		effect->ChangeAnimation("flame", "flameLeft.bmp");
		destroyEffectList_.push_back(effect);

		if (destroyTime_ < 0.7f)
		{
			destroyEffectDelay_ = 0.08f;
		}
		else
		{
			destroyEffectDelay_ = 0.1f;
		}
	}

	if (destroyTime_ < 0.0f)
	{
		mainRender_->Deactivate();
		splashRender1_->Activate();
		splashRender2_->Activate();
		splashRender3_->Activate();
		splashRender4_->Activate();
		splashTime_ = 0.5f;
		state_ = eFireBreathState::SPLASH_LEFT;
	}
}

void FireBreath::onDestroyRight(float _deltaTime)
{
	mainRender_->ChangeAnimation("DestroyRight", "middleBossRight.bmp", true);

	destroyEffectDelay_ -= _deltaTime;
	destroyTime_ -= _deltaTime;

	if (destroyEffectDelay_ < 0)
	{
		destroySound_->Stop();
		destroySound_->Play();
		RSRandom random;
		float x = random.RandomFloat(0.0f, 80.f) - 40.f;
		float y = random.RandomFloat(0.0f, 80.f) - 40.f;
		RSRenderer* effect = new RSRenderer(this);
		effect->SetPivot({ x, y });
		effect->CreateAnimation("flame", "flameLeft.bmp", 6, 10, 0.03f);
		effect->ChangeAnimation("flame", "flameLeft.bmp");
		destroyEffectList_.push_back(effect);

		if (destroyTime_ < 0.7f)
		{
			destroyEffectDelay_ = 0.08f;
		}
		else
		{
			destroyEffectDelay_ = 0.1f;
		}
	}

	if (destroyTime_ < 0.0f)
	{
		mainRender_->Deactivate();
		splashRender1_->Activate();
		splashRender2_->Activate();
		splashRender3_->Activate();
		splashRender4_->Activate();
		splashTime_ = 0.5f;
		state_ = eFireBreathState::SPLASH_RIGHT;
	}
}

void FireBreath::onSplashLeft(float _deltaTime)
{
	static float x = 0.0f;
	static float y = -100.0f;
	static float y2 = -50.0f;

	splashTime_ -= _deltaTime;
	blinkTime_ -= _deltaTime;

	x += 100.f * _deltaTime;
	y += 200.f * _deltaTime;
	y2 += 200.f * _deltaTime;

	float4 splash1 = splashRender1_->GetPivot();
	float4 splash2 = splashRender2_->GetPivot();
	float4 splash3 = splashRender3_->GetPivot();
	float4 splash4 = splashRender4_->GetPivot();

	splash1.x -= x * _deltaTime;
	splash1.y += y * _deltaTime;
	splash2.x += x * _deltaTime;
	splash2.y += y * _deltaTime;
	splash3.x -= x * _deltaTime;
	splash3.y += y2 * _deltaTime;
	splash4.x += x * _deltaTime;
	splash4.y += y2 * _deltaTime;

	splashRender1_->SetPivot(splash1);
	splashRender2_->SetPivot(splash2);
	splashRender3_->SetPivot(splash3);
	splashRender4_->SetPivot(splash4);

	if (blinkTime_ < 0.0f)
	{
		blinkTime_ = 0.03f;

		splashRender1_->SwitchActivation();
		splashRender2_->SwitchActivation();
		splashRender3_->SwitchActivation();
		splashRender4_->SwitchActivation();
	}

	if (splashTime_ < 0.0f)
	{
		splashRender1_->Deactivate();
		splashRender2_->Deactivate();
		splashRender3_->Deactivate();
		splashRender4_->Deactivate();
		nextStateDelay_ = 2.0f;
		Player* player = level_->FindActor<Player>("Player");
		player->ExitBossAreaBGM();
		player->StopBGM();
		state_ = eFireBreathState::SPLASH_AFTER;
	}
}

void FireBreath::onSplashRight(float _deltaTime)
{
	static float x = 0.0f;
	static float y = -100.0f;
	static float y2 = -50.0f;

	splashTime_ -= _deltaTime;
	blinkTime_ -= _deltaTime;

	x += 100.f * _deltaTime;
	y += 200.f * _deltaTime;
	y2 += 200.f * _deltaTime;

	float4 splash1 = splashRender1_->GetPivot();
	float4 splash2 = splashRender2_->GetPivot();
	float4 splash3 = splashRender3_->GetPivot();
	float4 splash4 = splashRender4_->GetPivot();

	splash1.x += x * _deltaTime;
	splash1.y += y * _deltaTime;
	splash2.x -= x * _deltaTime;
	splash2.y += y * _deltaTime;
	splash3.x += x * _deltaTime;
	splash3.y += y2 * _deltaTime;
	splash4.x -= x * _deltaTime;
	splash4.y += y2 * _deltaTime;

	splashRender1_->SetPivot(splash1);
	splashRender2_->SetPivot(splash2);
	splashRender3_->SetPivot(splash3);
	splashRender4_->SetPivot(splash4);

	if (blinkTime_ < 0.0f)
	{
		blinkTime_ = 0.03f;

		splashRender1_->SwitchActivation();
		splashRender2_->SwitchActivation();
		splashRender3_->SwitchActivation();
		splashRender4_->SwitchActivation();
	}

	if (splashTime_ < 0.0f)
	{
		splashRender1_->Deactivate();
		splashRender2_->Deactivate();
		splashRender3_->Deactivate();
		splashRender4_->Deactivate();
		nextStateDelay_ = 2.0f;
		Player* player = level_->FindActor<Player>("Player");
		player->ExitBossAreaBGM();
		player->StopBGM();
		state_ = eFireBreathState::SPLASH_AFTER;
	}
}

void FireBreath::onSplashAfter(float _deltaTime)
{
	nextStateDelay_ -= _deltaTime;
	if (nextStateDelay_ < 0.0f)
	{
		Player* player = level_->FindActor<Player>("Player");
		player->ChangeBGM("stage1-1.mid");
		player->StartBGM();

		GoalPost* goalPost = level_->FindActor<GoalPost>("GoalPost");
		goalPost->Drop({ location_.x, 727.f });

		PlayLevelUI* ui = level_->FindActor<PlayLevelUI>("PlayLevelUI");
		ui->StopTime();

		state_ = eFireBreathState::DEACTIVATE;
	}
}


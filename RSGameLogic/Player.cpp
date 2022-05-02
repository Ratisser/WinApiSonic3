#include "Player.h"
#include "PlayLevel.h"
#include "eCollisionCheckColor.h"
#include "eCollisionCheckGroup.h"
#include "Monitor.h"
#include "PlayerSkill.h"
#include "Ring.h"
#include "Wood.h"
#include "PlayLevelUI.h"

#include <RSCollision.h>
#include <RSResourceManager.h>
#include <RSSoundPlayer.h>
#include <RSMath.h>

Player::Player()
	: direction_(float4::RIGHT)
	, upVector_(float4::UP)
	, forwardVector_(float4::RIGHT)
	, playerSpeed_(0.0f, 0.0f)
	, accelation_(0.0f)
	, gravityAccelation_(0.0f, 0.0f)
	, fActualSpeedLimit_(2000.f)
	, bGround_(false)
	, bCanJump_(true)
	, bLeftAnglePossible_(false)
	, bRightAnglePossible_(false)
	, bAngled_(false)
	, bSpin_(false)
	, bRolling_(false)
	, bRollRight_(false)
	, bRollLeft_(false)
	, bRollUp_(false)
	, playerState_(ePlayerState::WAIT)
	, nextState_(ePlayerState::WAIT)
	, mainRender_(nullptr)
	, bottomCenterCollision_(nullptr)
	, bottomLeftCollision_(nullptr)
	, bottomRightCollision_(nullptr)
	, topCenterCollision_(nullptr)
	, centerLeftCollision_(nullptr)
	, centerRightCollision_(nullptr)
	, bottomCenterRollLeftCollision_(nullptr)
	, bottomCenterRollRightCollision_(nullptr)
	, bottomCenterRollUpCollision_(nullptr)
	, bottomCenterRollPostCollision_(nullptr)
	, rollCheckSideCollision_(nullptr)
	, bottomRightRollCollision_(nullptr)
	, bottomLeftRollCollision_(nullptr)
	, groundCheckCollision_(nullptr)
	, groundCheckRollCollision_(nullptr)
	, jumpBoardCheckCollision_(nullptr)
	, woodRunCheckCollision_(nullptr)
	, woodRunFinishCheckCollision_(nullptr)
	, electricFieldCollision_(nullptr)
	, currentStateTime_(0.0f)
	, delayForNextState_(0.0f)
	, bgmPlayer_(nullptr)
	, jumpSound_(nullptr)
	, rollingSound_(nullptr)
	, spinSound_(nullptr)
	, spinDashSound_(nullptr)
	, jumpTime_(0.0)
	, rotation_(0.0f)
	, lerpRatio_(0.0f)
	, woodRunTime_(0.0f)
	, bDebugRender_(false)
	, skillType_(eMonitor::RING)
	, bUsingBubbleSkill_(false)
	, bUsingElectricSkill_(false)
	, bUsingFireSkill_(false)
	, skill_(nullptr)
	, skillPosition_(float4::ZERO)
	, bLeftRollStartForSkillLocation_(false)
	, bInvincible_(false)
	, bBlink_(false)
	, blinkCooltime_(0.0f)
	, blinkTime_(0.0f)
	, invincibleTime_(0.0f)
	, bBossArea_(false)
	, bBossAreaBGM_(false)
{
	ringsReadyToDrop_.reserve(200);

	stateNameMap_[ePlayerState::WAIT] = "Wait";
	stateNameMap_[ePlayerState::DELAY] = "Delay";
	stateNameMap_[ePlayerState::IDLE_LEFT] = "IdleLeft";
	stateNameMap_[ePlayerState::IDLE_RIGHT] = "IdleRight";
	stateNameMap_[ePlayerState::BORING_LEFT] = "BoringLeft";
	stateNameMap_[ePlayerState::BORING_RIGHT] = "BoringRight";
	stateNameMap_[ePlayerState::WALK_LEFT] = "WalkLeft";
	stateNameMap_[ePlayerState::WALK_LEFT_ANGLED] = "WalkLeftAngled";
	stateNameMap_[ePlayerState::WALK_RIGHT] = "WalkRight";
	stateNameMap_[ePlayerState::WALK_RIGHT_ANGLED] = "WalkRightAngled";
	stateNameMap_[ePlayerState::FULLSPEED_LEFT] = "FullSpeedLeft";
	stateNameMap_[ePlayerState::FULLSPEED_LEFT_ANGLED] = "FullSpeedLeftAngled";
	stateNameMap_[ePlayerState::FULLSPEED_RIGHT] = "FullSpeedRight";
	stateNameMap_[ePlayerState::FULLSPEED_RIGHT_ANGLED] = "FullSpeedRightAngled";
	stateNameMap_[ePlayerState::ROLLING_LEFT] = "RollingLeft";
	stateNameMap_[ePlayerState::ROLLING_RIGHT] = "RollingRight";
	stateNameMap_[ePlayerState::ROLLING_UP] = "RollingUp";
	stateNameMap_[ePlayerState::JUMP_LEFT] = "JumpLeft";
	stateNameMap_[ePlayerState::JUMP_RIGHT] = "JumpRight";
	stateNameMap_[ePlayerState::JUMP_SHIELD_LEFT] = "JumpShieldLeft";
	stateNameMap_[ePlayerState::JUMP_SHIELD_RIGHT] = "JumpShieldRight";
	stateNameMap_[ePlayerState::SKID_LEFT] = "SkidLeft";
	stateNameMap_[ePlayerState::SKID_RIGHT] = "SkidRight";
	stateNameMap_[ePlayerState::PUSH_LEFT] = "PushLeft";
	stateNameMap_[ePlayerState::PUSH_RIGHT] = "PushRight";
	stateNameMap_[ePlayerState::FALLING_LEFT] = "FallingLeft";
	stateNameMap_[ePlayerState::FALLING_RIGHT] = "FallingRight";
	stateNameMap_[ePlayerState::FALLING_UP] = "FallingUp";
	stateNameMap_[ePlayerState::JUMP_BOARD] = "JumpBoard";
	stateNameMap_[ePlayerState::WOOD_RUNNING] = "WoodRunning";
	stateNameMap_[ePlayerState::CROUCH_LEFT] = "CrouchLeft";
	stateNameMap_[ePlayerState::CROUCH_RIGHT] = "CrouchRight";
	stateNameMap_[ePlayerState::LOOK_UP_LEFT] = "LookUpLeft";
	stateNameMap_[ePlayerState::LOOK_UP_RIGHT] = "LookUpRight";
	stateNameMap_[ePlayerState::SPIN_LEFT] = "SpinLeft";
	stateNameMap_[ePlayerState::SPIN_RIGHT] = "SpinRight";
	stateNameMap_[ePlayerState::SPIN_DASH_LEFT] = "SpinDashLeft";
	stateNameMap_[ePlayerState::SPIN_DASH_RIGHT] = "SpinDashRight";
	stateNameMap_[ePlayerState::PUSH_LEFT] = "PushLeft";
	stateNameMap_[ePlayerState::PUSH_RIGHT] = "PushRight";
	stateNameMap_[ePlayerState::SPRING_JUMP_LEFT] = "SpringJumpLeft";
	stateNameMap_[ePlayerState::SPRING_JUMP_RIGHT] = "SpringJumpRight";
	stateNameMap_[ePlayerState::DAMAGED_LEFT] = "DamagedLeft";
	stateNameMap_[ePlayerState::DAMAGED_RIGHT] = "DamagedRight";
	stateNameMap_[ePlayerState::STAGE_CLEAR] = "StageClear";
}

Player::~Player()
{
	SAFE_DELETE(mainRender_);

	SAFE_DELETE(bottomCenterCollision_);
	SAFE_DELETE(bottomRightCollision_);
	SAFE_DELETE(bottomLeftCollision_);
	SAFE_DELETE(topCenterCollision_);
	SAFE_DELETE(centerLeftCollision_);
	SAFE_DELETE(centerRightCollision_);
	SAFE_DELETE(bottomCenterRollRightCollision_);
	SAFE_DELETE(bottomCenterRollLeftCollision_);
	SAFE_DELETE(bottomCenterRollPostCollision_);
	SAFE_DELETE(bottomCenterRollUpCollision_);
	SAFE_DELETE(rollCheckSideCollision_);
	SAFE_DELETE(bottomRightRollCollision_);
	SAFE_DELETE(bottomLeftRollCollision_);
	SAFE_DELETE(groundCheckRollCollision_);
	SAFE_DELETE(groundCheckCollision_);
	SAFE_DELETE(woodRunCheckCollision_);
	SAFE_DELETE(woodRunFinishCheckCollision_);
	SAFE_DELETE(jumpBoardCheckCollision_);
	SAFE_DELETE(electricFieldCollision_);

	SAFE_DELETE(bgmPlayer_);
	SAFE_DELETE(jumpSound_);
	SAFE_DELETE(rollingSound_);
	SAFE_DELETE(spinSound_);
	SAFE_DELETE(spinDashSound_);
}

void Player::Start()
{
	SetLocation({ 2880.0f, 1096.0f });
	SetRenderOrder(1000);

	initInput();
	initRendererAndAnimation();
	initCollision();
	initSoundPlayer();

	level_->SetCamPos(location_ - float4(0.0f, 50.f));
}

void Player::UpdateBefore()
{
	float deltaTime = RSTimer::GetInstance().GetDeltaTime();
	{
		if (true == RSInput::GetInstance().IsKeyDown("TestKey1"))
		{
			SetLocation({ 2880.0f, 1096.0f });
			//SetLocation({ 10000.0f, 934.0f });
			ResetState();
		}

		if (true == RSInput::GetInstance().IsKeyDown("TestKey2"))
		{
			SetLocation({ 4154.f, 1150.f });
			ResetState();
		}

		if (true == RSInput::GetInstance().IsKeyDown("TestKey3"))
		{
			SetLocation({ 5575.f, 707.f });
			ResetState();
		}

		if (true == RSInput::GetInstance().IsKeyDown("TestKey4"))
		{
			SetLocation({ 6540.f, 1160.f });
			ResetState();
		}

		if (true == RSInput::GetInstance().IsKeyDown("TestKey5"))
		{
			SetLocation({ 6973.f, 1280.f });
			ResetState();
		}

		if (true == RSInput::GetInstance().IsKeyDown("TestKey6"))
		{
			SetLocation({ 8802.f, 1156.f });
			ResetState();
		}

		if (true == RSInput::GetInstance().IsKeyDown("TestKey7"))
		{
			SetLocation({ 9088.f, 1012.f });
			ResetState();
		}
	}

	if (playerState_ == ePlayerState::WAIT)
	{
		return;
	}

	mainRender_->UpdateAnimation();

	RSCollision* collision = bottomCenterCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::LOCATION));
	if (bBossArea_)
	{
		float4 cam = level_->GetCamPos();
		const float4 target = { 9976.f + 160.f, 696.f + 150.f };
		float4 direction = cam.Direction(target);

		if (abs(cam.x - target.x) > 1.0f || abs(cam.y - target.y) > 1.0f)
		{
			level_->SetCamMovement(direction * 300.f * deltaTime);
		}
	}
	else if (nullptr != collision && collision->GetActor()->GetName() == std::string("StartLocation"))
	{
		float4 cam = level_->GetCamPos();
		float4 target = { 2792.f + 160.f, 920.f + 120.f };
		float4 direction = cam.Direction(target);

		if (abs(cam.x - target.x) > 2.0f || abs(cam.y - target.y) > 2.0f)
		{
			level_->SetCamMovement(direction * 500.f * deltaTime);
		}
	}
	else
	{
	
		float4 cam = level_->GetCamPos();
		float4 target = location_;
		target.y -= 50;
		float4 direction = cam.Direction(target);

		if (abs(cam.x - target.x) > 2.0f || abs(cam.y - target.y) > 2.0f)
		{
			level_->SetCamMovement(direction * 800.f * deltaTime);
		}
		else
		{
			level_->SetCamPos(location_ + float4(0.0f, -50.f));
		}
	}

	if (bBossAreaBGM_)
	{
		if (false == bgmPlayer_->IsPlaying())
		{
			bgmPlayer_->Play();
			bgmPlayer_->SetPosition(1800U);
		}
	}
}

void Player::Update()
{
	float deltaTime = RSTimer::GetInstance().GetDeltaTime();
	currentStateTime_ += deltaTime;
	delayForNextState_ -= deltaTime;

	switch (playerState_)
	{
	case ePlayerState::WAIT:
		onWait(deltaTime);
		break;
	case ePlayerState::DELAY:
		onDelay(deltaTime);
		break;
	case ePlayerState::IDLE_LEFT:
		onIdleLeft(deltaTime);
		break;
	case ePlayerState::IDLE_RIGHT:
		onIdleRight(deltaTime);
		break;
	case ePlayerState::BORING_LEFT:
		onBoringLeft(deltaTime);
		break;
	case ePlayerState::BORING_RIGHT:
		onBoringRight(deltaTime);
		break;
	case ePlayerState::WALK_LEFT:
		onWalkLeft(deltaTime);
		break;
	case ePlayerState::WALK_LEFT_ANGLED:
		onWalkLeftAngled(deltaTime);
		break;
	case ePlayerState::WALK_RIGHT:
		onWalkRight(deltaTime);
		break;
	case ePlayerState::WALK_RIGHT_ANGLED:
		onWalkRightAngled(deltaTime);
		break;
	case ePlayerState::FULLSPEED_LEFT:
		onFullSpeedLeft(deltaTime);
		break;
	case ePlayerState::FULLSPEED_LEFT_ANGLED:
		onFullSpeedLeftAngled(deltaTime);
		break;
	case ePlayerState::FULLSPEED_RIGHT:
		onFullSpeedRight(deltaTime);
		break;
	case ePlayerState::FULLSPEED_RIGHT_ANGLED:
		onFullSpeedRightAngled(deltaTime);
		break;
	case ePlayerState::ROLLING_LEFT:
		onRollingLeft(deltaTime);
		break;
	case ePlayerState::ROLLING_RIGHT:
		onRollingRight(deltaTime);
		break;
	case ePlayerState::ROLLING_UP:
		onRollingUp(deltaTime);
		break;
	case ePlayerState::JUMP_LEFT:
		onJumpLeft(deltaTime);
		break;
	case ePlayerState::JUMP_RIGHT:
		onJumpRight(deltaTime);
		break;
	case ePlayerState::JUMP_SHIELD_LEFT:
		onJumpShieldLeft(deltaTime);
		break;
	case ePlayerState::JUMP_SHIELD_RIGHT:
		onJumpShieldRight(deltaTime);
		break;
	case ePlayerState::SKID_LEFT:
		onSkidLeft(deltaTime);
		break;
	case ePlayerState::SKID_RIGHT:
		onSkidRight(deltaTime);
		break;
	case ePlayerState::PUSH_LEFT:
		onPushLeft(deltaTime);
		break;
	case ePlayerState::PUSH_RIGHT:
		onPushRight(deltaTime);
		break;
	case ePlayerState::FALLING_LEFT:
		onFallingLeft(deltaTime);
		break;
	case ePlayerState::FALLING_RIGHT:
		onFallingRight(deltaTime);
		break;
	case ePlayerState::FALLING_UP:
		onFallingUp(deltaTime);
		break;
	case ePlayerState::JUMP_BOARD:
		onJumpBoard(deltaTime);
		break;
	case ePlayerState::CROUCH_LEFT:
		onCrouchLeft(deltaTime);
		break;
	case ePlayerState::CROUCH_RIGHT:
		onCrouchRight(deltaTime);
		break;
	case ePlayerState::LOOK_UP_LEFT:
		onLookUpLeft(deltaTime);
		break;
	case ePlayerState::LOOK_UP_RIGHT:
		onLookUpRight(deltaTime);
		break;
	case ePlayerState::SPIN_LEFT:
		onSpinLeft(deltaTime);
		break;
	case ePlayerState::SPIN_RIGHT:
		onSpinRight(deltaTime);
		break;
	case ePlayerState::SPIN_DASH_LEFT:
		onSpinDashLeft(deltaTime);
		break;
	case ePlayerState::SPIN_DASH_RIGHT:
		onSpinDashRight(deltaTime);
		break;
	case ePlayerState::WOOD_RUNNING:
		onWoodRunning(deltaTime);
		break;
	case ePlayerState::SPRING_JUMP_LEFT:
		onSpringJumpLeft(deltaTime);
		break;
	case ePlayerState::SPRING_JUMP_RIGHT:
		onSpringJumpRight(deltaTime);
		break;
	case ePlayerState::DAMAGED_LEFT:
		onDamagedLeft(deltaTime);
		break;
	case ePlayerState::DAMAGED_RIGHT:
		onDamagedRight(deltaTime);
		break;
	case ePlayerState::STAGE_CLEAR:
		onStageClear(deltaTime);
		break;
	default:
		break;
	}

	//자기장
	{
		if (skillType_ == eMonitor::ELECTRIC)
		{
			std::list<RSCollision*> collisionList = electricFieldCollision_->GetCollisionGroupCheck(static_cast<int>(eCollisionCheckGroup::RING));

			std::list<RSCollision*>::iterator collisionIter = collisionList.begin();
			std::list<RSCollision*>::iterator collisionEndIter = collisionList.end();

			for (; collisionIter != collisionEndIter; ++collisionIter)
			{
				Ring* currentRing = dynamic_cast<Ring*>((*collisionIter)->GetActor());

				if (currentRing->IsCollected())
				{
					continue;
				}

				std::list<Ring*>::iterator ringIter = electricEffectRingList_.begin();
				std::list<Ring*>::iterator ringEndIter = electricEffectRingList_.end();
				bool bExist = false;
				for (; ringIter != ringEndIter; ++ringIter)
				{
					if (*ringIter == currentRing)
					{
						bExist = true;
						break;
					}
				}

				if (!bExist)
				{
					electricEffectRingList_.push_back(currentRing);
				}
			}
		}
	}

	// 깜빡임 처리
	if (bBlink_)
	{
		if (blinkCooltime_ < 0.0f)
		{
			mainRender_->SwitchActivation();
			blinkCooltime_ = BLINK_COOLTIME;
		}

		if (blinkTime_ < 0.0f)
		{
			bBlink_ = false;
			mainRender_->Activate();
		}

		blinkTime_ -= deltaTime;
		blinkCooltime_ -= deltaTime;
	}

	// 무적 처리
	if (bInvincible_)
	{
		invincibleTime_ -= deltaTime;

		if (invincibleTime_ < 0.0f)
		{
			bInvincible_ = false;
		}
	}

#ifdef _DEBUG
	debugRenderOnOff();
#endif
}

void Player::UpdateAfter()
{
	float deltaTime = RSTimer::GetInstance().GetDeltaTime();

	skillPosition_.x = 0.0f;
	skillPosition_.y = -18.f;
	if (bLeftRollStartForSkillLocation_)
	{
		skillPosition_ = skillPosition_.Rotate2DByDegree(rotation_);
	}
	else
	{
		skillPosition_ = skillPosition_.Rotate2DByDegree(-rotation_);
	}
	skillPosition_ = skillPosition_ + location_;

	// 자기장
	{
		std::list<Ring*>::iterator iter = electricEffectRingList_.begin();
		std::list<Ring*>::iterator endIter = electricEffectRingList_.end();
		while (iter != endIter)
		{
			Ring* currentRing = *iter;
			if (currentRing->IsCollected())
			{
				iter = electricEffectRingList_.erase(iter);
			}
			else
			{
				float4 ringLocation = currentRing->GetLocation();
				float4 direction = ringLocation.Direction(location_);
				float distance = ringLocation.DistanceTo(location_);

				currentRing->SetMovement(direction * (NORMAL_ACCELATION + distance * 2.f) * deltaTime);
				++iter;
			}
		}
	}
}

void Player::Render()
{
	mainRender_->Render();

#ifdef _DEBUG
	if (bDebugRender_)
	{
		DebugRender();
	}

#endif
}

void Player::Cleanup()
{
	bgmPlayer_->Stop();
}

void Player::ChangeState(ePlayerState _state, float _delay)
{
	nextState_ = _state;
	delayForNextState_ = _delay;
	playerState_ = ePlayerState::DELAY;
}

void Player::ResetState()
{
	ChangeState(ePlayerState::IDLE_RIGHT);
	direction_ = float4::ZERO;
	upVector_ = float4::ZERO;
	forwardVector_ = float4::ZERO;
	gravityAccelation_ = float4::ZERO;
	playerSpeed_ = float4::ZERO;
	gravitySpeed_ = float4::ZERO;
	actualSpeed_ = float4::ZERO;
	accelation_ = 0.0f;

	bCanJump_ = true;
	bGround_ = false;
	bLeftAnglePossible_ = false;
	bRightAnglePossible_ = false;
	bAngled_ = false;
	bRollRight_ = false;
	bRollLeft_ = false;
	bRollUp_ = false;
	bRolling_ = false;
	rotation_ = 0.f;
	bAngled_ = false;
	mainRender_->SetRotate(rotation_);
	mapCollisionSideCheck();

}

ePlayerState Player::GetState()
{
	return playerState_;
}

void Player::DebugRender()
{
	HDC hdc = RSResourceManager::GetInstance().GetBackBuffer()->GetDC();
	char buffer[100] = { 0, };

	sprintf_s(buffer, 100, "location_ : %.2f, %.2f", location_.x, location_.y);
	TextOutA(hdc, 5, 5, buffer, static_cast<int>(strlen(buffer)));
	sprintf_s(buffer, 100, "상태 : %s , right %d, left %d up %d rolling %d", stateNameMap_.at(playerState_), bRollRight_, bRollLeft_, bRollUp_, bRolling_);
	TextOutA(hdc, 5, 25, buffer, static_cast<int>(strlen(buffer)));
	sprintf_s(buffer, 100, "playerSpeed_ : %.2f, %.2f, direction_ : %.2f, %.2f", playerSpeed_.x, playerSpeed_.y, direction_.x, direction_.y);
	TextOutA(hdc, 5, 45, buffer, static_cast<int>(strlen(buffer)));
	sprintf_s(buffer, 100, "gravitySpeed_ : %.2f, %.2f", gravitySpeed_.x, gravitySpeed_.y);
	TextOutA(hdc, 5, 65, buffer, static_cast<int>(strlen(buffer)));
	sprintf_s(buffer, 100, "accel : %.2f, bGround_ : %d, SkillType : %d", accelation_, bGround_, static_cast<int>(skillType_));
	TextOutA(hdc, 5, 85, buffer, static_cast<int>(strlen(buffer)));

	sprintf_s(buffer, 100, "rotation_ : %.2f, bSpin_ : %d, bCanJump_ : %d", rotation_, bSpin_, bCanJump_);
	TextOutA(hdc, 5, 105, buffer, static_cast<int>(strlen(buffer)));

	if (!bRolling_)
	{
		float4 ellipsePos = location_ + bottomCenterCollision_->GetPivot();
		ellipsePos -= level_->GetCamPos();
		ellipsePos += level_->GetCamSize().halffloat4();
		int r = 2;
		::Ellipse(hdc, ellipsePos.ix() - r, ellipsePos.iy() - r, ellipsePos.ix() + r, ellipsePos.iy() + r);
		ellipsePos = location_ + bottomLeftCollision_->GetPivot();
		ellipsePos -= level_->GetCamPos();
		ellipsePos += level_->GetCamSize().halffloat4();
		::Ellipse(hdc, ellipsePos.ix() - r, ellipsePos.iy() - r, ellipsePos.ix() + r, ellipsePos.iy() + r);
		ellipsePos = location_ + bottomRightCollision_->GetPivot();
		ellipsePos -= level_->GetCamPos();
		ellipsePos += level_->GetCamSize().halffloat4();
		::Ellipse(hdc, ellipsePos.ix() - r, ellipsePos.iy() - r, ellipsePos.ix() + r, ellipsePos.iy() + r);
		ellipsePos = location_ + topCenterCollision_->GetPivot();
		ellipsePos -= level_->GetCamPos();
		ellipsePos += level_->GetCamSize().halffloat4();
		::Ellipse(hdc, ellipsePos.ix() - r, ellipsePos.iy() - r, ellipsePos.ix() + r, ellipsePos.iy() + r);
		ellipsePos = location_ + centerLeftCollision_->GetPivot();
		ellipsePos -= level_->GetCamPos();
		ellipsePos += level_->GetCamSize().halffloat4();
		::Ellipse(hdc, ellipsePos.ix() - r, ellipsePos.iy() - r, ellipsePos.ix() + r, ellipsePos.iy() + r);
		ellipsePos = location_ + centerRightCollision_->GetPivot();
		ellipsePos -= level_->GetCamPos();
		ellipsePos += level_->GetCamSize().halffloat4();
		::Ellipse(hdc, ellipsePos.ix() - r, ellipsePos.iy() - r, ellipsePos.ix() + r, ellipsePos.iy() + r);

		ellipsePos = skillPosition_;
		ellipsePos -= level_->GetCamPos();
		ellipsePos += level_->GetCamSize().halffloat4();
		::Ellipse(hdc, ellipsePos.ix() - r, ellipsePos.iy() - r, ellipsePos.ix() + r, ellipsePos.iy() + r);
	}
	else
	{
		float4 ellipsePos = location_ + bottomCenterCollision_->GetPivot();
		ellipsePos -= level_->GetCamPos();
		ellipsePos += level_->GetCamSize().halffloat4();
		int r = 2;
		::Ellipse(hdc, ellipsePos.ix() - r, ellipsePos.iy() - r, ellipsePos.ix() + r, ellipsePos.iy() + r);
		ellipsePos = location_ + bottomLeftRollCollision_->GetPivot();
		ellipsePos -= level_->GetCamPos();
		ellipsePos += level_->GetCamSize().halffloat4();
		::Ellipse(hdc, ellipsePos.ix() - r, ellipsePos.iy() - r, ellipsePos.ix() + r, ellipsePos.iy() + r);
		ellipsePos = location_ + bottomRightRollCollision_->GetPivot();
		ellipsePos -= level_->GetCamPos();
		ellipsePos += level_->GetCamSize().halffloat4();
		::Ellipse(hdc, ellipsePos.ix() - r, ellipsePos.iy() - r, ellipsePos.ix() + r, ellipsePos.iy() + r);
		ellipsePos = skillPosition_;
		ellipsePos -= level_->GetCamPos();
		ellipsePos += level_->GetCamSize().halffloat4();
		::Ellipse(hdc, ellipsePos.ix() - r, ellipsePos.iy() - r, ellipsePos.ix() + r, ellipsePos.iy() + r);

	}

	int lineX = GetCamEffectLocation().ix();
	int lineY = GetCamEffectLocation().iy();

	HGDIOBJ old = SelectObject(hdc, GetStockObject(WHITE_PEN));

	MoveToEx(hdc, lineX, lineY, nullptr);
	LineTo(hdc, lineX + static_cast<int>(forwardVector_.x * 150), lineY + static_cast<int>(forwardVector_.y * 150));

	MoveToEx(hdc, lineX, lineY, nullptr);
	LineTo(hdc, lineX + static_cast<int>(upVector_.x * 50), lineY + static_cast<int>(upVector_.y * 50));

	SelectObject(hdc, old);
}

void Player::SetSkillType(eMonitor _skill)
{
	skillType_ = _skill;

	if (nullptr == skill_)
	{
		RSDebug::AssertFalse();
	}

	switch (skillType_)
	{
	case eMonitor::ELECTRIC:
		skill_->electricShield();
		break;
	case eMonitor::FIRE:
		skill_->fireShield();
		break;
	case eMonitor::BUBBLE:
		skill_->bubbleShield();
		break;
	default:
		break;
	}
}

void Player::SetSkill(PlayerSkill* _skill)
{
	skill_ = _skill;
}

float4 Player::GetSpeed()
{
	return playerSpeed_;
}

float4 Player::SetSpeed(const float4& _speed)
{
	return playerSpeed_ = _speed;
}

float4 Player::GetGravitySpeed()
{
	return gravitySpeed_;
}

float4 Player::SetGravitySpeed(const float4& _speed)
{
	return gravitySpeed_ = _speed;
}

bool Player::IsGround()
{
	return bGround_;
}

void Player::PushRingToDrop(Ring* _ring)
{
	ringsReadyToDrop_.push_back(_ring);
}

void Player::SpringJump(float _power, eDirection _direction)
{
	float4 direction;
	switch (_direction)
	{
	case eDirection::UP:
		direction = float4::UP;
		break;
	case eDirection::DOWN:
		direction = float4::DOWN;
		break;
	case eDirection::LEFT:
		bSpin_ = false;
		direction = float4::LEFT;
		break;
	case eDirection::RIGHT:
		bSpin_ = false;
		direction = float4::RIGHT;
		break;
	default:
		break;
	}

	RSSoundManager::GetInstance().PlaySoundByName("spring.wav");
	// 스프링점프로 진입하는 상태
	switch (playerState_)
	{
	case ePlayerState::IDLE_LEFT:
		[[fallthrough]];
	case ePlayerState::WALK_LEFT:
		[[fallthrough]];
	case ePlayerState::JUMP_LEFT:
		[[fallthrough]];
	case ePlayerState::SKID_LEFT:
		[[fallthrough]];
	case ePlayerState::SPIN_LEFT:
		[[fallthrough]];
	case ePlayerState::SPRING_JUMP_LEFT:
	{
		switch (_direction)
		{
		case eDirection::UP:
			mainRender_->ChangeAnimation("JumpBoardLeft", "sonicLeft.bmp", true);
			gravitySpeed_.y = -_power;
			bGround_ = false;
			SetMovement(float4::UP * 5.f);
			ChangeState(ePlayerState::SPRING_JUMP_LEFT);
			break;
		case eDirection::DOWN:
			break;
		case eDirection::LEFT:
			mainRender_->ChangeAnimation("WalkLeft", "sonicLeft.bmp", true);
			playerSpeed_.x = -_power;
			bCanJump_ = true;
			SetMovement(float4::LEFT * 5.f);
			ChangeState(ePlayerState::WALK_LEFT);
			break;
		case eDirection::RIGHT:
			mainRender_->ChangeAnimation("WalkRight", "sonicRight.bmp", true);
			playerSpeed_.x = _power;
			bCanJump_ = true;
			SetMovement(float4::RIGHT * 5.f);
			ChangeState(ePlayerState::WALK_RIGHT);
			break;
		default:
			break;
		}
	}
	break;
	case ePlayerState::IDLE_RIGHT:
		[[fallthrough]];
	case ePlayerState::WALK_RIGHT:
		[[fallthrough]];
	case ePlayerState::JUMP_RIGHT:
		[[fallthrough]];
	case ePlayerState::SKID_RIGHT:
		[[fallthrough]];
	case ePlayerState::JUMP_BOARD:
		[[fallthrough]];
	case ePlayerState::SPIN_RIGHT:
		[[fallthrough]];
	case ePlayerState::SPRING_JUMP_RIGHT:
	{
		switch (_direction)
		{
		case eDirection::UP:
			mainRender_->ChangeAnimation("JumpBoardRight", "sonicRight.bmp", true);
			gravitySpeed_.y = -_power;
			bGround_ = false;
			SetMovement(float4::UP * 5.f);
			ChangeState(ePlayerState::SPRING_JUMP_RIGHT);
			break;
		case eDirection::DOWN:
			break;
		case eDirection::LEFT:
			mainRender_->ChangeAnimation("WalkLeft", "sonicLeft.bmp", true);
			playerSpeed_.x = -_power;
			bCanJump_ = true;
			SetMovement(float4::LEFT * 5.f);
			ChangeState(ePlayerState::WALK_LEFT);
			break;
		case eDirection::RIGHT:
			mainRender_->ChangeAnimation("WalkRight", "sonicRight.bmp", true);
			playerSpeed_.x = _power;
			bCanJump_ = true;
			SetMovement(float4::RIGHT * 5.f);
			ChangeState(ePlayerState::WALK_RIGHT);
			break;
		default:
			break;
		}

	}
	break;
	default:
		break;
	}
}

void Player::SpikeTrapped()
{
	if (bInvincible_)
	{
		return;
	}

	RSSoundManager::GetInstance().PlaySoundByName("sting.wav");

	if (skillType_ == eMonitor::ELECTRIC
		|| skillType_ == eMonitor::BUBBLE
		|| skillType_ == eMonitor::FIRE)
	{
		skill_->ShieldOff();
		skillType_ = eMonitor::RING;
	}
	else
	{
		PlayLevelUI* ui = level_->FindActor<PlayLevelUI>("PlayLevelUI");
		if (0 < ui->GetRingCount())
		{
			RSSoundManager::GetInstance().PlaySoundByName("ringDrop.wav");
			DropAllRings();
			ui->SetRing(0);
		}
	}

	switch (playerState_)
	{
	case ePlayerState::IDLE_LEFT:
		[[fallthrough]];
	case ePlayerState::WALK_LEFT:
		[[fallthrough]];
	case ePlayerState::JUMP_LEFT:
		[[fallthrough]];
	case ePlayerState::SKID_LEFT:
		[[fallthrough]];
	case ePlayerState::PUSH_LEFT:
		[[fallthrough]];
	case ePlayerState::SPIN_LEFT:
		[[fallthrough]];
	case ePlayerState::SPRING_JUMP_LEFT:
	{
		SetMovement({ 5.0f, -5.0f });
		playerSpeed_.x = 100.f;
		gravitySpeed_.y = -200.f;
		bInvincible_ = true;
		invincibleTime_ = 999.f;
		ChangeState(ePlayerState::DAMAGED_LEFT);
	}
	break;
	case ePlayerState::IDLE_RIGHT:
		[[fallthrough]];
	case ePlayerState::WALK_RIGHT:
		[[fallthrough]];
	case ePlayerState::JUMP_RIGHT:
		[[fallthrough]];
	case ePlayerState::SKID_RIGHT:
		[[fallthrough]];
	case ePlayerState::PUSH_RIGHT:
		[[fallthrough]];
	case ePlayerState::JUMP_BOARD:
		[[fallthrough]];
	case ePlayerState::SPIN_RIGHT:
		[[fallthrough]];
	case ePlayerState::SPRING_JUMP_RIGHT:
	{
		SetMovement({ -5.0f, -5.0f });
		playerSpeed_.x = -100.f;
		gravitySpeed_.y = -200.f;
		bInvincible_ = true;
		invincibleTime_ = 999.f;
		ChangeState(ePlayerState::DAMAGED_RIGHT);
	}
	break;
	default:
		break;
	}
}

void Player::Attacked()
{
	if (bInvincible_)
	{
		return;
	}

	RSSoundManager::GetInstance().PlaySoundByName("attacked.wav");

	if (skillType_ == eMonitor::ELECTRIC
		|| skillType_ == eMonitor::BUBBLE
		|| skillType_ == eMonitor::FIRE)
	{
		skill_->ShieldOff();
		skillType_ = eMonitor::RING;
	}
	else
	{
		PlayLevelUI* ui = level_->FindActor<PlayLevelUI>("PlayLevelUI");
		if (0 < ui->GetRingCount())
		{
			RSSoundManager::GetInstance().PlaySoundByName("ringDrop.wav");
			DropAllRings();
			ui->SetRing(0);
		}
	}

	switch (playerState_)
	{
	case ePlayerState::IDLE_LEFT:
		[[fallthrough]];
	case ePlayerState::WALK_LEFT:
		[[fallthrough]];
	case ePlayerState::JUMP_LEFT:
		[[fallthrough]];
	case ePlayerState::SKID_LEFT:
		[[fallthrough]];
	case ePlayerState::PUSH_LEFT:
		[[fallthrough]];
	case ePlayerState::SPIN_LEFT:
		[[fallthrough]];
	case ePlayerState::SPIN_DASH_LEFT:
		[[fallthrough]];
	case ePlayerState::LOOK_UP_LEFT:
		[[fallthrough]];
	case ePlayerState::CROUCH_LEFT:
		[[fallthrough]];
	case ePlayerState::SPRING_JUMP_LEFT:
	{
		SetMovement({ 5.0f, -5.0f });
		playerSpeed_.x = 100.f;
		gravitySpeed_.y = -200.f;
		bInvincible_ = true;
		invincibleTime_ = 999.f;
		ChangeState(ePlayerState::DAMAGED_LEFT);
	}
	break;
	case ePlayerState::IDLE_RIGHT:
		[[fallthrough]];
	case ePlayerState::WALK_RIGHT:
		[[fallthrough]];
	case ePlayerState::JUMP_RIGHT:
		[[fallthrough]];
	case ePlayerState::SKID_RIGHT:
		[[fallthrough]];
	case ePlayerState::PUSH_RIGHT:
		[[fallthrough]];
	case ePlayerState::JUMP_BOARD:
		[[fallthrough]];
	case ePlayerState::SPIN_RIGHT:
		[[fallthrough]];
	case ePlayerState::SPIN_DASH_RIGHT:
		[[fallthrough]];
	case ePlayerState::LOOK_UP_RIGHT:
		[[fallthrough]];
	case ePlayerState::CROUCH_RIGHT:
		[[fallthrough]];
	case ePlayerState::SPRING_JUMP_RIGHT:
	{
		SetMovement({ -5.0f, -5.0f });
		playerSpeed_.x = -100.f;
		gravitySpeed_.y = -200.f;
		bInvincible_ = true;
		invincibleTime_ = 999.f;
		ChangeState(ePlayerState::DAMAGED_RIGHT);
	}
	break;
	default:
		break;
	}
}

void Player::DropAllRings()
{
	PlayLevelUI* ui = level_->FindActor<PlayLevelUI>("PlayLevelUI");
	size_t ringCount = static_cast<size_t>(ui->GetRingCount());
	while (ringCount > ringsReadyToDrop_.size())
	{
		Ring* newRing = level_->AddActor<Ring>();
		newRing->SetLocation({ 0.0f, 0.0f });
		newRing->Deactivate();
		ringsReadyToDrop_.push_back(newRing);
	}

	level_->CollisionGroupCheck();

	auto startIter = ringsReadyToDrop_.begin();
	auto endIter = ringsReadyToDrop_.end();

	size_t currentCount = 0;
	while (startIter != endIter && ringCount > currentCount && 30 > currentCount)
	{
		(*startIter)->Drop(location_);
		++currentCount;
		++startIter;
	}

	ringsReadyToDrop_.clear();
}

void Player::StopBGM()
{
	bgmPlayer_->Stop();
}

void Player::StartBGM()
{
	if (false == bgmPlayer_->IsPlaying())
	{
		bgmPlayer_->Play();
	}
}

void Player::ChangeBGM(const std::string& _name)
{
	StopBGM();
	bgmPlayer_->ChangeSound(_name);
}

void Player::ChangeBGMBossArea()
{
	StopBGM();
	bgmPlayer_->ChangeSound("boss.mid");
}

void Player::EnterBossArea()
{
	bBossAreaBGM_ = true;
	bBossArea_ = true;
}

void Player::ExitBossArea()
{
	ChangeBGM("stage1-1.mid");

	bBossArea_ = false;
}

void Player::ExitBossAreaBGM()
{
	bBossAreaBGM_ = false;
}

void Player::StageClear()
{
	mainRender_->ChangeAnimation("ShakeFingerEnter", "sonicRight.bmp", true);
	ChangeState(ePlayerState::STAGE_CLEAR);
}

void Player::initInput()
{
	if (false == RSInput::GetInstance().IsKeyAvailable("Up"))
	{
		RSInput::GetInstance().CreateKey("Up", VK_UP);
	}
	if (false == RSInput::GetInstance().IsKeyAvailable("Down"))
	{
		RSInput::GetInstance().CreateKey("Down", VK_DOWN);
	}
	if (false == RSInput::GetInstance().IsKeyAvailable("Left"))
	{
		RSInput::GetInstance().CreateKey("Left", VK_LEFT);
	}
	if (false == RSInput::GetInstance().IsKeyAvailable("Right"))
	{
		RSInput::GetInstance().CreateKey("Right", VK_RIGHT);
	}
	if (false == RSInput::GetInstance().IsKeyAvailable("Jump"))
	{
		RSInput::GetInstance().CreateKey("Jump", 'A');
	}
	if (false == RSInput::GetInstance().IsKeyAvailable("TestKey1"))
	{
		RSInput::GetInstance().CreateKey("TestKey1", '1');
	}
	if (false == RSInput::GetInstance().IsKeyAvailable("TestKey2"))
	{
		RSInput::GetInstance().CreateKey("TestKey2", '2');
	}
	if (false == RSInput::GetInstance().IsKeyAvailable("TestKey3"))
	{
		RSInput::GetInstance().CreateKey("TestKey3", '3');
	}
	if (false == RSInput::GetInstance().IsKeyAvailable("TestKey4"))
	{
		RSInput::GetInstance().CreateKey("TestKey4", '4');
	}
	if (false == RSInput::GetInstance().IsKeyAvailable("TestKey5"))
	{
		RSInput::GetInstance().CreateKey("TestKey5", '5');
	}
	if (false == RSInput::GetInstance().IsKeyAvailable("TestKey6"))
	{
		RSInput::GetInstance().CreateKey("TestKey6", '6');
	}
	if (false == RSInput::GetInstance().IsKeyAvailable("TestKey7"))
	{
		RSInput::GetInstance().CreateKey("TestKey7", '7');
	}
}

void Player::initRendererAndAnimation()
{
	mainRender_ = new RSRenderer(this);
	mainRender_->CreateAnimation("IdleLeft", "sonicLeft.bmp", 0, 0, 0.1f, true);
	mainRender_->CreateAnimation("WalkLeft", "sonicLeft.bmp", 25, 32, 0.2f, true);
	mainRender_->CreateAnimation("WalkLeftAngled", "sonicLeft.bmp", 33, 40, 0.2f, true);
	mainRender_->CreateAnimation("FullSpeedLeft", "sonicLeft.bmp", 41, 44, 0.05f, true);
	mainRender_->CreateAnimation("FullSpeedLeftAngled", "sonicLeft.bmp", 45, 48, 0.05f, true);
	mainRender_->CreateAnimation("SpinLeft", "sonicLeft.bmp", 50, 56, 0.05f, true);
	mainRender_->CreateAnimation("JumpLeft", "sonicLeft.bmp", 50, 56, 0.05f, true);
	mainRender_->CreateAnimation("SkidLeft", "sonicLeft.bmp", 63, 66, 0.2f, true);
	mainRender_->CreateAnimation("SpringJumpLeft", "sonicLeft.bmp", 67, 67, 0.1f, false);
	mainRender_->CreateAnimation("JumpBoardLeft", "sonicLeft.bmp", 160, 171, 0.12f, false);
	mainRender_->CreateAnimation("CrouchLeft", "sonicLeft.bmp", 23, 24, 0.1f, false);
	mainRender_->CreateAnimation("LookUpLeft", "sonicLeft.bmp", 21, 22, 0.15f, false);
	mainRender_->CreateAnimation("SpinDashLeft", "sonicLeft.bmp", 69, 78, 0.05f, true);
	mainRender_->CreateAnimation("PushLeft", "sonicLeft.bmp", 57, 60, 0.5f, true);
	mainRender_->CreateAnimation("DamagedLeft", "sonicLeft.bmp", 61, 62, 0.3f, true);

	mainRender_->CreateAnimation("IdleRight", "sonicRight.bmp", 0, 0, 0.1f, true);
	mainRender_->CreateAnimation("WalkRight", "sonicRight.bmp", 25, 32, 0.2f, true);
	mainRender_->CreateAnimation("WalkRightAngled", "sonicRight.bmp", 33, 40, 0.2f, true);
	mainRender_->CreateAnimation("FullSpeedRight", "sonicRight.bmp", 41, 44, 0.05f, true);
	mainRender_->CreateAnimation("FullSpeedRightAngled", "sonicRight.bmp", 45, 48, 0.05f, true);
	mainRender_->CreateAnimation("SpinRight", "sonicRight.bmp", 50, 56, 0.05f, true);
	mainRender_->CreateAnimation("JumpRight", "sonicRight.bmp", 50, 56, 0.05f, true);
	mainRender_->CreateAnimation("SkidRight", "sonicRight.bmp", 63, 66, 0.2f, true);
	mainRender_->CreateAnimation("SpringJumpRight", "sonicRight.bmp", 67, 67, 0.1f, false);
	mainRender_->CreateAnimation("JumpBoardRight", "sonicRight.bmp", 160, 171, 0.12f, false);
	mainRender_->CreateAnimation("CrouchRight", "sonicRight.bmp", 23, 24, 0.1f, false);
	mainRender_->CreateAnimation("LookUpRight", "sonicRight.bmp", 21, 22, 0.15f, false);
	mainRender_->CreateAnimation("SpinDashRight", "sonicRight.bmp", 69, 78, 0.05f, true);
	mainRender_->CreateAnimation("PushRight", "sonicRight.bmp", 57, 60, 0.5f, true);
	mainRender_->CreateAnimation("DamagedRight", "sonicRight.bmp", 61, 62, 0.3f, true);

	mainRender_->CreateAnimation("WoodRunning", "sonicRight.bmp", 139, 150, 0.125f, true);
	
	mainRender_->CreateAnimation("ShakeFingerEnter", "sonicRight.bmp", 79, 80, 0.3f, false);
	mainRender_->CreateAnimation("ShakeFinger", "sonicRight.bmp", 80, 83, 0.2f, true);


	mainRender_->ChangeAnimation("IdleRight", "sonicRight.bmp", true);
}

void Player::initCollision()
{
	bottomCenterCollision_ = new RSCollision(this, eCollisionType::POINT, static_cast<int>(eCollisionCheckGroup::PLAYER));
	bottomCenterCollision_->SetCheckColor(static_cast<DWORD>(eCollisionCheckColor::MAP));
	level_->PushCollision(bottomCenterCollision_);

	bottomLeftCollision_ = new RSCollision(this, eCollisionType::POINT, static_cast<int>(eCollisionCheckGroup::PLAYER));
	bottomLeftCollision_->SetCheckColor(static_cast<DWORD>(eCollisionCheckColor::MAP));

	bottomRightCollision_ = new RSCollision(this, eCollisionType::POINT, static_cast<int>(eCollisionCheckGroup::PLAYER));
	bottomRightCollision_->SetCheckColor(static_cast<DWORD>(eCollisionCheckColor::MAP));

	topCenterCollision_ = new RSCollision(this, eCollisionType::POINT, static_cast<int>(eCollisionCheckGroup::PLAYER));
	topCenterCollision_->SetCheckColor(static_cast<DWORD>(eCollisionCheckColor::MAP));
	topCenterCollision_->SetPivot({ 0.0f, -32.f });
	level_->PushCollision(topCenterCollision_);

	centerLeftCollision_ = new RSCollision(this, eCollisionType::POINT, static_cast<int>(eCollisionCheckGroup::PLAYER));
	centerLeftCollision_->SetCheckColor(static_cast<DWORD>(eCollisionCheckColor::MAP));
	centerLeftCollision_->SetPivot({ -14.0f, -20.f });
	level_->PushCollision(centerLeftCollision_);

	centerRightCollision_ = new RSCollision(this, eCollisionType::POINT, static_cast<int>(eCollisionCheckGroup::PLAYER));
	centerRightCollision_->SetCheckColor(static_cast<DWORD>(eCollisionCheckColor::MAP));
	centerRightCollision_->SetPivot({ 14.0f, -20.f });
	level_->PushCollision(centerRightCollision_);

	bottomCenterRollLeftCollision_ = new RSCollision(this, eCollisionType::POINT, static_cast<int>(eCollisionCheckGroup::PLAYER));
	bottomCenterRollLeftCollision_->SetCheckColor(static_cast<DWORD>(eCollisionCheckColor::ROLL_LEFT));

	bottomCenterRollRightCollision_ = new RSCollision(this, eCollisionType::POINT, static_cast<int>(eCollisionCheckGroup::PLAYER));
	bottomCenterRollRightCollision_->SetCheckColor(static_cast<DWORD>(eCollisionCheckColor::ROLL_RIGHT));

	bottomCenterRollUpCollision_ = new RSCollision(this, eCollisionType::POINT, static_cast<int>(eCollisionCheckGroup::PLAYER));
	bottomCenterRollUpCollision_->SetCheckColor(static_cast<DWORD>(eCollisionCheckColor::ROLL_UP));

	bottomCenterRollPostCollision_ = new RSCollision(this, eCollisionType::POINT, static_cast<int>(eCollisionCheckGroup::PLAYER));
	bottomCenterRollPostCollision_->SetCheckColor(static_cast<DWORD>(eCollisionCheckColor::ROLL_HALF_POST));

	rollCheckSideCollision_ = new RSCollision(this, eCollisionType::POINT, static_cast<int>(eCollisionCheckGroup::PLAYER));
	rollCheckSideCollision_->SetCheckColor(static_cast<DWORD>(eCollisionCheckColor::ROLL_SIDE_CHECK));

	bottomLeftRollCollision_ = new RSCollision(this, eCollisionType::POINT, static_cast<int>(eCollisionCheckGroup::PLAYER));
	bottomLeftRollCollision_->SetCheckColor(static_cast<DWORD>(eCollisionCheckColor::ROLL_LEFT));

	bottomRightRollCollision_ = new RSCollision(this, eCollisionType::POINT, static_cast<int>(eCollisionCheckGroup::PLAYER));
	bottomRightRollCollision_->SetCheckColor(static_cast<DWORD>(eCollisionCheckColor::ROLL_RIGHT));

	groundCheckCollision_ = new RSCollision(this, eCollisionType::POINT, static_cast<int>(eCollisionCheckGroup::PLAYER));
	groundCheckCollision_->SetCheckColor(static_cast<DWORD>(eCollisionCheckColor::MAP));
	groundCheckCollision_->SetPivot({ 0.0f, 1.0f });

	groundCheckRollCollision_ = new RSCollision(this, eCollisionType::POINT, static_cast<int>(eCollisionCheckGroup::PLAYER));
	groundCheckRollCollision_->SetCheckColor(static_cast<DWORD>(eCollisionCheckColor::ROLL_RIGHT));
	groundCheckRollCollision_->SetPivot({ 0.0f, 1.0f });

	jumpBoardCheckCollision_ = new RSCollision(this, eCollisionType::POINT, static_cast<int>(eCollisionCheckGroup::JUMP_BOARD));
	jumpBoardCheckCollision_->SetCheckColor(static_cast<DWORD>(eCollisionCheckColor::JUMP_BOARD));

	woodRunCheckCollision_ = new RSCollision(this, eCollisionType::POINT, static_cast<int>(eCollisionCheckGroup::HORIZONTAL_RUN));
	woodRunCheckCollision_->SetCheckColor(static_cast<DWORD>(eCollisionCheckColor::HORIZONTAL_RUN));

	woodRunFinishCheckCollision_ = new RSCollision(this, eCollisionType::POINT, static_cast<int>(eCollisionCheckGroup::HORIZONTAL_RUN));
	woodRunFinishCheckCollision_->SetCheckColor(static_cast<DWORD>(eCollisionCheckColor::HORIZONTAL_RUN_FINISH));

	electricFieldCollision_ = new RSCollision(this, eCollisionType::CIRCLE, static_cast<int>(eCollisionCheckGroup::PLAYER));
	electricFieldCollision_->SetRadius(100.f);
}

void Player::initSoundPlayer()
{
	bgmPlayer_ = new RSSoundPlayer("stage1-1.mid");
	jumpSound_ = new RSSoundPlayer("jump.wav");
	rollingSound_ = new RSSoundPlayer("rolling.wav");
	spinSound_ = new RSSoundPlayer("spin.wav");
	spinDashSound_ = new RSSoundPlayer("spinDash.wav");
}

void Player::debugRenderOnOff()
{
	if (true == RSInput::GetInstance().IsKeyDown("P"))
	{
		bDebugRender_ = !bDebugRender_;
	}
}

void Player::processGravity(float _deltaTime)
{
	RSCollision* collisionMonitor = groundCheckCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MONITOR));

	if (nullptr == groundCheckCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP))
		&& (nullptr == collisionMonitor || false == dynamic_cast<Monitor*>(collisionMonitor->GetActor())->IsActivate()))
	{
		bGround_ = false;
		if (gravityAccelation_.y < GRAVITY_POWER)
		{
			gravityAccelation_.y += GRAVITY_POWER;
		}
	}
	else
	{
		bGround_ = true;
		//gravitySpeed_.x = 0.0f;
		playerSpeed_.y = 0.0f;
		gravitySpeed_.y = 0.f;
		gravityAccelation_.y = 0.0f;
		location_.y = static_cast<float>(static_cast<int>(location_.y));
	}
}

void Player::mapCollisionSideCheck()
{
	// 일반 이동 체크
	{
		float4 leftCollision = float4::LEFT;
		float4 leftUpVector = leftCollision.Rotate2DByDegree(90);
		leftCollision = leftCollision * 3;
		for (int i = -7; i <= 7; i++)
		{
			bottomLeftCollision_->SetPivot(leftCollision + (leftUpVector * static_cast<float>(i)));
			if (nullptr == bottomLeftCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
			{
				if (abs(i) > 5 || abs(i) < 2)
				{
					bLeftAnglePossible_ = false;
				}
				else
				{
					bLeftAnglePossible_ = true;
				}
				break;
			}
		}

		float4 rightCollision = float4::RIGHT;
		float4 RightUpVector = rightCollision.Rotate2DByDegree(-90);
		rightCollision = rightCollision * 3;
		for (int i = -7; i <= 7; i++)
		{
			bottomRightCollision_->SetPivot(rightCollision + (RightUpVector * static_cast<float>(i)));
			if (nullptr == bottomRightCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
			{
				if (abs(i) > 5 || abs(i) < 2)
				{
					bRightAnglePossible_ = false;
				}
				else
				{
					bRightAnglePossible_ = true;
				}
				break;
			}
		}
	}
}

void Player::rollSideCheck()
{
	if (nullptr != rollCheckSideCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::ROLL_LEFT)))
	{
		bRollLeft_ = true;
		bRollRight_ = false;
		bRollUp_ = false;
		groundCheckRollCollision_->SetCheckColor(static_cast<int>(eCollisionCheckColor::ROLL_LEFT));
	}
	else if (nullptr != rollCheckSideCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::ROLL_RIGHT)))
	{
		bRollLeft_ = false;
		bRollRight_ = true;
		bRollUp_ = false;
		groundCheckRollCollision_->SetCheckColor(static_cast<int>(eCollisionCheckColor::ROLL_RIGHT));
	}
	else if (nullptr != rollCheckSideCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::ROLL_UP)))
	{
		bRollLeft_ = false;
		bRollRight_ = false;
		bRollUp_ = true;
		groundCheckRollCollision_->SetCheckColor(static_cast<int>(eCollisionCheckColor::ROLL_UP));
	}
}

void Player::setForwardRight()
{
	if (bRolling_)
	{
		forwardVector_ = bottomCenterCollision_->GetPivot().Direction(bottomRightRollCollision_->GetPivot());
		upVector_ = forwardVector_.Rotate2DByDegree(-90);
	}
	else
	{
		forwardVector_ = bottomCenterCollision_->GetPivot().Direction(bottomRightCollision_->GetPivot());
		upVector_ = forwardVector_.Rotate2DByDegree(-90);
	}
}

void Player::setForwardLeft()
{
	if (bRolling_)
	{
		forwardVector_ = bottomCenterCollision_->GetPivot().Direction(bottomLeftRollCollision_->GetPivot());
		upVector_ = forwardVector_.Rotate2DByDegree(90);
	}
	else
	{
		forwardVector_ = bottomCenterCollision_->GetPivot().Direction(bottomLeftCollision_->GetPivot());
		upVector_ = forwardVector_.Rotate2DByDegree(90);
	}
}

void Player::normalMove(float _deltaTime)
{
	float4 nextPlayerSpeed = playerSpeed_ + (direction_ * accelation_ * _deltaTime);

	if (nextPlayerSpeed.Length() < NORMAL_SPEED_LIMIT)
	{
		playerSpeed_ = nextPlayerSpeed;
	}
	else
	{
		while (playerSpeed_.Length() > NORMAL_SPEED_LIMIT)
		{
			playerSpeed_ = playerSpeed_ * 0.8f;
		}
	}

	gravitySpeed_ += gravityAccelation_ * _deltaTime;

	actualSpeed_ = playerSpeed_ + gravitySpeed_;

	SetMovement(actualSpeed_ * _deltaTime);


	// 땅, 모니터에 박힌것.
	if (nullptr != bottomCenterCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
	{
		while (nullptr != bottomCenterCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
		{
			SetMovement({ 0.0f, -1.0f });
			location_.y = static_cast<float>(static_cast<int>(location_.y));
		}
	}
	else if (bAngled_ == false)
	{
		bottomCenterCollision_->SetPivot({ 0, 1 });
		if (nullptr != bottomCenterCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
		{
			// 정상 땅에 딱 붙어있는 상태
			bottomCenterCollision_->SetPivot({ 0, 0 });
			return;
		}

		bottomCenterCollision_->SetPivot({ 0, 3 });
		if (nullptr == bottomCenterCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
		{
			bGround_ = false;
			bottomCenterCollision_->SetPivot({ 0, 0 });
			return;
		}


		bottomCenterCollision_->SetPivot({ 0, 0 });
		// 여기 왔으면 경사라고 본다.
		while (nullptr == bottomCenterCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
		{
			SetMovement({ 0.0f, 1.0f });
			location_.y = static_cast<float>(static_cast<int>(location_.y));
		}

		SetMovement({ 0.0f, -1.0f });
	}
}

void Player::noLimitMove(float _deltaTime)
{
	float4 nextPlayerSpeed = playerSpeed_ + (direction_ * accelation_ * _deltaTime);

	playerSpeed_ = nextPlayerSpeed;

	gravitySpeed_ += gravityAccelation_ * _deltaTime;

	actualSpeed_ = playerSpeed_ + gravitySpeed_;

	SetMovement(actualSpeed_ * _deltaTime);

	// 땅에 박힌것.
	if (nullptr != bottomCenterCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
	{
		while (nullptr != bottomCenterCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
		{
			SetMovement({ 0.0f, -1.0f });
			location_.y = static_cast<float>(static_cast<int>(location_.y));
		}
	}
	else if (bAngled_ == false)
	{
		bottomCenterCollision_->SetPivot({ 0, 1 });
		if (nullptr != bottomCenterCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
		{
			// 정상 땅에 딱 붙어있는 상태
			bottomCenterCollision_->SetPivot({ 0, 0 });
			return;
		}

		bottomCenterCollision_->SetPivot({ 0, 3 });
		if (nullptr == bottomCenterCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
		{
			bGround_ = false;
			bottomCenterCollision_->SetPivot({ 0, 0 });
			return;
		}


		bottomCenterCollision_->SetPivot({ 0, 0 });
		// 여기 왔으면 경사라고 본다.
		while (nullptr == bottomCenterCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
		{
			SetMovement({ 0.0f, 1.0f });
			location_.y = static_cast<float>(static_cast<int>(location_.y));
		}

		SetMovement({ 0.0f, -1.0f });
	}
}

void Player::onWait(float _deltaTime)
{
	//PlayLevelUI* ui = level_->FindActor<PlayLevelUI>("PlayLevelUI");
	//ui->StageStart();
}

void Player::onDelay(float _deltaTime)
{
	if (delayForNextState_ < 0.f)
	{
		currentStateTime_ = 0.0f;
		playerState_ = nextState_;
	}
}

void Player::onIdleLeft(float _deltaTime)
{
	accelation_ = 0.0f;
	playerSpeed_ = float4::ZERO;
	bSpin_ = false;

	mainRender_->ChangeAnimation("IdleLeft", "sonicLeft.bmp");
	processGravity(_deltaTime);
	mapCollisionSideCheck();
	setForwardLeft();

	// 위 보기
	if (bGround_ && RSInput::GetInstance().IsKeyPress("Up"))
	{
		ChangeState(ePlayerState::LOOK_UP_LEFT);
		return;
	}

	// 웅크리기
	if (bGround_ && RSInput::GetInstance().IsKeyPress("Down"))
	{
		ChangeState(ePlayerState::CROUCH_LEFT);
		return;
	}

	// 점프
	if (bCanJump_ && bGround_)
	{
		if (true == RSInput::GetInstance().IsKeyDown("Jump"))
		{
			gravitySpeed_ = forwardVector_ * playerSpeed_.x;
			playerSpeed_ = { 0.0f, 0.0f };
			jumpTime_ = JUMP_TIME;
			jumpSound_->Play();
			ChangeState(ePlayerState::JUMP_LEFT);
			return;
		}
	}

	if (true == RSInput::GetInstance().IsKeyPress("Left"))
	{
		if (nullptr != centerLeftCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
		{
			return;
		}
		playerSpeed_.x = -1.0f;
		ChangeState(ePlayerState::WALK_LEFT);
	}
	else if (true == RSInput::GetInstance().IsKeyPress("Right"))
	{
		if (nullptr != centerRightCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
		{
			return;
		}
		playerSpeed_.x = 1.0f;
		ChangeState(ePlayerState::WALK_RIGHT);
	}

	normalMove(_deltaTime);
}

void Player::onIdleRight(float _deltaTime)
{
	accelation_ = 0.0f;
	playerSpeed_ = float4::ZERO;
	bSpin_ = false;

	float currentRenderRotate = mainRender_->GetRotate();
	currentRenderRotate = float4::Lerp({ currentRenderRotate, 0.0f }, { 0.0f, 0.0f }, lerpRatio_ * 0.05f).x;

	mainRender_->SetRotate(currentRenderRotate);
	rotation_ = currentRenderRotate;

	if (lerpRatio_ > 1.f)
	{
		bAngled_ = false;
	}
	lerpRatio_ += _deltaTime;

	mainRender_->ChangeAnimation("IdleRight", "sonicRight.bmp");
	processGravity(_deltaTime);
	mapCollisionSideCheck();
	setForwardRight();

	// 위 보기
	if (bGround_ && RSInput::GetInstance().IsKeyPress("Up"))
	{
		ChangeState(ePlayerState::LOOK_UP_RIGHT);
		return;
	}

	// 웅크리기
	if (bGround_ && RSInput::GetInstance().IsKeyPress("Down"))
	{
		ChangeState(ePlayerState::CROUCH_RIGHT);
	}

	// 점프
	if (bCanJump_ && bGround_)
	{
		if (true == RSInput::GetInstance().IsKeyDown("Jump"))
		{
			gravitySpeed_ = forwardVector_ * playerSpeed_.x;
			playerSpeed_ = { 0.0f, 0.0f };
			jumpTime_ = JUMP_TIME;
			jumpSound_->Play();
			ChangeState(ePlayerState::JUMP_RIGHT);
			return;
		}
	}

	if (true == RSInput::GetInstance().IsKeyPress("Left"))
	{
		if (nullptr != centerLeftCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
		{
			return;
		}
		playerSpeed_.x = -1.0f;
		ChangeState(ePlayerState::WALK_LEFT);
	}
	else if (true == RSInput::GetInstance().IsKeyPress("Right"))
	{
		if (nullptr != centerRightCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
		{
			return;
		}
		playerSpeed_.x = 1.0f;
		ChangeState(ePlayerState::WALK_RIGHT);
	}

	normalMove(_deltaTime);
}

void Player::onBoringLeft(float _deltaTime)
{
}

void Player::onBoringRight(float _deltaTime)
{
}

void Player::onWalkLeft(float _deltaTime)
{
	accelation_ = 0.0f;

	mainRender_->SetMask("sonicLeft_mask.bmp");
	mapCollisionSideCheck();
	rollSideCheck();
	processGravity(_deltaTime);
	setForwardLeft();

	float currentRenderRotate = mainRender_->GetRotate();
	currentRenderRotate = float4::Lerp({ currentRenderRotate, 0.0f }, { 0.0f, 0.0f }, lerpRatio_).x;
	mainRender_->SetRotate(currentRenderRotate);
	rotation_ = currentRenderRotate;

	if (lerpRatio_ > 1.f)
	{
		bAngled_ = false;
	}
	lerpRatio_ += _deltaTime * 3.f;

	// 좌측 충돌 체크
	if (nullptr != centerLeftCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
	{
		playerSpeed_.x = 0;
		mainRender_->ChangeAnimation("IdleLeft", "sonicLeft.bmp");
		ChangeState(ePlayerState::IDLE_LEFT);
		return;
	}

	// 좌측 모니터 체크
	RSCollision* collisionMonitor = centerLeftCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MONITOR));
	if (nullptr != collisionMonitor)
	{
		Monitor* monitor = dynamic_cast<Monitor*>(collisionMonitor->GetActor());
		if (nullptr != monitor && monitor->IsActivate())
		{
			playerSpeed_.x = 0;
			mainRender_->ChangeAnimation("PushLeft", "sonicLeft.bmp");
			ChangeState(ePlayerState::PUSH_LEFT);
			return;
		}
	}

	// 점프
	if (bCanJump_ && bGround_)
	{
		jumpTime_ = JUMP_TIME;
		if (true == RSInput::GetInstance().IsKeyDown("Jump"))
		{
			gravitySpeed_ = forwardVector_ * playerSpeed_.x;
			jumpTime_ = JUMP_TIME;
			jumpSound_->Play();
			ChangeState(ePlayerState::JUMP_LEFT);
			return;
		}
	}

	// 구르기
	if (bGround_ && abs(playerSpeed_.x) > 150.f && RSInput::GetInstance().IsKeyPress("Down") && RSInput::GetInstance().IsKeyFree("Left"))
	{
		bSpin_ = true;
		rollingSound_->Play();
		ChangeState(ePlayerState::SPIN_LEFT);
		return;
	}

	direction_ = float4::LEFT;

	// 키 입력에 따라 움직이는 부분
	{
		if (RSInput::GetInstance().IsKeyPress("Left"))
		{
			accelation_ += NORMAL_ACCELATION;
		}
		else if (RSInput::GetInstance().IsKeyPress("Right"))
		{
			if (playerSpeed_.x > -100.f)
			{
				playerSpeed_.x = 0.0f;
			}

			if (playerSpeed_.x < -200.f)
			{
				RSSoundManager::GetInstance().PlaySoundByName("skid.wav");
				mainRender_->ChangeAnimation("SkidLeft", "sonicLeft.bmp");
				ChangeState(ePlayerState::SKID_LEFT);
				return;
			}

			accelation_ = playerSpeed_.x * 0.80f - 300.f;

			if (playerSpeed_.x >= 0)
			{
				ChangeState(ePlayerState::IDLE_RIGHT);
			}
		}
		else
		{
			if (playerSpeed_.x < -1.f)
			{
				accelation_ = playerSpeed_.x * 0.80f - 60.f;
			}
			else
			{
				playerSpeed_.x = 0.f;
				ChangeState(ePlayerState::IDLE_LEFT);
			}
		}
	}


	// 속도에 따라 애니메이션 바뀌는 부분
	if (!bSpin_)
	{
		float factor = (abs(playerSpeed_.x) / 250.f);
		if (factor > 1.0f)
		{
			factor = 1.0f;
		}

		mainRender_->SetFrameTime("WalkLeft", 0.3f - 0.25f * factor);
		if (playerSpeed_.Length() > FULL_SPEED_CHANGE)
		{
			mainRender_->ChangeAnimation("FullSpeedLeft", "sonicLeft.bmp");
			//ChangeState(ePlayerState::FULLSPEED_LEFT);
		}
		else
		{
			mainRender_->ChangeAnimation("WalkLeft", "sonicLeft.bmp");
		}
	}

	if (bGround_ && bRightAnglePossible_)
	{
		//ChangeState(ePlayerState::WALK_LEFT_ANGLED);
	}

	// 도는 행위에 진입
	if (bRollLeft_)
	{
		if (nullptr != groundCheckRollCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::ROLL_LEFT)))
		{
			rotation_ = 0.0f;
			ChangeState(ePlayerState::ROLLING_LEFT);
			bRolling_ = true;
			return;
		}
		else
		{
			bRolling_ = false;
		}
	}

	if (bSpin_)
	{
		noLimitMove(_deltaTime);
	}
	else
	{
		normalMove(_deltaTime);
	}
}

void Player::onWalkRight(float _deltaTime)
{
	accelation_ = 0.0f;

	mainRender_->SetMask("sonicRight_mask.bmp");
	mapCollisionSideCheck();
	rollSideCheck();
	processGravity(_deltaTime);
	setForwardRight();

	float currentRenderRotate = mainRender_->GetRotate();
	currentRenderRotate = float4::Lerp({ currentRenderRotate, 0.0f }, { 0.0f, 0.0f }, lerpRatio_ * 0.05f).x;

	mainRender_->SetRotate(currentRenderRotate);
	rotation_ = currentRenderRotate;

	if (lerpRatio_ > 1.f)
	{
		bAngled_ = false;
	}
	lerpRatio_ += _deltaTime;

	// 우측 충돌 체크
	if (nullptr != centerRightCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
	{
		playerSpeed_.x = 0;
		mainRender_->ChangeAnimation("IdleRight", "sonicRight.bmp");
		ChangeState(ePlayerState::IDLE_RIGHT);
		return;
	}

	// 우측 모니터 체크
	RSCollision* collisionMonitor = centerRightCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MONITOR));
	if (nullptr != collisionMonitor)
	{
		Monitor* monitor = dynamic_cast<Monitor*>(collisionMonitor->GetActor());
		if (nullptr != monitor && monitor->IsActivate())
		{
			playerSpeed_.x = 0;
			mainRender_->ChangeAnimation("PushRight", "sonicRight.bmp");
			ChangeState(ePlayerState::PUSH_RIGHT);
			return;
		}
	}

	// 점프
	if (bCanJump_ && bGround_)
	{
		jumpTime_ = JUMP_TIME;
		if (true == RSInput::GetInstance().IsKeyDown("Jump"))
		{
			gravitySpeed_ = forwardVector_ * playerSpeed_.x;
			jumpTime_ = JUMP_TIME;
			jumpSound_->Play();
			ChangeState(ePlayerState::JUMP_RIGHT);
			return;
		}
	}

	// 구르기
	if (bGround_ && abs(playerSpeed_.x) > 150.f && RSInput::GetInstance().IsKeyPress("Down") && RSInput::GetInstance().IsKeyFree("Right"))
	{
		bSpin_ = true;
		rollingSound_->Play();
		ChangeState(ePlayerState::SPIN_RIGHT);
		return;
	}

	direction_ = float4::RIGHT;

	// 키 입력에 따라 움직이는 부분
	{
		if (RSInput::GetInstance().IsKeyPress("Left"))
		{
			if (playerSpeed_.x < 100.f)
			{
				playerSpeed_.x = 0.0f;
			}

			if (playerSpeed_.x > 200.f)
			{
				RSSoundManager::GetInstance().PlaySoundByName("skid.wav");
				mainRender_->ChangeAnimation("SkidRight", "sonicRight.bmp");
				ChangeState(ePlayerState::SKID_RIGHT);
				return;
			}

			accelation_ = playerSpeed_.x * -0.80f - 300.f;

			if (playerSpeed_.x <= 0)
			{
				ChangeState(ePlayerState::IDLE_LEFT);
			}
		}
		else if (RSInput::GetInstance().IsKeyPress("Right"))
		{
			accelation_ += NORMAL_ACCELATION;
		}
		else
		{
			if (playerSpeed_.x > 1.f)
			{
				accelation_ = playerSpeed_.x * -0.80f - 60.f;
			}
			else
			{
				playerSpeed_.x = 0.f;
				mainRender_->ChangeAnimation("IdleRight", "sonicRight.bmp");
				ChangeState(ePlayerState::IDLE_RIGHT);
			}
		}
	}


	// 속도에 따라 애니메이션 바뀌는 부분
	if (bAngled_ == false && !bSpin_)
	{
		float factor = (abs(playerSpeed_.x) / 250.f);
		if (factor > 1.0f)
		{
			factor = 1.0f;
		}

		mainRender_->SetFrameTime("WalkRight", 0.3f - 0.25f * factor);
		if (playerSpeed_.Length() > FULL_SPEED_CHANGE)
		{
			mainRender_->ChangeAnimation("FullSpeedRight", "sonicRight.bmp");
			//ChangeState(ePlayerState::FULLSPEED_LEFT);
		}
		else
		{
			mainRender_->ChangeAnimation("WalkRight", "sonicRight.bmp");
		}
	}

	// 도는 행위에 진입
	if (bRollRight_)
	{
		if (nullptr != groundCheckRollCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::ROLL_RIGHT)))
		{
			rotation_ = 0.0f;
			ChangeState(ePlayerState::ROLLING_RIGHT);
			bRolling_ = true;
			return;
		}
		else
		{
			bRolling_ = false;
		}
	}
	else if (bRollUp_)
	{
		if (nullptr != groundCheckRollCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::ROLL_UP)))
		{
			rotation_ = 0.0f;
			ChangeState(ePlayerState::ROLLING_UP);
			bRolling_ = true;
			return;
		}
		else
		{
			bRolling_ = false;
		}
	}
	else if (false == bAngled_ && nullptr != groundCheckRollCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::ROLL_RIGHT)))
	{
		bAngled_ = true;
		bRolling_ = true;
		ChangeState(ePlayerState::WALK_RIGHT_ANGLED);
		return;
	}

	// 점프대
	if (playerSpeed_.x > 300.f && nullptr != jumpBoardCheckCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
	{
		gravitySpeed_.y = -550.f;
		ChangeState(ePlayerState::JUMP_BOARD);
		return;
	}

	// 나무타기
	if (playerSpeed_.x > 450.f && nullptr != woodRunCheckCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
	{
		woodRunTime_ = TORAD(65.f);
		playerSpeed_.x = 500.f;
		bSpin_ = false;
		level_->FindActor<Wood>("Wood")->WoodRun();
		ChangeState(ePlayerState::WOOD_RUNNING);
		return;
	}

	if (bSpin_)
	{
		noLimitMove(_deltaTime);
	}
	else
	{
		normalMove(_deltaTime);
	}
}

void Player::onWalkLeftAngled(float _deltaTime)
{
}

void Player::onWalkRightAngled(float _deltaTime)
{
	mainRender_->SetMask("sonicRight_mask.bmp");

	const float pivotLenth = 3.f;
	if (bAngled_)
	{
		bottomRightRollCollision_->SetCheckColor(static_cast<int>(eCollisionCheckColor::ROLL_RIGHT));
		if (rotation_ < 90.f)
		{
			bottomRightRollCollision_->SetPivot(float4::RIGHT * pivotLenth);
		}
		else if (rotation_ <= 360.f && rotation_ > 270)
		{
			bottomRightRollCollision_->SetPivot(float4::UP * pivotLenth);
		}
		while (nullptr != bottomRightRollCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::ROLL_RIGHT)))
		{
			float4 temp = bottomRightRollCollision_->GetPivot();
			temp = temp.Rotate2DByDegree(-10);
			bottomRightRollCollision_->SetPivot(temp);
		}
	}

	static float delay = 0.1f;
	if (delay < 0.0f)
	{
		float tempRotate = mainRender_->GetRotate();
		if (rotation_ > tempRotate)
		{
			mainRender_->SetRotate(rotation_);
		}
	}
	delay -= _deltaTime;

	setForwardRight();
	rotation_ = forwardVector_.GetDegree();

	while (nullptr != bottomCenterRollRightCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::ROLL_RIGHT)))
	{
		SetMovement(upVector_);
	}

	// 중력이 작용된다고 보는 시점

	playerSpeed_.x += GRAVITY_POWER * forwardVector_.y * _deltaTime;

	if (true == RSInput::GetInstance().IsKeyPress("Right"))
	{
		playerSpeed_.x += NORMAL_ACCELATION * _deltaTime;
	}

	// 속도에 따라 애니메이션 바뀌는 부분
	if (!bSpin_)
	{
		float factor = (abs(playerSpeed_.x) / 250.f);
		if (factor > 1.0f)
		{
			factor = 1.0f;
		}

		mainRender_->SetFrameTime("WalkRight", 0.3f - 0.25f * factor);
		if (playerSpeed_.Length() > FULL_SPEED_CHANGE)
		{
			mainRender_->ChangeAnimation("FullSpeedRight", "sonicRight.bmp");
			//ChangeState(ePlayerState::FULLSPEED_LEFT);
		}
		else
		{
			mainRender_->ChangeAnimation("WalkRight", "sonicRight.bmp");
		}
	}

	// 속력이 모자랄 때
	if (playerSpeed_.x < 50.f)
	{
		bAngled_ = false;
		ChangeState(ePlayerState::FALLING_RIGHT);
	}

	// 점프
	if (bCanJump_ && bGround_)
	{
		jumpTime_ = JUMP_TIME;
		if (true == RSInput::GetInstance().IsKeyDown("Jump"))
		{
			gravitySpeed_ = upVector_ * GRAVITY_POWER;
			gravitySpeed_.y = gravitySpeed_.y + upVector_.y * GRAVITY_POWER;
			playerSpeed_ = float4::ZERO;
			jumpTime_ = JUMP_TIME;
			jumpSound_->Play();
			rotation_ = 0.0f;
			bRolling_ = false;
			bAngled_ = false;
			mainRender_->SetRotate(0.0f);
			SetMovement(upVector_ * 2);
			ChangeState(ePlayerState::JUMP_RIGHT);
			return;
		}
	}

	SetMovement(forwardVector_ * playerSpeed_.x * _deltaTime);

	if (nullptr != bottomCenterRollPostCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::ROLL_RIGHT)))
	{
		gravitySpeed_.y = playerSpeed_.x * -forwardVector_.x;
		playerSpeed_.x = playerSpeed_.x * -forwardVector_.y;
		rotation_ = 0.0f;
		lerpRatio_ = 0.0f;
		bRolling_ = false;
		ChangeState(ePlayerState::WALK_RIGHT);
		return;
	}
}

void Player::onFullSpeedLeft(float _deltaTime)
{
}

void Player::onFullSpeedRight(float _deltaTime)
{
}

void Player::onFullSpeedLeftAngled(float _deltaTime)
{
}

void Player::onFullSpeedRightAngled(float _deltaTime)
{
}

void Player::onRollingLeft(float _deltaTime)
{
	mainRender_->SetMask("sonicLeft_mask.bmp");
	bLeftRollStartForSkillLocation_ = true;
	const float pivotLenth = 3.f;
	if (bRollLeft_)
	{
		bottomLeftRollCollision_->SetCheckColor(static_cast<int>(eCollisionCheckColor::ROLL_LEFT));
		if (rotation_ > 90.f)
		{
			bottomLeftRollCollision_->SetPivot(float4::UP * pivotLenth);
		}
		else if (rotation_ >= 0.f)
		{
			bottomLeftRollCollision_->SetPivot(float4::LEFT * pivotLenth);
		}
		while (nullptr != bottomLeftRollCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::ROLL_LEFT)))
		{
			float4 temp = bottomLeftRollCollision_->GetPivot();
			temp = temp.Rotate2DByDegree(10);
			bottomLeftRollCollision_->SetPivot(temp);
		}
	}
	else if (bRollRight_)
	{
		if (rotation_ >= 270.f)
		{
			bottomLeftRollCollision_->SetPivot(float4::DOWN * pivotLenth);
		}
		else if (rotation_ >= 180.f)
		{
			bottomLeftRollCollision_->SetPivot(float4::RIGHT * pivotLenth);
		}
		while (nullptr != bottomLeftRollCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::ROLL_RIGHT)))
		{
			float4 temp = bottomLeftRollCollision_->GetPivot();
			temp = temp.Rotate2DByDegree(10);
			bottomLeftRollCollision_->SetPivot(temp);
		}
	}

	static float delay = 0.1f;
	if (delay < 0.0f)
	{
		float tempRotate = mainRender_->GetRotate();
		if (-rotation_ < tempRotate)
		{
			mainRender_->SetRotate(-rotation_);
		}
	}
	delay -= _deltaTime;

	if (bRollLeft_ && nullptr != bottomCenterRollPostCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::ROLL_LEFT)))
	{
		bRollRight_ = true;
		bRollLeft_ = false;

		bottomLeftRollCollision_->SetCheckColor(static_cast<int>(eCollisionCheckColor::ROLL_RIGHT));
		//delay = 0.1f;
	}
	else if (bRollRight_ && nullptr != bottomCenterCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
	{
		bRolling_ = false;
		rotation_ = 0.0f;
		mainRender_->SetRotate(0.0f);
		groundCheckRollCollision_->SetCheckColor(static_cast<int>(eCollisionCheckColor::ROLL_RIGHT));
		ChangeState(ePlayerState::WALK_LEFT);
		return;
	}

	setForwardLeft();
	rotation_ = forwardVector_.GetDegreeLeftXAxis();

	while (nullptr != bottomCenterRollRightCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::ROLL_RIGHT)))
	{
		SetMovement(upVector_);
	}

	// 중력이 작용된다고 보는 시점
	playerSpeed_.x += GRAVITY_POWER * -forwardVector_.y * _deltaTime;

	if (true == RSInput::GetInstance().IsKeyPress("Left"))
	{
		playerSpeed_.x -= NORMAL_ACCELATION * _deltaTime;
	}

	// 속도에 따라 애니메이션 바뀌는 부분
	if (!bSpin_)
	{
		float factor = (abs(playerSpeed_.x) / 250.f);
		if (factor > 1.0f)
		{
			factor = 1.0f;
		}

		mainRender_->SetFrameTime("WalkLeft", 0.3f - 0.25f * factor);
		if (playerSpeed_.Length() > FULL_SPEED_CHANGE)
		{
			mainRender_->ChangeAnimation("FullSpeedLeft", "sonicLeft.bmp");
			//ChangeState(ePlayerState::FULLSPEED_LEFT);
		}
		else
		{
			mainRender_->ChangeAnimation("WalkLeft", "sonicLeft.bmp");
		}
	}

	// 속력이 모자랄 때
	if (playerSpeed_.x > -50.f)
	{
		ChangeState(ePlayerState::FALLING_LEFT);
	}

	// 점프
	if (bCanJump_ && bGround_)
	{
		jumpTime_ = JUMP_TIME;
		if (true == RSInput::GetInstance().IsKeyDown("Jump"))
		{
			gravitySpeed_ = upVector_ * GRAVITY_POWER;
			gravitySpeed_.y = gravitySpeed_.y + upVector_.y * GRAVITY_POWER;
			playerSpeed_ = float4::ZERO;
			jumpTime_ = JUMP_TIME;
			jumpSound_->Play();
			rotation_ = 0.0f;
			bRolling_ = false;
			mainRender_->SetRotate(0.0f);
			SetMovement(upVector_ * 2);
			ChangeState(ePlayerState::JUMP_LEFT);
			return;
		}
	}

	SetMovement(forwardVector_ * -playerSpeed_.x * _deltaTime);
}

void Player::onRollingRight(float _deltaTime)
{
	mainRender_->SetMask("sonicRight_mask.bmp");
	bLeftRollStartForSkillLocation_ = false;
	const float pivotLenth = 3.f;
	if (bRollRight_)
	{
		bottomRightRollCollision_->SetCheckColor(static_cast<int>(eCollisionCheckColor::ROLL_RIGHT));
		if (rotation_ < 90.f)
		{
			bottomRightRollCollision_->SetPivot(float4::RIGHT * pivotLenth);
		}
		else if (rotation_ <= 360.f && rotation_ > 270)
		{
			bottomRightRollCollision_->SetPivot(float4::UP * pivotLenth);
		}
		while (nullptr != bottomRightRollCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::ROLL_RIGHT)))
		{
			float4 temp = bottomRightRollCollision_->GetPivot();
			temp = temp.Rotate2DByDegree(-10);
			bottomRightRollCollision_->SetPivot(temp);
		}
	}
	else if (bRollLeft_)
	{
		if (rotation_ >= 270.f || rotation_ < 20.f)
		{
			bottomRightRollCollision_->SetPivot(float4::DOWN * pivotLenth);
		}
		else if (rotation_ >= 180.f)
		{
			bottomRightRollCollision_->SetPivot(float4::LEFT * pivotLenth);
		}
		while (nullptr != bottomRightRollCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::ROLL_LEFT)))
		{
			float4 temp = bottomRightRollCollision_->GetPivot();
			temp = temp.Rotate2DByDegree(-10);
			bottomRightRollCollision_->SetPivot(temp);
		}
	}

	static float delay = 0.1f;
	if (delay < 0.0f)
	{
		float tempRotate = mainRender_->GetRotate();
		if (rotation_ > tempRotate)
		{
			mainRender_->SetRotate(rotation_);
		}
	}
	delay -= _deltaTime;

	if (bRollRight_ && nullptr != bottomCenterRollPostCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::ROLL_RIGHT)))
	{
		bRollRight_ = false;
		bRollLeft_ = true;

		bottomRightRollCollision_->SetCheckColor(static_cast<int>(eCollisionCheckColor::ROLL_LEFT));
		//delay = 0.1f;
	}
	else if (bRollLeft_ && nullptr != bottomCenterCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
	{
		bRolling_ = false;
		rotation_ = 0.0f;
		mainRender_->SetRotate(0.0f);
		groundCheckRollCollision_->SetCheckColor(static_cast<int>(eCollisionCheckColor::ROLL_LEFT));
		ChangeState(ePlayerState::WALK_RIGHT);
		return;
	}

	setForwardRight();
	rotation_ = forwardVector_.GetDegree();

	while (nullptr != bottomCenterRollRightCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::ROLL_RIGHT)))
	{
		SetMovement(upVector_);
	}

	// 중력이 작용된다고 보는 시점

	playerSpeed_.x += GRAVITY_POWER * forwardVector_.y * _deltaTime;

	if (true == RSInput::GetInstance().IsKeyPress("Right"))
	{
		playerSpeed_.x += NORMAL_ACCELATION * _deltaTime;
	}

	// 속도에 따라 애니메이션 바뀌는 부분
	if (!bSpin_)
	{
		float factor = (abs(playerSpeed_.x) / 250.f);
		if (factor > 1.0f)
		{
			factor = 1.0f;
		}

		mainRender_->SetFrameTime("WalkRight", 0.3f - 0.25f * factor);
		if (playerSpeed_.Length() > FULL_SPEED_CHANGE)
		{
			mainRender_->ChangeAnimation("FullSpeedRight", "sonicRight.bmp");
			//ChangeState(ePlayerState::FULLSPEED_LEFT);
		}
		else
		{
			mainRender_->ChangeAnimation("WalkRight", "sonicRight.bmp");
		}
	}

	// 속력이 모자랄 때
	if (playerSpeed_.x < 50.f)
	{
		ChangeState(ePlayerState::FALLING_RIGHT);
	}

	// 점프
	if (bCanJump_ && bGround_)
	{
		jumpTime_ = JUMP_TIME;
		if (true == RSInput::GetInstance().IsKeyDown("Jump"))
		{
			gravitySpeed_ = upVector_ * GRAVITY_POWER;
			gravitySpeed_.y = gravitySpeed_.y + upVector_.y * GRAVITY_POWER;
			playerSpeed_ = float4::ZERO;
			jumpTime_ = JUMP_TIME;
			jumpSound_->Play();
			rotation_ = 0.0f;
			bRolling_ = false;
			mainRender_->SetRotate(0.0f);
			SetMovement(upVector_ * 2);
			ChangeState(ePlayerState::JUMP_RIGHT);
			return;
		}
	}

	SetMovement(forwardVector_ * playerSpeed_.x * _deltaTime);
}

void Player::onRollingUp(float _deltaTime)
{
	mainRender_->SetMask("sonicRight_mask.bmp");
	const float pivotLenth = 3.f;
	if (bRollUp_)
	{
		bottomRightRollCollision_->SetCheckColor(static_cast<int>(eCollisionCheckColor::ROLL_UP));
		if (rotation_ < 90.f)
		{
			bottomRightRollCollision_->SetPivot(float4::RIGHT * pivotLenth);
		}
		else if (rotation_ <= 360.f && rotation_ > 270)
		{
			bottomRightRollCollision_->SetPivot(float4::UP * pivotLenth);
		}
		while (nullptr != bottomRightRollCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::ROLL_UP)))
		{
			float4 temp = bottomRightRollCollision_->GetPivot();
			temp = temp.Rotate2DByDegree(-10);
			bottomRightRollCollision_->SetPivot(temp);
		}
	}

	static float delay = 0.1f;
	if (delay < 0.0f)
	{
		float tempRotate = mainRender_->GetRotate();
		if (rotation_ > tempRotate)
		{
			mainRender_->SetRotate(rotation_);
		}
	}
	delay -= _deltaTime;

	if (bRollUp_ && nullptr != bottomCenterRollPostCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::ROLL_UP)))
	{
		bRollUp_ = false;
		bRollRight_ = true;

		bottomRightRollCollision_->SetCheckColor(static_cast<int>(eCollisionCheckColor::ROLL_RIGHT));
		ChangeState(ePlayerState::ROLLING_RIGHT);
	}

	setForwardRight();
	rotation_ = forwardVector_.GetDegree();

	while (nullptr != bottomCenterRollUpCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::ROLL_RIGHT)))
	{
		SetMovement(upVector_);
	}

	// 중력이 작용된다고 보는 시점

	playerSpeed_.x += GRAVITY_POWER / 2.f * forwardVector_.y * _deltaTime;

	if (true == RSInput::GetInstance().IsKeyPress("Right"))
	{
		playerSpeed_.x += NORMAL_ACCELATION * _deltaTime;
	}

	// 속도에 따라 애니메이션 바뀌는 부분
	if (!bSpin_)
	{
		float factor = (abs(playerSpeed_.x) / 250.f);
		if (factor > 1.0f)
		{
			factor = 1.0f;
		}

		mainRender_->SetFrameTime("WalkRight", 0.3f - 0.25f * factor);
		if (playerSpeed_.Length() > FULL_SPEED_CHANGE)
		{
			mainRender_->ChangeAnimation("FullSpeedRight", "sonicRight.bmp");
			//ChangeState(ePlayerState::FULLSPEED_LEFT);
		}
		else
		{
			mainRender_->ChangeAnimation("WalkRight", "sonicRight.bmp");
		}
	}

	// 속력이 모자랄 때
	if (playerSpeed_.x < 50.f)
	{
		ChangeState(ePlayerState::FALLING_UP);
	}

	// 점프
	if (bCanJump_ && bGround_)
	{
		jumpTime_ = JUMP_TIME;
		if (true == RSInput::GetInstance().IsKeyDown("Jump"))
		{
			gravitySpeed_ = upVector_ * GRAVITY_POWER;
			gravitySpeed_.y = gravitySpeed_.y + upVector_.y * GRAVITY_POWER;
			playerSpeed_ = float4::ZERO;
			jumpTime_ = JUMP_TIME;
			jumpSound_->Play();
			rotation_ = 0.0f;
			bRolling_ = false;
			mainRender_->SetRotate(0.0f);
			ChangeState(ePlayerState::JUMP_RIGHT);
			return;
		}
	}

	SetMovement(forwardVector_ * playerSpeed_.x * _deltaTime);
}

void Player::onJumpLeft(float _deltaTime)
{
	rollSideCheck();

	accelation_ = 0.0f;
	bGround_ = false;
	bSpin_ = false;
	mainRender_->ChangeAnimation("JumpLeft", "sonicLeft.bmp");

	if (bCanJump_)
	{
		jumpTime_ -= _deltaTime;
		gravitySpeed_ = upVector_ * JUMP_POWER;
		if (jumpTime_ > 0.2f)
		{

		}
		else if (true == RSInput::GetInstance().IsKeyFree("Jump") || jumpTime_ < 0.0f)
		{
			gravitySpeed_.y = gravitySpeed_.y * 0.5f;
			bCanJump_ = false;
		}
	}
	else
	{
		if (nullptr == groundCheckCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
		{
			if (gravityAccelation_.y < GRAVITY_POWER)
			{
				gravityAccelation_.y += GRAVITY_POWER;
			}
		}
		else
		{
			// 방울스킬
			if (bUsingBubbleSkill_ && skillType_ == eMonitor::BUBBLE)
			{
				jumpTime_ = JUMP_TIME;
				bUsingBubbleSkill_ = false;
				bCanJump_ = true;
				skill_->bubbleSkillOff();
				RSSoundManager::GetInstance().PlaySoundByName("bubbleShieldSkill.wav");
				return;
			}

			bUsingFireSkill_ = false;
			bUsingElectricSkill_ = false;
			gravitySpeed_.x = 0.0f;
			gravitySpeed_.y = 0.0f;
			bGround_ = true;
			bCanJump_ = true;
			if (playerSpeed_.x > 1.0f)
			{
				ChangeState(ePlayerState::WALK_RIGHT);
			}
			else
			{
				ChangeState(ePlayerState::WALK_LEFT);
			}
		}
	}

	// 스킬
	if (RSInput::GetInstance().IsKeyDown("Jump"))
	{
		switch (skillType_)
		{
		case eMonitor::ELECTRIC:
			if (false == bUsingElectricSkill_)
			{
				gravitySpeed_.y = -400.f;
				bUsingElectricSkill_ = true;
				RSSoundManager::GetInstance().PlaySoundByName("electricShieldSkill.wav");
				skill_->electricSkillOn();
			}
			break;
		case eMonitor::FIRE:
			if (false == bUsingFireSkill_)
			{
				jumpTime_ = 0.0f;
				bUsingFireSkill_ = true;
				RSSoundManager::GetInstance().PlaySoundByName("fireShieldSkill.wav");
				if (RSInput::GetInstance().IsKeyPress("Right"))
				{
					skill_->fireSkillRightOn();
					playerSpeed_.x = 500.f;
					ChangeState(ePlayerState::JUMP_RIGHT);
					return;
				}
				else
				{
					skill_->fireSkillLeftOn();
					playerSpeed_.x = -500.f;
				}
			}
			break;
		case eMonitor::BUBBLE:
			if (false == bUsingBubbleSkill_)
			{
				jumpTime_ = 0.0f;
				bCanJump_ = false;
				bUsingBubbleSkill_ = true;
				gravitySpeed_.y = 500.f;
				skill_->bubbleSkillOn();
				RSSoundManager::GetInstance().PlaySoundByName("bubbleShieldSkill.wav");
			}
			break;
		default:
			break;
		}
	}

	// 움직임
	// 충돌 체크
	{
		if (RSInput::GetInstance().IsKeyPress("Left"))
		{
			if (nullptr != centerLeftCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
			{
				SetMovement({ 1.f, 0.0f });
				playerSpeed_.x = 0;
			}
			else
			{
				accelation_ += NORMAL_ACCELATION * 2;
			}
		}
		else if (RSInput::GetInstance().IsKeyPress("Right"))
		{
			if (nullptr != centerRightCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
			{
				SetMovement({ -1.f, 0.0f });
				playerSpeed_.x = 0;
			}
			else
			{
				accelation_ -= NORMAL_ACCELATION * 2;
			}
		}
	}

	// 위 충돌 체크
	if (nullptr != topCenterCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
	{
		playerSpeed_.y = 0;
		gravitySpeed_.y = 0.0f;
		gravityAccelation_.y = 0.0f;
		jumpTime_ = 0.0f;
		SetMovement({ 0.0f, 1.0f });
	}

	// 도는 행위에 진입
	if (bRollLeft_)
	{
		if (nullptr != groundCheckRollCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::ROLL_LEFT)))
		{
			rotation_ = 0.0f;
			bCanJump_ = true;
			ChangeState(ePlayerState::ROLLING_LEFT);
			bRolling_ = true;
			return;
		}
		else
		{
			bRolling_ = false;
		}
	}

	float4 nextPlayerSpeed = playerSpeed_ + (direction_ * accelation_ * _deltaTime);

	if (nextPlayerSpeed.Length() < NORMAL_SPEED_LIMIT)
	{
		playerSpeed_ = nextPlayerSpeed;
	}
	else
	{
		while (playerSpeed_.Length() > NORMAL_SPEED_LIMIT)
		{
			playerSpeed_ = playerSpeed_ * 0.8f;
		}
	}

	gravitySpeed_ += gravityAccelation_ * _deltaTime;

	actualSpeed_ = playerSpeed_ + gravitySpeed_;

	if (nullptr != centerLeftCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
	{
		SetMovement({ 1.f, 0.0f });
		playerSpeed_.x = 0;
	}
	else if (nullptr != centerRightCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
	{
		SetMovement({ -1.f, 0.0f });
		playerSpeed_.x = 0;
	}
	else
	{
		SetMovement(actualSpeed_ * _deltaTime);
	}

	if (nullptr != bottomCenterCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
	{
		SetMovement({ 0.0f, -1.0f });
		location_.y = static_cast<float>(static_cast<int>(location_.y));
	}
}

void Player::onJumpRight(float _deltaTime)
{
	rollSideCheck();

	accelation_ = 0.0f;
	bGround_ = false;
	bSpin_ = false;
	if (false == bAngled_)
	{
		mainRender_->ChangeAnimation("JumpRight", "sonicRight.bmp");
	}
	if (bCanJump_)
	{
		jumpTime_ -= _deltaTime;
		gravitySpeed_ = upVector_ * JUMP_POWER;
		if (jumpTime_ > 0.2f)
		{

		}
		else if (true == RSInput::GetInstance().IsKeyFree("Jump") || jumpTime_ < 0.0f)
		{
			gravitySpeed_.y = gravitySpeed_.y * 0.5f;
			bCanJump_ = false;
		}
	}
	else
	{
		if (nullptr == groundCheckCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
		{
			if (gravityAccelation_.y < GRAVITY_POWER)
			{
				gravityAccelation_.y += GRAVITY_POWER;
			}
			//else if (gravityAccelation_.y < 640.f)
			//{
			//	gravityAccelation_.y += 120.f * _deltaTime;
			//}
		}
		else
		{
			// 방울스킬
			if (bUsingBubbleSkill_ && skillType_ == eMonitor::BUBBLE)
			{
				jumpTime_ = JUMP_TIME;
				bUsingBubbleSkill_ = false;
				bCanJump_ = true;
				skill_->bubbleSkillOff();
				RSSoundManager::GetInstance().PlaySoundByName("bubbleShieldSkill.wav");
				return;
			}

			bUsingFireSkill_ = false;
			bUsingElectricSkill_ = false;
			gravitySpeed_.x = 0.0f;
			gravitySpeed_.y = 0.0f;
			bGround_ = true;
			bCanJump_ = true;
			if (playerSpeed_.x < -1.0f)
			{
				ChangeState(ePlayerState::WALK_LEFT);
			}
			else
			{
				ChangeState(ePlayerState::WALK_RIGHT);
			}
		}
	}

	// 스킬
	if (RSInput::GetInstance().IsKeyDown("Jump"))
	{
		switch (skillType_)
		{
		case eMonitor::ELECTRIC:
			if (false == bUsingElectricSkill_)
			{
				gravitySpeed_.y = -400.f;
				bUsingElectricSkill_ = true;
				RSSoundManager::GetInstance().PlaySoundByName("electricShieldSkill.wav");
				skill_->electricSkillOn();
			}
			break;
		case eMonitor::FIRE:
			if (false == bUsingFireSkill_)
			{
				jumpTime_ = 0.0f;
				bUsingFireSkill_ = true;
				RSSoundManager::GetInstance().PlaySoundByName("fireShieldSkill.wav");
				if (RSInput::GetInstance().IsKeyPress("Left"))
				{
					skill_->fireSkillLeftOn();
					playerSpeed_.x = -500.f;
					ChangeState(ePlayerState::JUMP_LEFT);
				}
				else
				{
					skill_->fireSkillRightOn();
					playerSpeed_.x = 500.f;
				}
			}
			break;
		case eMonitor::BUBBLE:
			if (false == bUsingBubbleSkill_)
			{
				jumpTime_ = 0.0f;
				bCanJump_ = false;
				bUsingBubbleSkill_ = true;
				gravitySpeed_.y = 500.f;
				skill_->bubbleSkillOn();
				RSSoundManager::GetInstance().PlaySoundByName("bubbleShieldSkill.wav");
			}
			break;
		default:
			break;
		}
	}

	// 움직임
	// 충돌 체크
	{
		if (RSInput::GetInstance().IsKeyPress("Left"))
		{
			if (nullptr != centerLeftCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
			{
				SetMovement({ 1.f, 0.0f });
				playerSpeed_.x = 0;
				return;
			}
			else
			{
				accelation_ -= NORMAL_ACCELATION * 2;
			}
		}
		else if (RSInput::GetInstance().IsKeyPress("Right"))
		{
			if (nullptr != centerRightCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
			{
				SetMovement({ -1.f, 0.0f });
				playerSpeed_.x = 0;
				return;
			}
			else
			{
				accelation_ += NORMAL_ACCELATION * 2;
			}
		}
	}

	// 위 충돌 체크
	if (nullptr != topCenterCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
	{
		playerSpeed_.y = 0;
		gravitySpeed_.y = 0.0f;
		gravityAccelation_.y = 0.0f;
		jumpTime_ = 0.0f;
		SetMovement({ 0.0f, 1.0f });
	}

	// 도는 행위에 진입
	if (bRollRight_)
	{
		if (nullptr != groundCheckRollCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::ROLL_RIGHT)))
		{
			bCanJump_ = true;
			rotation_ = 0.0f;
			ChangeState(ePlayerState::ROLLING_RIGHT);
			bRolling_ = true;
			return;
		}
		else
		{
			bRolling_ = false;
		}
	}
	else if (bRollUp_)
	{
		if (nullptr != groundCheckRollCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::ROLL_UP)))
		{
			bCanJump_ = true;
			rotation_ = 0.0f;
			ChangeState(ePlayerState::ROLLING_UP);
			bRolling_ = true;
			return;
		}
		else
		{
			bRolling_ = false;
		}
	}
	else if (false == bAngled_ && nullptr != groundCheckRollCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::ROLL_RIGHT)))
	{
		bAngled_ = true;
		bRolling_ = true;
		bCanJump_ = true;
		ChangeState(ePlayerState::WALK_RIGHT_ANGLED);
		return;
	}

	float4 nextPlayerSpeed = playerSpeed_ + (direction_ * accelation_ * _deltaTime);

	if (nextPlayerSpeed.Length() < NORMAL_SPEED_LIMIT)
	{
		playerSpeed_ = nextPlayerSpeed;
	}
	else
	{
		while (playerSpeed_.Length() > NORMAL_SPEED_LIMIT)
		{
			playerSpeed_ = playerSpeed_ * 0.8f;
		}
	}

	gravitySpeed_ += gravityAccelation_ * _deltaTime;

	actualSpeed_ = playerSpeed_ + gravitySpeed_;

	if (nullptr != centerLeftCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
	{
		SetMovement({ 1.f, 0.0f });
		playerSpeed_.x = 0;
	}
	else if (nullptr != centerRightCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
	{
		SetMovement({ -1.f, 0.0f });
		playerSpeed_.x = 0;
	}
	else
	{
		SetMovement(actualSpeed_ * _deltaTime);
	}

	if (nullptr != bottomCenterCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
	{
		SetMovement({ 0.0f, -1.0f });
		location_.y = static_cast<float>(static_cast<int>(location_.y));
	}
}

void Player::onJumpShieldLeft(float _deltaTime)
{
}

void Player::onJumpShieldRight(float _deltaTime)
{
}

void Player::onJumpSpringLeft(float _deltaTime)
{
}

void Player::onJumpStringRight(float _deltaTime)
{
}

void Player::onSkidLeft(float _deltaTime)
{
	mapCollisionSideCheck();
	rollSideCheck();
	processGravity(_deltaTime);
	setForwardLeft();

	// 좌측 충돌 체크
	if (nullptr != centerLeftCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
	{
		playerSpeed_.x = 0;
		mainRender_->ChangeAnimation("IdleLeft", "sonicLeft.bmp");
		ChangeState(ePlayerState::IDLE_LEFT);
		return;
	}

	// 좌측 모니터 체크
	RSCollision* collisionMonitor = centerLeftCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MONITOR));
	if (nullptr != collisionMonitor)
	{
		Monitor* monitor = dynamic_cast<Monitor*>(collisionMonitor->GetActor());
		if (nullptr != monitor && monitor->IsActivate())
		{
			playerSpeed_.x = 0;
			mainRender_->ChangeAnimation("PushLeft", "sonicLeft.bmp");
			ChangeState(ePlayerState::PUSH_LEFT);
			return;
		}
	}

	// 점프
	if (bCanJump_ && bGround_)
	{
		jumpTime_ = JUMP_TIME;
		if (true == RSInput::GetInstance().IsKeyDown("Jump"))
		{
			gravitySpeed_ = forwardVector_ * playerSpeed_.x;
			jumpTime_ = JUMP_TIME;
			jumpSound_->Play();
			ChangeState(ePlayerState::JUMP_LEFT);
			return;
		}
	}

	if (RSInput::GetInstance().IsKeyFree("Right"))
	{
		ChangeState(ePlayerState::WALK_LEFT);
		return;
	}

	playerSpeed_.x += 1000 * _deltaTime;

	if (playerSpeed_.x > 0)
	{
		ChangeState(ePlayerState::WALK_RIGHT);
	}

	normalMove(_deltaTime);
}

void Player::onSkidRight(float _deltaTime)
{
	mapCollisionSideCheck();
	rollSideCheck();
	processGravity(_deltaTime);
	setForwardRight();

	// 우측 충돌 체크
	if (nullptr != centerRightCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
	{
		playerSpeed_.x = 0;
		mainRender_->ChangeAnimation("IdleRight", "sonicRight.bmp");
		ChangeState(ePlayerState::IDLE_RIGHT);
		return;
	}

	// 우측 모니터 체크
	RSCollision* collisionMonitor = centerRightCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MONITOR));
	if (nullptr != collisionMonitor)
	{
		Monitor* monitor = dynamic_cast<Monitor*>(collisionMonitor->GetActor());
		if (nullptr != monitor && monitor->IsActivate())
		{
			playerSpeed_.x = 0;
			mainRender_->ChangeAnimation("PushRight", "sonicRight.bmp");
			ChangeState(ePlayerState::PUSH_RIGHT);
			return;
		}
	}

	// 점프
	if (bCanJump_ && bGround_)
	{
		jumpTime_ = JUMP_TIME;
		if (true == RSInput::GetInstance().IsKeyDown("Jump"))
		{
			gravitySpeed_ = forwardVector_ * playerSpeed_.x;
			jumpTime_ = JUMP_TIME;
			jumpSound_->Play();
			ChangeState(ePlayerState::JUMP_RIGHT);
			return;
		}
	}

	if (RSInput::GetInstance().IsKeyFree("Left"))
	{
		ChangeState(ePlayerState::WALK_RIGHT);
		return;
	}

	playerSpeed_.x -= 1000 * _deltaTime;

	if (playerSpeed_.x < 0)
	{
		ChangeState(ePlayerState::WALK_LEFT);
	}

	normalMove(_deltaTime);
}

void Player::onPushLeft(float _deltaTime)
{
	if (false == RSInput::GetInstance().IsKeyPress("Left"))
	{
		mainRender_->ChangeAnimation("IdleLeft", "sonicLeft.bmp");
		ChangeState(ePlayerState::IDLE_LEFT);
	}

	// 점프
	if (bCanJump_ && bGround_)
	{
		jumpTime_ = JUMP_TIME;
		if (true == RSInput::GetInstance().IsKeyDown("Jump"))
		{
			gravitySpeed_ = forwardVector_ * playerSpeed_.x;
			jumpTime_ = JUMP_TIME;
			jumpSound_->Play();
			ChangeState(ePlayerState::JUMP_LEFT);
			return;
		}
	}
}

void Player::onPushRight(float _deltaTime)
{
	if (false == RSInput::GetInstance().IsKeyPress("Right"))
	{
		mainRender_->ChangeAnimation("IdleRight", "sonicRight.bmp");
		ChangeState(ePlayerState::IDLE_RIGHT);
	}

	// 점프
	if (bCanJump_ && bGround_)
	{
		jumpTime_ = JUMP_TIME;
		if (true == RSInput::GetInstance().IsKeyDown("Jump"))
		{
			gravitySpeed_ = forwardVector_ * playerSpeed_.x;
			jumpTime_ = JUMP_TIME;
			jumpSound_->Play();
			ChangeState(ePlayerState::JUMP_RIGHT);
			return;
		}
	}
}

void Player::onFallingLeft(float _deltaTime)
{
	bLeftRollStartForSkillLocation_ = false;
	if (nullptr != groundCheckCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
	{
		rotation_ = 0.0f;
		mainRender_->SetRotate(rotation_);
		bRolling_ = false;
		ChangeState(ePlayerState::WALK_RIGHT);
		return;
	}

	const float pivotLenth = 3.f;

	bottomLeftRollCollision_->SetPivot(float4::DOWN * pivotLenth);

	while (nullptr != bottomLeftRollCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::ROLL_LEFT)))
	{
		float4 temp = bottomLeftRollCollision_->GetPivot();
		temp = temp.Rotate2DByDegree(-10);
		bottomLeftRollCollision_->SetPivot(temp);
	}

	setForwardLeft();
	upVector_ = upVector_ * -1.f;
	rotation_ = forwardVector_.GetDegreeLeftXAxis();

	static float delay = 0.1f;
	if (delay < 0.0f)
	{
		mainRender_->SetRotate(-rotation_ + 180);
		delay = 0.1f;
	}
	delay -= _deltaTime;

	playerSpeed_.x += GRAVITY_POWER * forwardVector_.y * _deltaTime;
	SetMovement(forwardVector_ * playerSpeed_.x * _deltaTime);
	forwardVector_ = forwardVector_ * -1.f;
}

void Player::onFallingRight(float _deltaTime)
{
	if (nullptr != groundCheckCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
	{
		rotation_ = 0.0f;
		mainRender_->SetRotate(rotation_);
		bRolling_ = false;
		ChangeState(ePlayerState::WALK_LEFT);
		return;
	}

	const float pivotLenth = 3.f;

	bottomRightRollCollision_->SetPivot(float4::DOWN * pivotLenth);

	while (nullptr != bottomRightRollCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::ROLL_RIGHT)))
	{
		float4 temp = bottomRightRollCollision_->GetPivot();
		temp = temp.Rotate2DByDegree(10);
		bottomRightRollCollision_->SetPivot(temp);
	}

	setForwardRight();
	forwardVector_ = forwardVector_ * -1.f;
	upVector_ = upVector_ * -1.f;
	rotation_ = forwardVector_.GetDegree();

	static float delay = 0.1f;
	if (delay < 0.0f)
	{
		mainRender_->SetRotate(rotation_);
		delay = 0.1f;
	}
	delay -= _deltaTime;

	playerSpeed_.x += GRAVITY_POWER * forwardVector_.y * _deltaTime;
	SetMovement(forwardVector_ * playerSpeed_.x * _deltaTime);
}

void Player::onFallingUp(float _deltaTime)
{
	if (nullptr != groundCheckCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
	{
		rotation_ = 0.0f;
		mainRender_->SetRotate(rotation_);
		bRolling_ = false;
		bottomRightRollCollision_->SetCheckColor(static_cast<DWORD>(eCollisionCheckColor::ROLL_RIGHT));
		ChangeState(ePlayerState::WALK_LEFT);
		return;
	}

	const float pivotLenth = 3.f;

	bottomRightRollCollision_->SetPivot(float4::DOWN * pivotLenth);

	while (nullptr != bottomRightRollCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::ROLL_UP)))
	{
		float4 temp = bottomRightRollCollision_->GetPivot();
		temp = temp.Rotate2DByDegree(10);
		bottomRightRollCollision_->SetPivot(temp);
	}

	setForwardRight();
	forwardVector_ = forwardVector_ * -1.f;
	upVector_ = upVector_ * -1.f;
	rotation_ = forwardVector_.GetDegree();

	static float delay = 0.1f;
	if (delay < 0.0f)
	{
		mainRender_->SetRotate(rotation_);
		delay = 0.1f;
	}
	delay -= _deltaTime;

	playerSpeed_.x += GRAVITY_POWER * forwardVector_.y * _deltaTime;
	SetMovement(forwardVector_ * playerSpeed_.x * _deltaTime);
}

void Player::onJumpBoard(float _deltaTime)
{
	mainRender_->ChangeAnimation("JumpBoardRight", "sonicRight.bmp");
	accelation_ = 0.0f;
	bGround_ = false;
	bSpin_ = false;

	float currentRenderRotate = mainRender_->GetRotate();
	currentRenderRotate = float4::Lerp({ currentRenderRotate, 0.0f }, { 0.0f, 0.0f }, lerpRatio_).x;
	mainRender_->SetRotate(currentRenderRotate);
	rotation_ = currentRenderRotate;

	if (lerpRatio_ > 1.f)
	{
		bAngled_ = false;
	}
	lerpRatio_ += _deltaTime * 0.1f;


	if (nullptr == groundCheckCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
	{
		if (gravityAccelation_.y < GRAVITY_POWER)
		{
			gravityAccelation_.y += GRAVITY_POWER;
		}
	}
	else
	{
		gravitySpeed_.x = 0.0f;
		gravitySpeed_.y = 0.0f;
		bGround_ = true;
		bCanJump_ = true;
		if (playerSpeed_.x < -1.0f)
		{
			ChangeState(ePlayerState::WALK_LEFT);
		}
		else
		{
			ChangeState(ePlayerState::WALK_RIGHT);
		}
	}

	// 움직임
	// 우측 충돌 체크
	if (nullptr != centerRightCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
	{
		playerSpeed_.x = 0;
	}
	else
	{
		if (RSInput::GetInstance().IsKeyPress("Left"))
		{
			accelation_ -= NORMAL_ACCELATION * 2;
		}
		else if (RSInput::GetInstance().IsKeyPress("Right"))
		{
			accelation_ += NORMAL_ACCELATION * 2;
		}
	}

	float4 nextPlayerSpeed = playerSpeed_ + (direction_ * accelation_ * _deltaTime);

	if (nextPlayerSpeed.Length() < NORMAL_SPEED_LIMIT)
	{
		playerSpeed_ = nextPlayerSpeed;
	}
	else
	{
		while (playerSpeed_.Length() > NORMAL_SPEED_LIMIT)
		{
			playerSpeed_ = playerSpeed_ * 0.8f;
		}
	}

	gravitySpeed_ += gravityAccelation_ * _deltaTime;

	actualSpeed_ = playerSpeed_ + gravitySpeed_;

	SetMovement(actualSpeed_ * _deltaTime);

	if (nullptr != bottomCenterCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
	{
		SetMovement({ 0.0f, -1.0f });
		location_.y = static_cast<float>(static_cast<int>(location_.y));
	}
}

void Player::onCrouchLeft(float _deltaTime)
{
	mainRender_->ChangeAnimation("CrouchLeft", "sonicLeft.bmp");
	if (RSInput::GetInstance().IsKeyUp("Down"))
	{
		ChangeState(ePlayerState::IDLE_LEFT);
		return;
	}

	if (RSInput::GetInstance().IsKeyDown("Jump"))
	{
		playerSpeed_.x = -300.f;
		spinSound_->Play();
		ChangeState(ePlayerState::SPIN_DASH_LEFT);
	}
}

void Player::onCrouchRight(float _deltaTime)
{
	mainRender_->ChangeAnimation("CrouchRight", "sonicRight.bmp");
	if (RSInput::GetInstance().IsKeyUp("Down"))
	{
		ChangeState(ePlayerState::IDLE_RIGHT);
	}

	if (RSInput::GetInstance().IsKeyDown("Jump"))
	{
		playerSpeed_.x = 300.f;
		spinSound_->Play();
		ChangeState(ePlayerState::SPIN_DASH_RIGHT);
	}
}

void Player::onLookUpLeft(float _deltaTime)
{
	mainRender_->ChangeAnimation("LookUpLeft", "sonicLeft.bmp");
	if (RSInput::GetInstance().IsKeyUp("Up"))
	{
		ChangeState(ePlayerState::IDLE_LEFT);
	}
}

void Player::onLookUpRight(float _deltaTime)
{
	mainRender_->ChangeAnimation("LookUpRight", "sonicRight.bmp");
	if (RSInput::GetInstance().IsKeyUp("Up"))
	{
		ChangeState(ePlayerState::IDLE_RIGHT);
	}
}

void Player::onSpinLeft(float _deltaTime)
{
	mainRender_->ChangeAnimation("SpinLeft", "sonicLeft.bmp");

	mapCollisionSideCheck();
	rollSideCheck();
	processGravity(_deltaTime);
	setForwardLeft();

	// 좌측 충돌 체크
	if (nullptr != centerLeftCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
	{
		playerSpeed_.x = 0;
		mainRender_->ChangeAnimation("IdleLeft", "sonicLeft.bmp");
		ChangeState(ePlayerState::IDLE_LEFT);
		return;
	}

	// 점프
	if (bCanJump_ && bGround_)
	{
		jumpTime_ = JUMP_TIME;
		if (true == RSInput::GetInstance().IsKeyDown("Jump"))
		{
			gravitySpeed_ = forwardVector_ * playerSpeed_.x;
			jumpTime_ = JUMP_TIME;
			jumpSound_->Play();
			ChangeState(ePlayerState::JUMP_LEFT);
			return;
		}
	}

	if (playerSpeed_.x < -1.f)
	{
		if (RSInput::GetInstance().IsKeyPress("Right"))
		{
			accelation_ -= NORMAL_ACCELATION * _deltaTime;
		}
		else
		{
			accelation_ -= 50.f * _deltaTime;
		}
	}
	else
	{
		playerSpeed_.x = 0.f;
		ChangeState(ePlayerState::IDLE_LEFT);
	}

	// 도는 행위에 진입
	if (bRollLeft_)
	{
		if (nullptr != groundCheckRollCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::ROLL_LEFT)))
		{
			rotation_ = 0.0f;
			ChangeState(ePlayerState::ROLLING_LEFT);
			bRolling_ = true;
			return;
		}
		else
		{
			bRolling_ = false;
		}
	}


	noLimitMove(_deltaTime);
}

void Player::onSpinRight(float _deltaTime)
{
	mainRender_->ChangeAnimation("SpinRight", "sonicRight.bmp");

	mapCollisionSideCheck();
	rollSideCheck();
	processGravity(_deltaTime);
	setForwardLeft();

	// 우측 충돌 체크
	if (nullptr != centerRightCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
	{
		playerSpeed_.x = 0;
		mainRender_->ChangeAnimation("IdleRight", "sonicRight.bmp");
		ChangeState(ePlayerState::IDLE_RIGHT);
		return;
	}

	// 점프
	if (bCanJump_ && bGround_)
	{
		jumpTime_ = JUMP_TIME;
		if (true == RSInput::GetInstance().IsKeyDown("Jump"))
		{
			gravitySpeed_ = forwardVector_ * playerSpeed_.x;
			jumpTime_ = JUMP_TIME;
			jumpSound_->Play();
			ChangeState(ePlayerState::JUMP_RIGHT);
			return;
		}
	}

	// 도는 행위에 진입
	if (bRollRight_)
	{
		if (nullptr != groundCheckRollCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::ROLL_RIGHT)))
		{
			rotation_ = 0.0f;
			ChangeState(ePlayerState::ROLLING_RIGHT);
			bRolling_ = true;
			return;
		}
		else
		{
			bRolling_ = false;
		}
	}
	else if (bRollUp_)
	{
		if (nullptr != groundCheckRollCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::ROLL_UP)))
		{
			rotation_ = 0.0f;
			ChangeState(ePlayerState::ROLLING_UP);
			bRolling_ = true;
			return;
		}
		else
		{
			bRolling_ = false;
		}
	}
	else if (false == bAngled_ && nullptr != groundCheckRollCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::ROLL_RIGHT)))
	{
		bAngled_ = true;
		bRolling_ = true;
		ChangeState(ePlayerState::WALK_RIGHT_ANGLED);
		return;
	}

	// 점프대
	if (playerSpeed_.x > 300.f && nullptr != jumpBoardCheckCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
	{
		gravitySpeed_.y = -550.f;
		ChangeState(ePlayerState::JUMP_BOARD);
		return;
	}

	// 나무타기
	if (playerSpeed_.x > 450.f && nullptr != woodRunCheckCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
	{
		woodRunTime_ = TORAD(65.f);
		playerSpeed_.x = 500.f;
		bSpin_ = false;
		level_->FindActor<Wood>("Wood")->WoodRun();
		ChangeState(ePlayerState::WOOD_RUNNING);
		return;
	}

	if (playerSpeed_.x > 1.f)
	{
		if (RSInput::GetInstance().IsKeyPress("Left"))
		{
			accelation_ -= NORMAL_ACCELATION * _deltaTime;
		}
		else
		{
			accelation_ -= 50.f * _deltaTime;
		}
	}
	else
	{
		playerSpeed_.x = 0.f;
		ChangeState(ePlayerState::IDLE_RIGHT);
	}

	noLimitMove(_deltaTime);
}

void Player::onSpinDashLeft(float _deltaTime)
{
	mainRender_->ChangeAnimation("SpinDashLeft", "sonicLeft.bmp");
	if (RSInput::GetInstance().IsKeyUp("Down"))
	{
		bSpin_ = true;
		spinDashSound_->Play();
		ChangeState(ePlayerState::SPIN_LEFT);
	}

	if (RSInput::GetInstance().IsKeyDown("Jump"))
	{
		if (spinSound_->IsPlaying())
		{
			spinSound_->Stop();
		}
		spinSound_->Play();
		spinSound_->SetPitch(abs(playerSpeed_.x) / 300.f);
		if (playerSpeed_.x > -700.f)
		{
			playerSpeed_.x -= 50;
		}
	}
}

void Player::onSpinDashRight(float _deltaTime)
{
	mainRender_->ChangeAnimation("SpinDashRight", "sonicRight.bmp");
	if (RSInput::GetInstance().IsKeyUp("Down"))
	{
		bSpin_ = true;
		spinDashSound_->Play();
		ChangeState(ePlayerState::SPIN_RIGHT);
	}

	if (RSInput::GetInstance().IsKeyDown("Jump"))
	{
		if (spinSound_->IsPlaying())
		{
			spinSound_->Stop();
		}
		spinSound_->Play();
		spinSound_->SetPitch(playerSpeed_.x / 300.f);
		if (playerSpeed_.x < 700.f)
		{
			playerSpeed_.x += 50;
		}
	}
}

void Player::onWoodRunning(float _deltaTime)
{
	mainRender_->ChangeAnimation("WoodRunning", "sonicRight.bmp");

	woodRunTime_ += _deltaTime * 4.6f;
	float directionX = cosf(woodRunTime_);
	direction_.x = directionX;

	if (RSInput::GetInstance().IsKeyPress("Right"))
	{
		if (playerSpeed_.x < 500.f)
		{
			playerSpeed_.x += 100.f * _deltaTime;
		}
	}
	else
	{
		playerSpeed_.x -= 600.f * _deltaTime;
	}

	if (playerSpeed_.x < 300.f)
	{
		if (directionX < 0.0f)
		{
			playerSpeed_.x *= -1.f;
			ChangeState(ePlayerState::WALK_LEFT);
		}
		else
		{
			ChangeState(ePlayerState::WALK_RIGHT);
		}
		return;
	}

	if (woodRunTime_ > 3.0f && nullptr != woodRunFinishCheckCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
	{
		ChangeState(ePlayerState::WALK_RIGHT);
		return;
	}

	SetMovement({ direction_.x * playerSpeed_.x * _deltaTime, -100.0f * _deltaTime });
}

void Player::onSpringJumpLeft(float _deltaTime)
{
	accelation_ = 0.0f;
	bGround_ = false;
	bSpin_ = false;

	if (mainRender_->IsAnimationEnd())
	{
		if (playerSpeed_.x > 0.0f)
		{
			mainRender_->ChangeAnimation("WalkRight", "sonicRight.bmp");
		}
		else
		{
			mainRender_->ChangeAnimation("WalkLeft", "sonicLeft.bmp");
		}
	}

	if (nullptr == groundCheckCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
	{
		if (gravityAccelation_.y < GRAVITY_POWER)
		{
			gravityAccelation_.y = GRAVITY_POWER;
		}
	}
	else
	{
		gravitySpeed_.x = 0.0f;
		gravitySpeed_.y = 0.0f;
		bGround_ = true;
		bCanJump_ = true;
		if (playerSpeed_.x > 1.0f)
		{
			ChangeState(ePlayerState::WALK_RIGHT);
		}
		else
		{
			ChangeState(ePlayerState::WALK_LEFT);
		}
		return;
	}


	// 움직임
	// 충돌 체크
	{
		if (RSInput::GetInstance().IsKeyPress("Left"))
		{
			if (nullptr != centerLeftCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
			{
				SetMovement({ 1.f, 0.0f });
				playerSpeed_.x = 0;
			}
			else
			{
				accelation_ += NORMAL_ACCELATION * 2;
			}
		}
		else if (RSInput::GetInstance().IsKeyPress("Right"))
		{
			if (nullptr != centerRightCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
			{
				SetMovement({ -1.f, 0.0f });
				playerSpeed_.x = 0;
			}
			else
			{
				accelation_ -= NORMAL_ACCELATION * 2;
			}
		}
	}

	// 위 충돌 체크
	if (nullptr != topCenterCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
	{
		playerSpeed_.y = 0;
		gravitySpeed_.y = 0.0f;
		gravityAccelation_.y = 0.0f;
		jumpTime_ = 0.0f;
		SetMovement({ 0.0f, 1.0f });
	}

	// 도는 행위에 진입
	if (bRollLeft_)
	{
		if (nullptr != groundCheckRollCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::ROLL_LEFT)))
		{
			rotation_ = 0.0f;
			bCanJump_ = true;
			ChangeState(ePlayerState::ROLLING_LEFT);
			bRolling_ = true;
			return;
		}
		else
		{
			bRolling_ = false;
		}
	}

	float4 nextPlayerSpeed = playerSpeed_ + (direction_ * accelation_ * _deltaTime);

	if (nextPlayerSpeed.Length() < NORMAL_SPEED_LIMIT)
	{
		playerSpeed_ = nextPlayerSpeed;
	}
	else
	{
		while (playerSpeed_.Length() > NORMAL_SPEED_LIMIT)
		{
			playerSpeed_ = playerSpeed_ * 0.8f;
		}
	}

	gravitySpeed_ += gravityAccelation_ * _deltaTime;

	actualSpeed_ = playerSpeed_ + gravitySpeed_;

	if (nullptr != centerLeftCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
	{
		SetMovement({ 1.f, 0.0f });
		playerSpeed_.x = 0;
	}
	else if (nullptr != centerRightCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
	{
		SetMovement({ -1.f, 0.0f });
		playerSpeed_.x = 0;
	}
	else
	{
		SetMovement(actualSpeed_ * _deltaTime);
	}

	if (nullptr != bottomCenterCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
	{
		SetMovement({ 0.0f, -1.0f });
		location_.y = static_cast<float>(static_cast<int>(location_.y));
	}
}

void Player::onSpringJumpRight(float _deltaTime)
{
	accelation_ = 0.0f;
	bGround_ = false;
	bSpin_ = false;

	if (mainRender_->IsAnimationEnd())
	{
		if (playerSpeed_.x > 0.0f)
		{
			mainRender_->ChangeAnimation("WalkRight", "sonicRight.bmp");
		}
		else
		{
			mainRender_->ChangeAnimation("WalkLeft", "sonicLeft.bmp");
		}
	}

	if (nullptr == groundCheckCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
	{
		if (gravityAccelation_.y < GRAVITY_POWER)
		{
			gravityAccelation_.y = GRAVITY_POWER;
		}
	}
	else
	{
		gravitySpeed_.x = 0.0f;
		gravitySpeed_.y = 0.0f;
		bGround_ = true;
		bCanJump_ = true;
		if (playerSpeed_.x > 1.0f)
		{
			ChangeState(ePlayerState::WALK_RIGHT);
		}
		else
		{
			ChangeState(ePlayerState::WALK_LEFT);
		}
		return;
	}


	// 움직임
	// 충돌 체크
	{
		if (RSInput::GetInstance().IsKeyPress("Left"))
		{
			if (nullptr != centerLeftCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
			{
				SetMovement({ 1.f, 0.0f });
				playerSpeed_.x = 0;
			}
			else
			{
				accelation_ -= NORMAL_ACCELATION * 2;
			}
		}
		else if (RSInput::GetInstance().IsKeyPress("Right"))
		{
			if (nullptr != centerRightCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
			{
				SetMovement({ -1.f, 0.0f });
				playerSpeed_.x = 0;
			}
			else
			{
				accelation_ += NORMAL_ACCELATION * 2;
			}
		}
	}

	// 위 충돌 체크
	if (nullptr != topCenterCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
	{
		playerSpeed_.y = 0;
		gravitySpeed_.y = 0.0f;
		gravityAccelation_.y = 0.0f;
		jumpTime_ = 0.0f;
		SetMovement({ 0.0f, 1.0f });
	}

	// 도는 행위에 진입
	if (bRollLeft_)
	{
		if (nullptr != groundCheckRollCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::ROLL_LEFT)))
		{
			rotation_ = 0.0f;
			bCanJump_ = true;
			ChangeState(ePlayerState::ROLLING_LEFT);
			bRolling_ = true;
			return;
		}
		else
		{
			bRolling_ = false;
		}
	}

	float4 nextPlayerSpeed = playerSpeed_ + (direction_ * accelation_ * _deltaTime);

	if (nextPlayerSpeed.Length() < NORMAL_SPEED_LIMIT)
	{
		playerSpeed_ = nextPlayerSpeed;
	}
	else
	{
		while (playerSpeed_.Length() > NORMAL_SPEED_LIMIT)
		{
			playerSpeed_ = playerSpeed_ * 0.8f;
		}
	}

	gravitySpeed_ += gravityAccelation_ * _deltaTime;

	actualSpeed_ = playerSpeed_ + gravitySpeed_;

	if (nullptr != centerLeftCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
	{
		SetMovement({ 1.f, 0.0f });
		playerSpeed_.x = 0;
	}
	else if (nullptr != centerRightCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
	{
		SetMovement({ -1.f, 0.0f });
		playerSpeed_.x = 0;
	}
	else
	{
		SetMovement(actualSpeed_ * _deltaTime);
	}

	if (nullptr != bottomCenterCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
	{
		SetMovement({ 0.0f, -1.0f });
		location_.y = static_cast<float>(static_cast<int>(location_.y));
	}
}

void Player::onDamagedLeft(float _deltaTime)
{
	mainRender_->ChangeAnimation("DamagedLeft", "sonicLeft.bmp");

	if (nullptr != groundCheckCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
	{
		bInvincible_ = true;
		invincibleTime_ = 2.0f;
		bBlink_ = true;
		blinkTime_ = 2.0f;
		bCanJump_ = true;
		bGround_ = true;
		ChangeState(ePlayerState::IDLE_LEFT);
	}

	mapCollisionSideCheck();
	rollSideCheck();
	processGravity(_deltaTime);
	normalMove(_deltaTime);
}

void Player::onDamagedRight(float _deltaTime)
{
	mainRender_->ChangeAnimation("DamagedRight", "sonicRight.bmp");

	if (nullptr != groundCheckCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
	{
		bInvincible_ = true;
		invincibleTime_ = 2.0f;
		bBlink_ = true;
		blinkTime_ = 2.0f;
		bCanJump_ = true;
		bGround_ = true;
		ChangeState(ePlayerState::IDLE_RIGHT);
	}

	mapCollisionSideCheck();
	rollSideCheck();
	processGravity(_deltaTime);
	normalMove(_deltaTime);
}

void Player::onStageClear(float _deltaTime)
{
	if (mainRender_->IsAnimationEnd())
	{
		mainRender_->ChangeAnimation("ShakeFinger", "sonicRight.bmp");
	}
}

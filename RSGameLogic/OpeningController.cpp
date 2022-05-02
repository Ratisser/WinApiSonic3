#include "OpeningController.h"

#include <RSSoundPlayer.h>

#include "Player.h"
#include "PlayLevelUI.h"
#include "SonicSeaWave.h"

OpeningController::OpeningController()
	: sonicRender_(nullptr)
	, player_(nullptr)
	, aircraftRender_(nullptr)
	, knucklesRender_(nullptr)
	, sonicPos_(float4::ZERO)
	, aircraftPos_(float4::ZERO)
	, knucklesPos_(float4::ZERO)
	, hoveringFactor_(0.0f)
	, state_(eOpeningState::APPEAR)
	, timer_(0.0f)
	, moveSpeed_({ 400.f, 0.0f })
	, seaWaveTimer_(0.0f)
{
}

OpeningController::~OpeningController()
{
	SAFE_DELETE(sonicRender_);
	SAFE_DELETE(aircraftRender_);
	SAFE_DELETE(knucklesRender_);
}

void OpeningController::Start()
{
	SetRenderOrder(99);
	SetLocation({ -4050.f, 920.f + 120.f });

	player_ = level_->FindActor<Player>("Player");

	sonicRender_ = new RSRenderer(this);
	sonicRender_->CreateAnimation("Idle", "sonicRight.bmp", 0, 0, 1.0f, true);
	sonicRender_->CreateAnimation("Jump", "sonicRight.bmp", 50, 56, 0.05f, true);
	sonicRender_->CreateAnimation("Damaged", "sonicRight.bmp", 61, 62, 0.3f, true);
	sonicRender_->CreateAnimation("SuperDash", "sonicRight.bmp", 126, 129, 0.05f, true);
	sonicRender_->ChangeAnimation("Idle", "sonicRight.bmp");

	aircraftRender_ = new RSRenderer(this);
	aircraftRender_->CreateAnimation("Idle", "aircraft.bmp", 0, 3, 0.05f, true);
	aircraftRender_->ChangeAnimation("Idle", "aircraft.bmp");

	knucklesRender_ = new RSRenderer(this);
	knucklesRender_->CreateAnimation("IdleLeft", "knucklesLeft.bmp", 0, 0, 0.1f, true);
	knucklesRender_->CreateAnimation("LaughLeft", "knucklesLeft.bmp", 1, 7, 0.2f);
	knucklesRender_->CreateAnimation("RunLeft", "knucklesLeft.bmp", 8, 11, 0.1f, true);
	knucklesRender_->CreateAnimation("JumpLeft", "knucklesLeft.bmp", 12, 19, 0.05f, true);

	knucklesRender_->CreateAnimation("RunRight", "knucklesRight.bmp", 8, 11, 0.1f, true);
	knucklesRender_->ChangeAnimation("JumpLeft", "knucklesLeft.bmp");
	knucklesRender_->Deactivate();

	sonicPos_ = { 28.f, -28.f };
	aircraftPos_ = { 0.f, 0.0f };
	knucklesPos_ = { 100.f, 50.f };

	sonicPos_.x -= 250.f;
	sonicPos_.y -= 30.f;
	aircraftPos_.x -= 250.f;
	aircraftPos_.y -= 30.f;
}

void OpeningController::UpdateBefore()
{
	sonicRender_->UpdateAnimation();
	aircraftRender_->UpdateAnimation();
	knucklesRender_->UpdateAnimation();
}

void OpeningController::Update()
{
	float deltaTime = RSTimer::GetInstance().GetDeltaTime();

	sonicRender_->SetPivot(sonicPos_);
	aircraftRender_->SetPivot(aircraftPos_);
	knucklesRender_->SetPivot(knucklesPos_);

	switch (state_)
	{
	case eOpeningState::APPEAR:
		onAppear(deltaTime);
		break;
	case eOpeningState::FLY:
		onFly(deltaTime);
		break;
	case eOpeningState::SONIC_JUMP:
		onSonicJump(deltaTime);
		break;
	case eOpeningState::SUPER_SONIC:
		onSuperSonic(deltaTime);
		break;
	case eOpeningState::FALLOW_SONIC:
		onFallowSonic(deltaTime);
		break;
	case eOpeningState::KNUCKLES_JUMP:
		onKnucklesJump(deltaTime);
		break;
	case eOpeningState::KNUCKLES_RUNLEFT:
		onKnucklesRunLeft(deltaTime);
		break;
	case eOpeningState::KNUCKLES_RUNRIGHT:
		onKnucklesRunRight(deltaTime);
		break;
	case eOpeningState::KNUCKLES_LAUGH:
		onKnucklesLaugh(deltaTime);
		break;
	case eOpeningState::KNUCKLES_RUNOUT:
		onKnucklesRunout(deltaTime);
		break;
	case eOpeningState::END_OPENING:
		onEndOpening(deltaTime);
		break;
	default:
		break;
	}
}

void OpeningController::UpdateAfter()
{
}

void OpeningController::Render()
{
	aircraftRender_->Render();
	sonicRender_->Render();
	knucklesRender_->Render();
}

void OpeningController::Cleanup()
{
}

void OpeningController::onAppear(float _deltaTime)
{
	if (false == player_->bgmPlayer_->IsPlaying())
	{
		player_->bgmPlayer_->Play();
	}
	hoveringFactor_ += TORAD(360.f) * _deltaTime;
	timer_ += _deltaTime;
	level_->SetCamPos(location_);

	SetMovement(moveSpeed_ * _deltaTime);

	if (timer_ < 1.f)
	{
		sonicPos_.x += 200 * _deltaTime;
		sonicPos_.y += 70 * _deltaTime;
		aircraftPos_.x += 200 * _deltaTime;
		aircraftPos_.y += 70 * _deltaTime;
	}
	else
	{
		timer_ = 0.0f;
		state_ = eOpeningState::FLY;
	}
}

void OpeningController::onFly(float _deltaTime)
{
	timer_ += _deltaTime;
	level_->SetCamPos(location_);
	SetMovement(moveSpeed_ * _deltaTime);

	hoveringFactor_ += TORAD(360.f) * _deltaTime;
	sonicPos_.y += cosf(hoveringFactor_) * 30.f * _deltaTime;
	aircraftPos_.y += cosf(hoveringFactor_) * 30.f * _deltaTime;

	if (timer_ >= 1.5f)
	{
		timer_ = 0.0f;
		state_ = eOpeningState::SONIC_JUMP;
	}
}

void OpeningController::onSonicJump(float _deltaTime)
{
	timer_ += _deltaTime;
	level_->SetCamPos(location_);
	SetMovement(moveSpeed_ * _deltaTime);

	hoveringFactor_ += TORAD(360.f) * _deltaTime;
	sonicPos_.y += cosf(hoveringFactor_) * 30.f * _deltaTime;
	aircraftPos_.y += cosf(hoveringFactor_) * 30.f * _deltaTime;

	sonicRender_->ChangeAnimation("Jump", "sonicRight.bmp");

	static float4 speed = { 100.f, -100.f };

	if (timer_ < 2.f)
	{
		speed.x -= 400 * _deltaTime;
		speed.y += 300 * _deltaTime;
		sonicPos_.x += speed.x * _deltaTime;
		sonicPos_.y += speed.y * _deltaTime;
	}
	else
	{
		sonicPos_.x = -200.f;
		sonicPos_.y = 90.f;
		timer_ = 0.0f;
		state_ = eOpeningState::SUPER_SONIC;
	}
}

void OpeningController::onSuperSonic(float _deltaTime)
{
	timer_ += _deltaTime;
	seaWaveTimer_ += _deltaTime;
	level_->SetCamPos(location_);
	SetMovement(moveSpeed_ * _deltaTime);

	hoveringFactor_ += TORAD(360.f) * _deltaTime;
	aircraftPos_.y += cosf(hoveringFactor_) * 30.f * _deltaTime;

	sonicRender_->ChangeAnimation("SuperDash", "sonicRight.bmp");

	if (timer_ < 2.0f)
	{
		sonicPos_.x += 200 * _deltaTime;

		if (seaWaveTimer_ > 0.1f)
		{
			SonicSeaWave* actor = level_->AddActor<SonicSeaWave>();
			actor->SetLocation(location_ + sonicPos_);
			seaWaveTimer_ = 0.0f;
		}
	}
	else
	{
		timer_ = 0.0f;
		state_ = eOpeningState::FALLOW_SONIC;
	}
}

void OpeningController::onFallowSonic(float _deltaTime)
{
	timer_ += _deltaTime;
	seaWaveTimer_ += _deltaTime;
	level_->SetCamPos(location_);
	SetMovement(moveSpeed_ * _deltaTime);

	hoveringFactor_ += TORAD(360.f) * _deltaTime;
	aircraftPos_.y += cosf(hoveringFactor_) * 30.f * _deltaTime;

	if (timer_ < 3.2f)
	{
		if (seaWaveTimer_ > 0.1f)
		{
			SonicSeaWave* actor = level_->AddActor<SonicSeaWave>();
			actor->SetLocation(location_ + sonicPos_);
			seaWaveTimer_ = 0.0f;
		}
	}

	if (timer_ < 1.0f)
	{
		sonicPos_.x -= 200 * _deltaTime;
		aircraftPos_.x -= 200 * _deltaTime;
	}
	else
	{
		moveSpeed_.x = 500.f;
	}

	if (timer_ > 7.0f)
	{
		player_->StopBGM();
	}

	if (timer_ > 8.2f)
	{
		sonicPos_.y = 54;
		knucklesPos_.y = 54;
		SetRenderOrder(101);

		player_->ChangeBGM("knuckles.mid");
		player_->StartBGM();
	}

	if (timer_ > 9.0f)
	{
		moveSpeed_.x = 0.f;
		timer_ = 0.0f;
		state_ = eOpeningState::KNUCKLES_JUMP;
	}
}

void OpeningController::onKnucklesJump(float _deltaTime)
{
	timer_ += _deltaTime;

	if (timer_ < 0.2f)
	{
		sonicPos_.x += 500.f * _deltaTime;
	}

	if (0.2f < timer_ && timer_ < 1.2f)
	{
		static float4 speed = { -172.f, -99.f };
		static float4 speed2 = { 0.0f, -200.f };
		sonicRender_->ChangeAnimation("Damaged", "sonicRight.bmp");
		knucklesRender_->Activate();

		speed.y += 200.f * _deltaTime;
		sonicPos_.x += speed.x * _deltaTime;
		sonicPos_.y += speed.y * _deltaTime;

		speed2.y += 404.f * _deltaTime;
		knucklesPos_.y += speed2.y * _deltaTime;
	}

	if (timer_ > 1.2f)
	{
		sonicRender_->ChangeAnimation("Idle", "sonicRight.bmp");
		knucklesRender_->ChangeAnimation("IdleLeft", "knucklesLeft.bmp");
	}

	if (timer_ > 2.0f)
	{
		timer_ = 0.0f;
		state_ = eOpeningState::KNUCKLES_LAUGH;
	}
}

void OpeningController::onKnucklesRunLeft(float _deltaTime)
{
}

void OpeningController::onKnucklesRunRight(float _deltaTime)
{
}

void OpeningController::onKnucklesLaugh(float _deltaTime)
{
	timer_ += _deltaTime;
	knucklesRender_->ChangeAnimation("LaughLeft", "knucklesLeft.bmp");

	if (timer_ > 1.f)
	{
		knucklesRender_->ChangeAnimation("IdleLeft", "knucklesLeft.bmp");
		timer_ = 0.0f;
		state_ = eOpeningState::KNUCKLES_RUNOUT;
	}
}

void OpeningController::onKnucklesRunout(float _deltaTime)
{
	timer_ += _deltaTime;
	knucklesRender_->ChangeAnimation("RunRight", "knucklesRight.bmp");

	if (timer_ < 2.0f)
	{
		knucklesPos_.x += 200 * _deltaTime;
	}
	else
	{
		player_->ChangeBGM("stage1-1.mid");
		player_->StartBGM();
		timer_ = 0.0f;
		state_ = eOpeningState::END_OPENING;
	}
}

void OpeningController::onEndOpening(float _deltaTime)
{
	static int a = 0;
	if (a == 0)
	{
		SetLocation({ 0.f, 0.f });
		PlayLevelUI* ui = level_->FindActor<PlayLevelUI>("PlayLevelUI");
		ui->StageStart();
		player_->ResetState();

		a = 1;
	}
}

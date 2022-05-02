#include "PlayLevelUI.h"
#include <RSSoundManager.h>
#include <RSLevelManager.h>

#include "Player.h"

PlayLevelUI::PlayLevelUI()
	: scoreRenderer_(nullptr)
	, timeRenderer_(nullptr)
	, ringRenderer_(nullptr)
	, lifeRenderer_(nullptr)
	, portraitRenderer_(nullptr)
	, number_(nullptr)
	, colon_(nullptr)
	, player_(nullptr)
	, life_(3)
	, score_(0)
	, ring_(0)
	, time_(0.0f)
	, ringBlinkTimer_(0.2f)
	, bStopTime_(true)
	, bOverHundredRing_(false)
	, state_(eUIState::WAIT)
	, animationTime_(0.0f)
	, stageStartRender0_(nullptr)
	, stageStartRender1_(nullptr)
	, stageStartRender2_(nullptr)
	, stageStartRender3_(nullptr)
	, stageClearRender0_(nullptr)
	, stageClearRender1_(nullptr)
	, stageClearRender2_(nullptr)
	, stageClearRender3_(nullptr)
	, timeBonus_(0)
	, ringBonus_(0)
	, total_(0)
	, scorePivot_({ 320.f, 0.0f })
{
}

PlayLevelUI::~PlayLevelUI()
{
	SAFE_DELETE(scoreRenderer_);
	SAFE_DELETE(timeRenderer_);
	SAFE_DELETE(ringRenderer_);
	SAFE_DELETE(lifeRenderer_);
	SAFE_DELETE(portraitRenderer_);
	SAFE_DELETE(number_);
	SAFE_DELETE(colon_);

	SAFE_DELETE(stageStartRender0_);
	SAFE_DELETE(stageStartRender1_);
	SAFE_DELETE(stageStartRender2_);
	SAFE_DELETE(stageStartRender3_);

	SAFE_DELETE(stageClearRender0_);
	SAFE_DELETE(stageClearRender1_);
	SAFE_DELETE(stageClearRender2_);
	SAFE_DELETE(stageClearRender3_);
}

void PlayLevelUI::Start()
{
	SetRenderOrder(10000);
	player_ = level_->FindActor<Player>("Player");

	scoreRenderer_ = new RSRenderer(this, false);
	scoreRenderer_->CreateAnimation("Score", "ui.bmp", 11, 11, 1.0f);
	scoreRenderer_->ChangeAnimation("Score", "ui.bmp", true);
	scoreRenderer_->SetPivot({ 40.0f, 20.0f });
	scoreRenderer_->UpdateAnimation();

	timeRenderer_ = new RSRenderer(this, false);
	timeRenderer_->CreateAnimation("Time", "ui.bmp", 12, 12, 1.0f);
	timeRenderer_->ChangeAnimation("Time", "ui.bmp", true);
	timeRenderer_->SetPivot({ 35.0f, 40.0f });

	ringRenderer_ = new RSRenderer(this, false);
	ringRenderer_->CreateAnimation("Ring", "ui.bmp", 13, 13, 1.0f);
	ringRenderer_->ChangeAnimation("Ring", "ui.bmp", true);
	ringRenderer_->SetPivot({ 40.0f, 60.0f });

	lifeRenderer_ = new RSRenderer(this, false);
	lifeRenderer_->CreateAnimation("Life", "ui.bmp", 14, 14, 1.0f);
	lifeRenderer_->ChangeAnimation("Life", "ui.bmp", true);
	lifeRenderer_->SetPivot({ 50, 220.f });
	lifeRenderer_->UpdateAnimation();

	portraitRenderer_ = new RSRenderer(this, false);
	portraitRenderer_->CreateAnimation("SonicFace", "monitors.bmp", 17, 17, 1.0f);
	portraitRenderer_->ChangeAnimation("SonicFace", "monitors.bmp", true);
	portraitRenderer_->SetPivot({ 26, 220.f });
	portraitRenderer_->UpdateAnimation();

	number_ = new RSRenderer(this, false);
	number_->CreateAnimation("0", "ui.bmp", 0, 0, 1.0f);
	number_->CreateAnimation("1", "ui.bmp", 1, 1, 1.0f);
	number_->CreateAnimation("2", "ui.bmp", 2, 2, 1.0f);
	number_->CreateAnimation("3", "ui.bmp", 3, 3, 1.0f);
	number_->CreateAnimation("4", "ui.bmp", 4, 4, 1.0f);
	number_->CreateAnimation("5", "ui.bmp", 5, 5, 1.0f);
	number_->CreateAnimation("6", "ui.bmp", 6, 6, 1.0f);
	number_->CreateAnimation("7", "ui.bmp", 7, 7, 1.0f);
	number_->CreateAnimation("8", "ui.bmp", 8, 8, 1.0f);
	number_->CreateAnimation("9", "ui.bmp", 9, 9, 1.0f);

	number_->CreateAnimation("s0", "ui.bmp", 17, 17, 1.0f);
	number_->CreateAnimation("s1", "ui.bmp", 18, 18, 1.0f);
	number_->CreateAnimation("s2", "ui.bmp", 19, 19, 1.0f);
	number_->CreateAnimation("s3", "ui.bmp", 20, 20, 1.0f);
	number_->CreateAnimation("s4", "ui.bmp", 21, 21, 1.0f);
	number_->CreateAnimation("s5", "ui.bmp", 22, 22, 1.0f);
	number_->CreateAnimation("s6", "ui.bmp", 23, 23, 1.0f);
	number_->CreateAnimation("s7", "ui.bmp", 24, 24, 1.0f);
	number_->CreateAnimation("s8", "ui.bmp", 25, 25, 1.0f);
	number_->CreateAnimation("s9", "ui.bmp", 26, 26, 1.0f);
	number_->ChangeAnimation("0", "ui.bmp", true);

	colon_ = new RSRenderer(this, false);
	colon_->CreateAnimation("colon", "ui.bmp", 10, 10, 1.0f);
	colon_->ChangeAnimation("colon", "ui.bmp");
	colon_->UpdateAnimation();
	colon_->SetPivot({ 75.f, 42.0f });


	stageStartRender0_ = new RSRenderer(this, false);
	stageStartRender0_->CreateAnimation("Sonic3", "stage1-1_title.bmp", 0, 0, 1.0f, false);
	stageStartRender0_->ChangeAnimation("Sonic3", "stage1-1_title.bmp");
	stageStartRender0_->SetPivot({ 0.0f, -240.f });
	stageStartRender0_->SetImagePivot({ 160.f, 120.f });

	stageStartRender1_ = new RSRenderer(this, false);
	stageStartRender1_->CreateAnimation("Sonic3", "stage1-1_title.bmp", 1, 1, 1.0f, false);
	stageStartRender1_->ChangeAnimation("Sonic3", "stage1-1_title.bmp");
	stageStartRender1_->SetPivot({ 320.f, 0.0f });
	stageStartRender1_->SetImagePivot({ 160.f, 120.f });

	stageStartRender2_ = new RSRenderer(this, false);
	stageStartRender2_->CreateAnimation("Sonic3", "stage1-1_title.bmp", 2, 2, 1.0f, false);
	stageStartRender2_->ChangeAnimation("Sonic3", "stage1-1_title.bmp");
	stageStartRender2_->SetPivot({ 320.f, 0.0f });
	stageStartRender2_->SetImagePivot({ 160.f, 120.f });

	stageStartRender3_ = new RSRenderer(this, false);
	stageStartRender3_->CreateAnimation("Sonic3", "stage1-1_title.bmp", 3, 3, 1.0f, false);
	stageStartRender3_->ChangeAnimation("Sonic3", "stage1-1_title.bmp");
	stageStartRender3_->SetPivot({ 320.f, 0.0f });
	stageStartRender3_->SetImagePivot({ 160.f, 120.f });


	stageClearRender0_ = new RSRenderer(this, false);
	stageClearRender0_->CreateAnimation("Sonic3", "stage1-1_end.bmp", 0, 0, 1.0f, false);
	stageClearRender0_->ChangeAnimation("Sonic3", "stage1-1_end.bmp");
	stageClearRender0_->SetPivot({ 320.0f, 0.f });
	stageClearRender0_->SetImagePivot({ 140.f, 120.f });

	stageClearRender1_ = new RSRenderer(this, false);
	stageClearRender1_->CreateAnimation("Sonic3", "stage1-1_end.bmp", 1, 1, 1.0f, false);
	stageClearRender1_->ChangeAnimation("Sonic3", "stage1-1_end.bmp");
	stageClearRender1_->SetPivot({ 320.f, 0.0f });
	stageClearRender1_->SetImagePivot({ 160.f, 120.f });

	stageClearRender2_ = new RSRenderer(this, false);
	stageClearRender2_->CreateAnimation("Sonic3", "stage1-1_end.bmp", 2, 2, 1.0f, false);
	stageClearRender2_->ChangeAnimation("Sonic3", "stage1-1_end.bmp");
	stageClearRender2_->SetPivot({ 320.f, 0.0f });
	stageClearRender2_->SetImagePivot({ 160.f, 120.f });

	stageClearRender3_ = new RSRenderer(this, false);
	stageClearRender3_->CreateAnimation("Sonic3", "stage1-1_end.bmp", 3, 3, 1.0f, false);
	stageClearRender3_->ChangeAnimation("Sonic3", "stage1-1_end.bmp");
	stageClearRender3_->SetPivot({ 320.f, 0.0f });
	stageClearRender3_->SetImagePivot({ 160.f, 120.f });

	scoreRenderer_->Deactivate();
	timeRenderer_->Deactivate();
	ringRenderer_->Deactivate();
	lifeRenderer_->Deactivate();
	portraitRenderer_->Deactivate();
	number_->Deactivate();
	colon_->Deactivate();
}

void PlayLevelUI::UpdateBefore()
{
	timeRenderer_->UpdateAnimation();
	ringRenderer_->UpdateAnimation();

	stageStartRender0_->UpdateAnimation();
	stageStartRender1_->UpdateAnimation();
	stageStartRender2_->UpdateAnimation();
	stageStartRender3_->UpdateAnimation();

	stageClearRender0_->UpdateAnimation();
	stageClearRender1_->UpdateAnimation();
	stageClearRender2_->UpdateAnimation();
	stageClearRender3_->UpdateAnimation();
}

void PlayLevelUI::Update()
{
	float deltaTime = RSTimer::GetInstance().GetDeltaTime();

	switch (state_)
	{
	case eUIState::WAIT:
		return;
		break;
	case eUIState::PLAY:
		break;
	case eUIState::STAGE_START:
		onStageStart(deltaTime);
		break;
	case eUIState::START_END:
		onStartEnd(deltaTime);
		break;
	case eUIState::STAGE_CLEAR:
		onStageClear(deltaTime);
		break;
	case eUIState::CLEAR_COUNT:
		onClearCount(deltaTime);
		break;
	case eUIState::COUNT_END:
		onCountEnd(deltaTime);
		break;
	default:
		break;
	}

	if (bStopTime_)
	{
		time_ += deltaTime;
	}

	if (ring_ == 0)
	{
		ringBlinkTimer_ -= deltaTime;
		if (ringBlinkTimer_ < 0.f)
		{
			ringBlinkTimer_ = 0.2f;
			if (ringRenderer_->IsActivate())
			{
				ringRenderer_->Deactivate();
			}
			else
			{
				ringRenderer_->Activate();
			}
		}
	}
	else
	{
		ringRenderer_->Activate();
	}
}

void PlayLevelUI::UpdateAfter()
{
}

void PlayLevelUI::Render()
{
	scoreRenderer_->Render();
	timeRenderer_->Render();
	ringRenderer_->Render();
	lifeRenderer_->Render();
	colon_->Render();

	// Print number
	{
		int digit = 1;
		float xpos = 100.f;
		float ypos = 20.f;
		int num = 0;
		int score = score_;
		do
		{
			num = (score % (digit * 10)) / digit;
			number_->ChangeAnimation(std::to_string(num), "ui.bmp", true);
			number_->UpdateAnimation();
			number_->SetPivot({ xpos, ypos });
			number_->Render();
			score -= num * digit;
			digit *= 10;
			xpos -= 8.f;
		} while (score / digit != 0);

		int min = static_cast<int>(time_) / 60;
		int sec = static_cast<int>(time_) % 60;

		xpos = 91.f;
		ypos = 41.f;
		digit = 1;
		num = 0;

		for (int i = 0; i < 2; i++)
		{
			num = (sec % (digit * 10)) / digit;
			number_->ChangeAnimation(std::to_string(num), "ui.bmp", true);
			number_->UpdateAnimation();
			number_->SetPivot({ xpos, ypos });
			number_->Render();
			sec -= num * digit;
			digit *= 10;
			xpos -= 8.f;
		}

		xpos = 67.f;
		digit = 1;
		num = 0;

		do
		{
			num = (min % (digit * 10)) / digit;
			number_->ChangeAnimation(std::to_string(num), "ui.bmp", true);
			number_->UpdateAnimation();
			number_->SetPivot({ xpos, ypos });
			number_->Render();
			min -= num * digit;
			digit *= 10;
			xpos -= 8.f;
		} while (min / digit != 0);

		xpos = 91.f;
		ypos = 61.f;
		digit = 1;
		num = 0;
		int ring = ring_;
		do
		{
			num = (ring % (digit * 10)) / digit;
			number_->ChangeAnimation(std::to_string(num), "ui.bmp", true);
			number_->UpdateAnimation();
			number_->SetPivot({ xpos, ypos });
			number_->Render();
			ring -= num * digit;
			digit *= 10;
			xpos -= 8.f;
		} while (ring / digit != 0);

		xpos = 63.f;
		ypos = 225.f;
		digit = 1;
		num = 0;
		int life = life_;
		std::string preFix = "s";
		do
		{
			num = (life % (digit * 10)) / digit;
			number_->ChangeAnimation(preFix + std::to_string(num), "ui.bmp", true);
			number_->UpdateAnimation();
			number_->SetPivot({ xpos, ypos });
			number_->Render();
			life -= num * digit;
			digit *= 10;
			xpos -= 8.f;
		} while (life / digit != 0);


		xpos = scorePivot_.x + 251.f;
		ypos = 126.f;
		digit = 1;
		num = 0;
		int timeBonus = timeBonus_;
		do
		{
			num = (timeBonus % (digit * 10)) / digit;
			number_->ChangeAnimation(std::to_string(num), "ui.bmp", true);
			number_->UpdateAnimation();
			number_->SetPivot({ xpos, ypos });
			number_->Render();
			timeBonus -= num * digit;
			digit *= 10;
			xpos -= 8.f;
		} while (timeBonus / digit != 0);

		xpos = scorePivot_.x + 251.f;
		ypos = 142.f;
		digit = 1;
		num = 0;
		int ringBonus = ringBonus_;
		do
		{
			num = (ringBonus % (digit * 10)) / digit;
			number_->ChangeAnimation(std::to_string(num), "ui.bmp", true);
			number_->UpdateAnimation();
			number_->SetPivot({ xpos, ypos });
			number_->Render();
			ringBonus -= num * digit;
			digit *= 10;
			xpos -= 8.f;
		} while (ringBonus / digit != 0);

		xpos = scorePivot_.x + 251.f;
		ypos = 170.f;
		digit = 1;
		num = 0;
		int total = total_;
		do
		{
			num = (total % (digit * 10)) / digit;
			number_->ChangeAnimation(std::to_string(num), "ui.bmp", true);
			number_->UpdateAnimation();
			number_->SetPivot({ xpos, ypos });
			number_->Render();
			total -= num * digit;
			digit *= 10;
			xpos -= 8.f;
		} while (total / digit != 0);

	}

	portraitRenderer_->Render();

	stageStartRender0_->Render();
	stageStartRender1_->Render();
	stageStartRender2_->Render();
	stageStartRender3_->Render();
	stageClearRender0_->Render();
	stageClearRender1_->Render();
	stageClearRender2_->Render();
	stageClearRender3_->Render();
}

void PlayLevelUI::Cleanup()
{
}

float PlayLevelUI::GetTime() const
{
	return time_;
}

void PlayLevelUI::StopTime()
{
	bStopTime_ = false;
}

void PlayLevelUI::StartTime()
{
	bStopTime_ = true;
}

int PlayLevelUI::GetRingCount() const
{
	return ring_;
}

void PlayLevelUI::ResetTime()
{
	time_ = 0.0f;
}

void PlayLevelUI::SubstractOneSecond()
{
	time_ -= 1.0f;
}

void PlayLevelUI::AddRing()
{
	++ring_;
	score_ += 100;

	if (!bOverHundredRing_ && ring_ >= 100)
	{
		bOverHundredRing_ = true;
		RSSoundManager::GetInstance().PlaySoundByName("life.mid");
	}
}

void PlayLevelUI::AddRing(int _ringCount)
{
	ring_ += _ringCount;
	score_ += _ringCount * 100;

	if (!bOverHundredRing_ && ring_ >= 100)
	{
		bOverHundredRing_ = true;
		RSSoundManager::GetInstance().PlaySoundByName("life.mid");
	}
}

void PlayLevelUI::SubtractRing()
{
	--ring_;
}

void PlayLevelUI::SetRing(int _ringCount)
{
	ring_ = _ringCount;
}

void PlayLevelUI::AddLife()
{
	++life_;
}

void PlayLevelUI::SubtractLife()
{
	--life_;
}

void PlayLevelUI::StageStart()
{
	state_ = eUIState::STAGE_START;
	StartTime();
	scoreRenderer_->Activate();
	timeRenderer_->Activate();
	ringRenderer_->Activate();
	lifeRenderer_->Activate();
	portraitRenderer_->Activate();
	number_->Activate();
	colon_->Activate();
}

void PlayLevelUI::StageClear()
{
	Player* player = level_->FindActor<Player>("Player");
	player->StopBGM();
	RSSoundManager::GetInstance().PlaySoundByName("stageClear.mid");
	state_ = eUIState::STAGE_CLEAR;
	timeBonus_ = 100 * static_cast<int>((600.f - time_) / 3.f);
	ringBonus_ = 100 * ring_;
	total_ = 0;
}

void PlayLevelUI::onStageStart(float _deltaTime)
{
	Player* player = level_->FindActor<Player>("Player");
	player->StartBGM();
	animationTime_ += _deltaTime;

	float4 pivot = stageStartRender0_->GetPivot();

	if (pivot.y < 0)
	{
		pivot.y += 640.f * _deltaTime;
		stageStartRender0_->SetPivot(pivot);
	}

	pivot = stageStartRender1_->GetPivot();
	if (animationTime_ > 0.2f)
	{
		if (pivot.x > 0)
		{
			pivot.x -= 640.f * _deltaTime;
			stageStartRender1_->SetPivot(pivot);
		}
	}

	pivot = stageStartRender2_->GetPivot();
	if (animationTime_ > 0.4f)
	{
		if (pivot.x > 0)
		{
			pivot.x -= 640.f * _deltaTime;
			stageStartRender2_->SetPivot(pivot);
		}
	}

	pivot = stageStartRender3_->GetPivot();
	if (animationTime_ > 0.6f)
	{
		if (pivot.x > 0)
		{
			pivot.x -= 640.f * _deltaTime;
			stageStartRender3_->SetPivot(pivot);
		}
	}

	if (animationTime_ > 2.0f)
	{
		animationTime_ = 0.0f;
		player->ResetState();
		state_ = eUIState::START_END;
	}
}

void PlayLevelUI::onStartEnd(float _deltaTime)
{
	animationTime_ += _deltaTime;

	float4 pivot = float4::ZERO;
	if (animationTime_ > 2.0f)
	{
		pivot = stageStartRender0_->GetPivot();
		if (pivot.y > -240.f)
		{
			pivot.y -= 640.f * 2 * _deltaTime;
			stageStartRender0_->SetPivot(pivot);
		}
	}

	if (animationTime_ > 2.1f)
	{
		pivot = stageStartRender1_->GetPivot();
		if (pivot.x < 320.f)
		{
			pivot.x += 640.f * 2 * _deltaTime;
			stageStartRender1_->SetPivot(pivot);
		}
	}

	if (animationTime_ > 2.2f)
	{
		pivot = stageStartRender2_->GetPivot();
		if (pivot.x < 320.f)
		{
			pivot.x += 640.f * 2 * _deltaTime;
			stageStartRender2_->SetPivot(pivot);
		}
	}

	if (animationTime_ > 2.3f)
	{
		pivot = stageStartRender3_->GetPivot();
		if (pivot.x < 320.f)
		{
			pivot.x += 640.f * 2 * _deltaTime;
			stageStartRender3_->SetPivot(pivot);
		}
	}

	if (animationTime_ > 2.5f)
	{
		animationTime_ = 0.0f;
		StartTime();


		state_ = eUIState::PLAY;
	}
}

void PlayLevelUI::onStageClear(float _deltaTime)
{
	animationTime_ += _deltaTime;

	float4 pivot = stageClearRender0_->GetPivot();

	if (pivot.x > 0)
	{
		pivot.x -= 640.f * _deltaTime;
		stageClearRender0_->SetPivot(pivot);
	}

	pivot = stageClearRender1_->GetPivot();
	if (animationTime_ > 0.2f)
	{
		if (pivot.x > 0)
		{
			pivot.x -= 640.f * _deltaTime;
			stageClearRender1_->SetPivot(pivot);
		}
	}

	pivot = stageClearRender2_->GetPivot();
	if (animationTime_ > 0.4f)
	{
		if (pivot.x > 0)
		{
			pivot.x -= 640.f * _deltaTime;
			stageClearRender2_->SetPivot(pivot);
		}
	}

	pivot = stageClearRender3_->GetPivot();
	if (animationTime_ > 0.6f)
	{
		if (pivot.x > 0)
		{
			scorePivot_.x -= 640.f * _deltaTime;
			pivot.x -= 640.f * _deltaTime;
			stageClearRender3_->SetPivot(pivot);
		}
	}

	if (animationTime_ > 5.0f)
	{
		animationTime_ = 0.0f;

		state_ = eUIState::CLEAR_COUNT;
	}
}

void PlayLevelUI::onClearCount(float _deltaTime)
{
	static float countTime = 0;
	countTime += _deltaTime;
	animationTime_ += _deltaTime;

	if (animationTime_ > 0.05f)
	{
		RSSoundManager::GetInstance().PlaySoundByName("click.wav");
		animationTime_ = 0.0f;
	}

	if (countTime > 0.0167f)
	{
		if (timeBonus_ > 0)
		{
			timeBonus_ -= 100;
			score_ += 100;
			total_ += 100;
		}
		else if (ringBonus_ > 0)
		{
			ringBonus_ -= 100;
			score_ += 100;
			total_ += 100;
		}
		else
		{
			RSSoundManager::GetInstance().PlaySoundByName("cashout.wav");
			state_ = eUIState::COUNT_END;
			animationTime_ = 0;
		}
		countTime = 0;
	}
}

void PlayLevelUI::onCountEnd(float _deltaTime)
{
	animationTime_ += _deltaTime;

	float4 pivot = float4::ZERO;
	if (animationTime_ > 3.0f)
	{
		pivot = stageClearRender0_->GetPivot();
		if (pivot.x < 320.f)
		{
			pivot.x += 640.f * 2 * _deltaTime;
			stageClearRender0_->SetPivot(pivot);
		}
	}

	if (animationTime_ > 3.1f)
	{
		pivot = stageClearRender1_->GetPivot();
		if (pivot.x < 320.f)
		{
			scorePivot_.x += 640.f * _deltaTime;
			pivot.x += 640.f * 2 * _deltaTime;
			stageClearRender1_->SetPivot(pivot);
		}
	}

	if (animationTime_ > 3.2f)
	{
		pivot = stageClearRender2_->GetPivot();
		if (pivot.x < 320.f)
		{
			pivot.x += 640.f * 2 * _deltaTime;
			stageClearRender2_->SetPivot(pivot);
		}
	}

	if (animationTime_ > 3.3f)
	{
		pivot = stageClearRender3_->GetPivot();
		if (pivot.x < 320.f)
		{
			pivot.x += 640.f * 2 * _deltaTime;
			stageClearRender3_->SetPivot(pivot);
		}
	}

	if (animationTime_ > 5.f)
	{
		animationTime_ = 0.0f;
		ResetTime();
		StartTime();
		ring_ = 0;

		Player* player = level_->FindActor<Player>("Player");
		player->ExitBossArea();
		player->ResetState();
		RSLevelManager::GetInstance().ChangeLevel("EndingLevel");
		RSSoundManager::GetInstance().PlaySoundByName("ending2.mid");
		state_ = eUIState::PLAY;
	}
}

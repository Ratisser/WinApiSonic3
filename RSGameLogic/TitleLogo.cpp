#include "TitleLogo.h"

#include <RSInput.h>
#include <RSLevelManager.h>
#include <RSRenderer.h>
#include <RSSoundPlayer.h>


TitleLogo::TitleLogo()
	: title_(nullptr)
	, titleSonic_(nullptr)
	, titleAircraft_(nullptr)
	, titleLabel_(nullptr)
	, titleUI_(nullptr)
	, titleBlinder_(nullptr)
	, fTitleLabelAnimTime_(-270.0f)
	, fTitleLabelAnimYFactor_(120.0f)
	, aircraftDirection_(float4::RIGHT)
	, eCurrentState_(eTitleState::SEGA_LOGO)
	, eCurrentMenu_(eTitleMenu::PLAY)
	, bgmPlayer_(nullptr)
{
	SetLocation({ 160.f, 120.f });
}

TitleLogo::~TitleLogo()
{
	if (nullptr != title_)
	{
		delete title_;
		title_ = nullptr;
	}

	if (nullptr != titleSonic_)
	{
		delete titleSonic_;
		titleSonic_ = nullptr;
	}

	if (nullptr != titleAircraft_)
	{
		delete titleAircraft_;
		titleAircraft_ = nullptr;
	}

	if (nullptr != titleLabel_)
	{
		delete titleLabel_;
		titleLabel_ = nullptr;
	}

	if (nullptr != titleUI_)
	{
		delete titleUI_;
		titleUI_ = nullptr;
	}

	if (nullptr != titleBlinder_)
	{
		delete titleBlinder_;
		titleBlinder_ = nullptr;
	}

	if (nullptr != bgmPlayer_)
	{
		delete bgmPlayer_;
		bgmPlayer_ = nullptr;
	}
}

void TitleLogo::UpdateBefore()
{
}

void TitleLogo::Update()
{
	if (true == RSInput::GetInstance().IsKeyDown("ToPlayStage"))
	{
		RSLevelManager::GetInstance().ChangeLevel("PlayLevel");
	}
	if (true == RSInput::GetInstance().IsKeyDown("ToEditStage"))
	{
		RSLevelManager::GetInstance().ChangeLevel("EditLevel");
	}

	switch (eCurrentState_)
	{
	case TitleLogo::eTitleState::SEGA_LOGO:
		segaLogo();
		break;
	case TitleLogo::eTitleState::SONIC_APPEAR:
		sonicAppear();
		break;
	case TitleLogo::eTitleState::LABEL_APPEAR:
		labelAppear();
		break;
	case TitleLogo::eTitleState::SELECT_MENU:
		selectMenu();
		break;
	default:
		break;
	}

	title_->UpdateAnimation();
	titleSonic_->UpdateAnimation();
	titleAircraft_->UpdateAnimation();
	titleLabel_->UpdateAnimation();
	titleUI_->UpdateAnimation();
}

void TitleLogo::UpdateAfter()
{
}

void TitleLogo::Render()
{
	// DO NOT CHANGE ORDER
	title_->Render();
	titleAircraft_->Render();
	titleSonic_->Render();
	titleLabel_->Render();
	titleUI_->Render();
	titleBlinder_->Render();
}

void TitleLogo::segaLogo()
{
	if (title_->IsAnimationEnd())
	{
		bgmPlayer_->Play();
		bgmPlayer_->SetVolume(0.5f);
		title_->ChangeAnimation("SonicApper", "title.bmp");
		eCurrentState_ = eTitleState::SONIC_APPEAR;
	}
}

void TitleLogo::sonicAppear()
{
	if (title_->IsAnimationEnd())
	{
		title_->ChangeAnimation("TitleBackground", "title.bmp");
		titleSonic_->Activate();
		titleUI_->Activate();
		titleLabel_->Activate();
		eCurrentState_ = eTitleState::LABEL_APPEAR;
	}
}

void TitleLogo::labelAppear()
{

	float deltaTime = RSTimer::GetInstance().GetDeltaTime();
	float4 curPivot = titleSonic_->GetPivot();
	if (curPivot.y > -16.f)
	{
		curPivot.y -= 120.f * deltaTime;
		titleSonic_->SetPivot(curPivot);
	}

	if (fTitleLabelAnimTime_ < 630.0f)
	{
		fTitleLabelAnimTime_ += 360.f * deltaTime;

		float temp = sinf(TORAD(fTitleLabelAnimTime_)) * fTitleLabelAnimYFactor_;

		if (fTitleLabelAnimYFactor_ > 0.0f)
		{
			fTitleLabelAnimYFactor_ -= 60.f * deltaTime;
		}

		titleLabel_->SetPivot({ 0.0f, temp });
	}
	else
	{
		eCurrentState_ = eTitleState::SELECT_MENU;
		titleAircraft_->SetPivot({ -250.f, -40.f });
		titleAircraft_->Activate();
	}

}

void TitleLogo::selectMenu()
{
	float deltaTime = RSTimer::GetInstance().GetDeltaTime();
	float4 aircraftPivot = titleAircraft_->GetPivot();
	float aircraftSpeed = 60.f;
	float4 temp = aircraftDirection_ * aircraftSpeed * deltaTime;
	aircraftPivot += temp;
	titleAircraft_->SetPivot(aircraftPivot);

	if (aircraftPivot.x > 250.f)
	{
		aircraftDirection_ = float4::LEFT;
		titleAircraft_->ChangeAnimation("AircraftFlyLeft", "titleAircraftLeft.bmp");
	}
	else if (aircraftPivot.x < -250.f)
	{
		aircraftDirection_ = float4::RIGHT;
		titleAircraft_->ChangeAnimation("AircraftFlyRight", "titleAircraftRight.bmp");
	}

	if (RSInput::GetInstance().IsKeyDown("Down"))
	{
		if (eTitleMenu::PLAY == eCurrentMenu_)
		{
			eCurrentMenu_ = eTitleMenu::COMPETITION;
			titleUI_->ChangeAnimation("TitleUICompetition", "titleSonic.bmp");
		}
		else
		{
			eCurrentMenu_ = eTitleMenu::PLAY;
			titleUI_->ChangeAnimation("TitleUIPlay", "titleSonic.bmp");
		}
	}
	else if (RSInput::GetInstance().IsKeyDown("Up"))
	{
		if (eTitleMenu::COMPETITION == eCurrentMenu_)
		{
			eCurrentMenu_ = eTitleMenu::PLAY;
			titleUI_->ChangeAnimation("TitleUIPlay", "titleSonic.bmp");
		}
		else
		{
			eCurrentMenu_ = eTitleMenu::COMPETITION;
			titleUI_->ChangeAnimation("TitleUICompetition", "titleSonic.bmp");
		}
	}
	else if (RSInput::GetInstance().IsKeyDown("Return"))
	{
		if (eTitleMenu::PLAY == eCurrentMenu_)
		{
			RSLevelManager::GetInstance().ChangeLevel("PlayLevel");
		}
		else if (eTitleMenu::COMPETITION == eCurrentMenu_)
		{
			RSLevelManager::GetInstance().ChangeLevel("EditLevel");
		}
	}
}

void TitleLogo::Cleanup()
{
	bgmPlayer_->Stop();
	eCurrentState_ = TitleLogo::eTitleState::SEGA_LOGO;
	eCurrentMenu_ = TitleLogo::eTitleMenu::PLAY;
	fTitleLabelAnimTime_ = -270.0f;
	fTitleLabelAnimYFactor_ = 120.0f;
	title_->Activate();
	titleSonic_->Deactivate();
	titleAircraft_->Deactivate();
	titleLabel_->Deactivate();
	titleUI_->Deactivate();

	title_->ChangeAnimation("ShowLogo", "title.bmp");
	titleSonic_->ChangeAnimation("TitleSonic", "titleSonic.bmp");
	titleSonic_->SetImagePivot({ 4.0f, 0.0f });
	titleAircraft_->ChangeAnimation("AircraftFlyRight", "titleAircraftRight.bmp");
	titleLabel_->ChangeAnimation("TitleLabel", "titleSonic.bmp");
	titleLabel_->SetImagePivot({ 3.0f, -3.0f });
	titleUI_->ChangeAnimation("TitleUIPlay", "titleSonic.bmp", true);
	titleUI_->SetPivot({ 0.0f, -5.0f });
}

void TitleLogo::Start()
{
	if (false == RSInput::GetInstance().IsKeyAvailable("Up"))
	{
		RSInput::GetInstance().CreateKey("Up", VK_UP);
	}
	if (false == RSInput::GetInstance().IsKeyAvailable("Down"))
	{
		RSInput::GetInstance().CreateKey("Down", VK_DOWN);
	}
	if (false == RSInput::GetInstance().IsKeyAvailable("Return"))
	{
		RSInput::GetInstance().CreateKey("Return", VK_RETURN);
	}
	if (false == RSInput::GetInstance().IsKeyAvailable("ToPlayStage"))
	{
		RSInput::GetInstance().CreateKey("ToPlayStage", '1');
	}
	if (false == RSInput::GetInstance().IsKeyAvailable("ToEditStage"))
	{
		RSInput::GetInstance().CreateKey("ToEditStage", '2');
	}

	if (nullptr == title_)
	{
		title_ = new RSRenderer(this, false);
		title_->SetImage("title.bmp");
		title_->CreateAnimation("ShowLogo", "title.bmp", 0, 0, 3.0f, false);
		title_->CreateAnimation("SonicApper", "title.bmp", 0, 17, 0.05f, false);
		title_->CreateAnimation("TitleBackground", "title.bmp", 18, 25, 0.5f, true);
		title_->ChangeAnimation("ShowLogo", "title.bmp");
	}

	if (nullptr == titleSonic_)
	{
		titleSonic_ = new RSRenderer(this, false);
		titleSonic_->Deactivate();
		titleSonic_->SetImage("titleSonic.bmp");
		titleSonic_->CreateAnimation("TitleSonic", "titleSonic.bmp", 0, 21, 0.1f, true);
		titleSonic_->ChangeAnimation("TitleSonic", "titleSonic.bmp");
		titleSonic_->SetImagePivot({ 4.0f, 0.0f });
	}

	if (nullptr == titleAircraft_)
	{
		titleAircraft_ = new RSRenderer(this, false);
		titleAircraft_->Deactivate();
		titleAircraft_->SetImage("titleAircraftRight.bmp");
		titleAircraft_->CreateAnimation("AircraftFlyRight", "titleAircraftRight.bmp", 0, 5, 0.05f, true);
		titleAircraft_->CreateAnimation("AircraftFlyLeft", "titleAircraftLeft.bmp", 0, 5, 0.05f, true);
		titleAircraft_->ChangeAnimation("AircraftFlyRight", "titleAircraftRight.bmp");
	}

	if (nullptr == titleLabel_)
	{
		titleLabel_ = new RSRenderer(this, false);
		titleLabel_->Deactivate();
		titleLabel_->SetImage("titleSonic.bmp");
		titleLabel_->CreateAnimation("TitleLabel", "titleSonic.bmp", 22, 22, 20.f);
		titleLabel_->ChangeAnimation("TitleLabel", "titleSonic.bmp");
		titleLabel_->SetImagePivot({ 3.0f, -3.0f });
	}

	if (nullptr == titleUI_)
	{
		titleUI_ = new RSRenderer(this, false);
		titleUI_->Deactivate();
		titleUI_->SetImage("titleSonic.bmp");
		titleUI_->CreateAnimation("TitleUIPlay", "titleSonic.bmp", 23, 23, 20.f);
		titleUI_->CreateAnimation("TitleUICompetition", "titleSonic.bmp", 24, 24, 20.f);
		titleUI_->ChangeAnimation("TitleUIPlay", "titleSonic.bmp", true);
		titleUI_->SetPivot({ 0.0f, -5.0f });
	}

	if (nullptr == titleBlinder_)
	{
		titleBlinder_ = new RSRenderer(this, false);
		titleBlinder_->SetImage("titleBlinder.bmp");
	}

	bgmPlayer_ = new RSSoundPlayer("title.mid");
}

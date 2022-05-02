#include "PlayerSkill.h"
#include "Player.h"
#include "PlayerSkillBackface.h"

PlayerSkill::PlayerSkill()
	: bubbleRender_(nullptr)
	, electricRender_(nullptr)
	, bubbleSkillRender_(nullptr)
	, fireRenderForward_(nullptr)
	, fireSkillRender_(nullptr)
	, player_(nullptr)
	, electricAnimCount_(0)
	, fireAnimCount_(0)
	, blinkTimer_(0.025f)
	, bBlink_(false)
	, backface_(nullptr)
	, bFireSkillCasting_(false)
	, bElectricSkillCasting_(false)
	, bBubbleSkillCasting_(false)
{
}

PlayerSkill::~PlayerSkill()
{
	SAFE_DELETE(bubbleRender_);
	SAFE_DELETE(electricRender_);

	SAFE_DELETE(bubbleSkillRender_);
	SAFE_DELETE(fireRenderForward_);

	SAFE_DELETE(fireSkillRender_);
}

void PlayerSkill::Start()
{
	SetRenderOrder(1001);
	bubbleRender_ = new RSRenderer(this);
	bubbleRender_->CreateAnimation("Shield", "bubble2.bmp", 0, 9, 0.1f, true);
	bubbleRender_->ChangeAnimation("Shield", "bubble2.bmp");
	bubbleSkillRender_ = new RSRenderer(this);
	bubbleSkillRender_->CreateAnimation("Idle", "bubble.bmp", 0, 1, 0.3f, true);
	bubbleSkillRender_->CreateAnimation("On", "bubble.bmp", 2, 3, 0.01f, false);
	bubbleSkillRender_->ChangeAnimation("Idle", "bubble.bmp");
	bubbleSkillRender_->SetPivot({ 0.0f, 24.f });

	electricRender_ = new RSRenderer(this);
	electricRender_->CreateAnimation("ShieldFlash", "electric.bmp", 0, 2, 0.02f);
	electricRender_->CreateAnimation("ShieldForward", "electric.bmp", 3, 11, 0.08f);
	electricRender_->CreateAnimation("ShieldBackward", "electric.bmp", 12, 20, 0.08f);
	electricRender_->ChangeAnimation("ShieldFlash", "electric.bmp");

	fireRenderForward_ = new RSRenderer(this);
	fireRenderForward_->CreateAnimation("ShieldForward", "fire.bmp", 9, 17, 0.05f, true);
	fireRenderForward_->ChangeAnimation("ShieldForward", "fire.bmp");

	fireSkillRender_ = new RSRenderer(this);
	fireSkillRender_->CreateAnimation("OnLeft", "fireSkillLeft.bmp", 0, 11, 0.04f);
	fireSkillRender_->CreateAnimation("OnRight", "fireSkillRight.bmp", 0, 11, 0.04f);
	fireSkillRender_->ChangeAnimation("OnLeft", "fireSkillLeft.bmp");

	bubbleRender_->Deactivate();
	bubbleSkillRender_->Deactivate();
	fireRenderForward_->Deactivate();
	fireSkillRender_->Deactivate();
	electricRender_->Deactivate();

}

void PlayerSkill::UpdateBefore()
{
	location_ = player_->skillPosition_;

	if (eMonitor::FIRE == player_->skillType_)
	{
		backface_->location_ = location_;
	}

	bubbleRender_->UpdateAnimation();

	electricRender_->UpdateAnimation();
	bubbleSkillRender_->UpdateAnimation();
	fireRenderForward_->UpdateAnimation();
	fireSkillRender_->UpdateAnimation();
}

void PlayerSkill::Update()
{
	float deltaTime = RSTimer::GetInstance().GetDeltaTime();
	switch (player_->skillType_)
	{
	case eMonitor::ELECTRIC:
	{
		if (backface_->electricSkillRender_->IsAnimationEnd())
		{
			backface_->electricSkillRender_->Deactivate();
			bElectricSkillCasting_ = false;
		}

		if (bElectricSkillCasting_)
		{
			backface_->electricSkillRender_->SwitchActivation();
		}


		electricShieldAnimationUpdate();
	}
	break;
	case eMonitor::FIRE:
	{
		if (bFireSkillCasting_)
		{
			fireRenderForward_->Deactivate();
			backface_->fireRenderBackward_->Deactivate();

			if (fireSkillRender_->IsAnimationEnd())
			{
				fireRenderForward_->Activate();
				backface_->fireRenderBackward_->Activate();
				fireSkillRender_->Deactivate();
				bFireSkillCasting_ = false;
			}
		}
		else
		{
			if (bBlink_)
			{
				fireRenderForward_->SwitchActivation();
				backface_->fireRenderBackward_->SwitchActivation();
			}
		}



	}
	break;
	case eMonitor::BUBBLE:
	{
		if (!bBubbleSkillCasting_ && bBlink_)
		{
			bubbleRender_->SwitchActivation();
			bubbleSkillRender_->SwitchActivation();
		}
	}
	break;
	default:
		break;
	}

	blinkTimer_ -= deltaTime;

	if (blinkTimer_ < 0.0f)
	{
		bBlink_ = !bBlink_;
		if (bBlink_)
		{
			blinkTimer_ = 0.1f;
		}
		else
		{
			blinkTimer_ = 0.03f;
		}
	}
}

void PlayerSkill::UpdateAfter()
{
}

void PlayerSkill::Render()
{
	switch (player_->skillType_)
	{
	case eMonitor::ELECTRIC:
		electricRender_->Render();
		break;
	case eMonitor::FIRE:
		fireRenderForward_->Render();
		fireSkillRender_->Render();
		break;
	case eMonitor::BUBBLE:
		bubbleRender_->Render();
		bubbleSkillRender_->Render();
		break;
	default:
		break;
	}
}

void PlayerSkill::Cleanup()
{
}

void PlayerSkill::SetPlayer(Player* _player)
{
	player_ = _player;
}

void PlayerSkill::SetBackface(PlayerSkillBackface* _backface)
{
	backface_ = _backface;
}

void PlayerSkill::bubbleShield()
{
	SetRenderOrder(1001);
	bubbleRender_->Activate();
	bubbleSkillRender_->Activate();
	bubbleSkillRender_->ChangeAnimation("Idle", "bubble.bmp");

	backface_->fireRenderBackward_->Deactivate();
	electricRender_->Deactivate();
}

void PlayerSkill::electricShield()
{
	electricRender_->Activate();
	electricAnimCount_ = 0;

	bubbleRender_->Deactivate();
	backface_->fireRenderBackward_->Deactivate();
}

void PlayerSkill::fireShield()
{
	backface_->fireRenderBackward_->Activate();
	fireAnimCount_ = 0;

	bubbleRender_->Deactivate();
	electricRender_->Deactivate();
}

void PlayerSkill::bubbleSkillOn()
{
	bubbleSkillRender_->ChangeAnimation("On", "bubble.bmp", true);
	bubbleSkillRender_->Activate();
	bubbleRender_->Deactivate();
	bBubbleSkillCasting_ = true;
}

void PlayerSkill::bubbleSkillOff()
{
	bubbleSkillRender_->ChangeAnimation("Idle", "bubble.bmp");
	bubbleSkillRender_->Deactivate();
	bubbleRender_->Activate();
	bBubbleSkillCasting_ = false;
}

void PlayerSkill::electricSkillOn()
{
	backface_->electricSkillRender_->ChangeAnimation("On", "electricSkill.bmp", true);
	backface_->SetLocation(player_->skillPosition_);
	backface_->electricSkillRender_->Activate();
	bElectricSkillCasting_ = true;
}

void PlayerSkill::fireSkillLeftOn()
{
	fireSkillRender_->ChangeAnimation("OnLeft", "fireSkillLeft.bmp", true);
	fireSkillRender_->Activate();
	bFireSkillCasting_ = true;
}

void PlayerSkill::fireSkillRightOn()
{
	fireSkillRender_->ChangeAnimation("OnRight", "fireSkillRight.bmp", true);
	fireSkillRender_->Activate();
	bFireSkillCasting_ = true;
}

void PlayerSkill::electricShieldAnimationUpdate()
{
	if (0 == electricAnimCount_)
	{
		if (electricRender_->IsAnimationEnd())
		{
			SetRenderOrder(1001);
			electricRender_->ChangeAnimation("ShieldForward", "electric.bmp");
			++electricAnimCount_;
		}
	}
	else if (1 == electricAnimCount_)
	{
		if (electricRender_->IsAnimationEnd())
		{
			SetRenderOrder(1001);
			electricRender_->ChangeAnimation("ShieldFlash", "electric.bmp");
			++electricAnimCount_;
		}
	}
	else if (2 == electricAnimCount_)
	{
		if (electricRender_->IsAnimationEnd())
		{
			SetRenderOrder(999);
			electricRender_->ChangeAnimation("ShieldBackward", "electric.bmp");
			++electricAnimCount_;
		}
	}
	else
	{
		if (electricRender_->IsAnimationEnd())
		{
			SetRenderOrder(1001);
			electricRender_->ChangeAnimation("ShieldFlash", "electric.bmp");
			electricAnimCount_ = 0;
		}
	}
}

void PlayerSkill::ShieldOff()
{
	backface_->electricSkillRender_->Deactivate();
	backface_->fireRenderBackward_->Deactivate();
	bubbleRender_->Deactivate();
	electricRender_->Deactivate();
	bubbleSkillRender_->Deactivate();
	fireRenderForward_->Deactivate();
	fireSkillRender_->Deactivate();

	bFireSkillCasting_ = false;
	bElectricSkillCasting_ = false;
	bBubbleSkillCasting_ = false;
}

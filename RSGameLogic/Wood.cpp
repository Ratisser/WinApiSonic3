#include "Wood.h"

Wood::Wood()
	: mainRender_(nullptr)
	, skinRender_(nullptr)
	, state_(eWoodState::IDLE)
{
}

Wood::~Wood()
{
	SAFE_DELETE(mainRender_);
	SAFE_DELETE(skinRender_);
}

void Wood::Start()
{
	mainRender_ = new RSRenderer(this);
	mainRender_->CreateAnimation("Idle", "stage1-1_wood.bmp", 8, 8, 1.0f, true);
	mainRender_->ChangeAnimation("Idle", "stage1-1_wood.bmp");

	skinRender_ = new RSRenderer(this);
	skinRender_->CreateAnimation("Idle", "stage1-1_wood.bmp", 0, 0, 1.0f, true);
	skinRender_->CreateAnimation("On", "stage1-1_wood.bmp", 1, 7, 0.05f);
	skinRender_->ChangeAnimation("Idle", "stage1-1_wood.bmp");

	SetRenderOrder(1002);
	float4 location({ 9449.1f, 1175.0f });
	location.y -= 256.f;
	SetLocation(location);
}

void Wood::UpdateBefore()
{
	mainRender_->UpdateAnimation();
	skinRender_->UpdateAnimation();
}

void Wood::Update()
{
	float deltaTime = RSTimer::GetInstance().GetDeltaTime();

	switch (state_)
	{
	case Wood::eWoodState::IDLE:
		break;
	case Wood::eWoodState::WOOD_RUN:
	{
		if (skinRender_->IsAnimationEnd())
		{
			state_ = eWoodState::MOVE;
		}
	}
		break;
	case Wood::eWoodState::MOVE:
	{
		float4 pivot = skinRender_->GetPivot();
		pivot.y += -200.f * deltaTime;
		skinRender_->SetPivot(pivot);
		if (pivot.y < -2000.f)
		{
			skinRender_->Deactivate();
			state_ = eWoodState::END;
		}
	}
		break;
	case Wood::eWoodState::END:
		break;
	default:
		break;
	}
}

void Wood::UpdateAfter()
{
}

void Wood::Render()
{
	mainRender_->Render();
	skinRender_->Render();
}

void Wood::Cleanup()
{
}

void Wood::WoodRun()
{
	if (state_ != eWoodState::END)
	{
		state_ = eWoodState::WOOD_RUN;
		skinRender_->ChangeAnimation("On", "stage1-1_wood.bmp");
	}
}

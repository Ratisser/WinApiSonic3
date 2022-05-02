#include "Spring.h"
#include "Player.h"

#include <RSCollision.h>

Spring::Spring()
	: mainRender_(nullptr)
	, collision_(nullptr)
	, bodyCollision_(nullptr)
	, direction_(eDirection::UP)
	, cooltime_(0.0f)
	, springPower_(600.f)
	, powerDirection_(float4::UP)
{
}

Spring::~Spring()
{
	SAFE_DELETE(mainRender_);
	SAFE_DELETE(collision_);
	SAFE_DELETE(bodyCollision_);
}

void Spring::Start()
{
	SetRenderOrder(99);
	SetUpdateOrder(10);
	mainRender_ = new RSRenderer(this);
	mainRender_->CreateAnimation("UpOff", "spring.bmp", 0, 0, 0.1f);
	mainRender_->CreateAnimation("UpOn", "spring.bmp", 1, 1, 0.1f);
	mainRender_->CreateAnimation("DownOff", "spring.bmp", 2, 2, 0.1f);
	mainRender_->CreateAnimation("DownOn", "spring.bmp", 3, 3, 0.1f);
	mainRender_->CreateAnimation("RightOff", "spring.bmp", 4, 4, 0.1f);
	mainRender_->CreateAnimation("RightOn", "spring.bmp", 5, 5, 0.1f);
	mainRender_->CreateAnimation("LeftOff", "spring.bmp", 6, 6, 0.1f);
	mainRender_->CreateAnimation("LeftOn", "spring.bmp", 7, 7, 0.1f);
	mainRender_->ChangeAnimation("UpOff", "spring.bmp");

	collision_ = new RSCollision(this, eCollisionType::RECT, static_cast<int>(eCollisionCheckGroup::SPRING));
	level_->PushCollision(collision_);

	bodyCollision_ = new RSCollision(this, eCollisionType::RECT, static_cast<int>(eCollisionCheckGroup::MAP));
	level_->PushCollision(bodyCollision_);
}

void Spring::UpdateBefore()
{
	mainRender_->UpdateAnimation();
}

void Spring::Update()
{
	float deltaTime = RSTimer::GetInstance().GetDeltaTime();

	if (cooltime_ > 0)
	{
		switch (direction_)
		{
		case eDirection::UP:
			mainRender_->ChangeAnimation("UpOn", "spring.bmp");
			break;
		case eDirection::DOWN:
			mainRender_->ChangeAnimation("DownOn", "spring.bmp");
			break;
		case eDirection::LEFT:
			mainRender_->ChangeAnimation("LeftOn", "spring.bmp");
			break;
		case eDirection::RIGHT:
			mainRender_->ChangeAnimation("RightOn", "spring.bmp");
			break;
		default:
			break;
		}
		cooltime_ -= deltaTime;
		return;
	}

	RSCollision* playerCollision = collision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::PLAYER));
	if (nullptr != playerCollision)
	{
		Player* player = dynamic_cast<Player*>(playerCollision->GetActor());

		player->SpringJump(springPower_, direction_);
		cooltime_ = COOLTIME;
	}

	switch (direction_)
	{
	case eDirection::UP:
		mainRender_->ChangeAnimation("UpOff", "spring.bmp");
		break;
	case eDirection::DOWN:
		mainRender_->ChangeAnimation("DownOff", "spring.bmp");
		break;
	case eDirection::LEFT:
		mainRender_->ChangeAnimation("LeftOff", "spring.bmp");
		break;
	case eDirection::RIGHT:
		mainRender_->ChangeAnimation("RightOff", "spring.bmp");
		break;
	default:
		break;
	}
}

void Spring::UpdateAfter()
{
}

void Spring::Render()
{
	mainRender_->Render();
	//HDC backbufferDC = RSResourceManager::GetInstance().GetBackBuffer()->GetDC();
	//collision_->DebugRender(backbufferDC);
	//mapCollision_->DebugRender(backbufferDC);
}

void Spring::Cleanup()
{
}

void Spring::SetSpringDirection(eDirection _direction)
{
	direction_ = _direction;
	switch (direction_)
	{
	case eDirection::UP:
		mainRender_->ChangeAnimation("UpOff", "spring.bmp");
		collision_->SetRect(-16.f, -2.0f, 16.f, 8.0f);
		bodyCollision_->SetRect(-17.f, 0.0f, 17.f, 16.f);
		break;
	case eDirection::DOWN:
		mainRender_->ChangeAnimation("DownOff", "spring.bmp");
		collision_->SetRect(-16.f, -8.0f, 16.f, 2.0);
		bodyCollision_->SetRect(-17.f, -16.f, 17.f, 0.0f);
		break;
	case eDirection::LEFT:
		mainRender_->ChangeAnimation("LeftOff", "spring.bmp");
		collision_->SetRect(-2.0f, -16.0f, 8.0f, 16.0f);
		bodyCollision_->SetRect(0.f, -17.f, 16.f, 17.f);
		break;
	case eDirection::RIGHT:
		mainRender_->ChangeAnimation("RightOff", "spring.bmp");
		collision_->SetRect(-8.0f, -16.0f, 2.0f, 16.0f);
		bodyCollision_->SetRect(-16.f, -17.f, 0.f, 17.f);
		break;
	default:
		break;
	}
}

void Spring::SetSpringPower(float _springPower)
{
	springPower_ = _springPower;
}

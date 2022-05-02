#include <RSCollision.h>

#include "Spike.h"
#include "Player.h"

Spike::Spike()
	: mainRender_(nullptr)
	, bodyCollision_(nullptr)
	, spikeCollision_(nullptr)
	, direction_(eDirection::UP)
{
}

Spike::~Spike()
{
	SAFE_DELETE(mainRender_);
	SAFE_DELETE(bodyCollision_);
	SAFE_DELETE(spikeCollision_);
}

void Spike::Start()
{
	SetRenderOrder(99);
	mainRender_ = new RSRenderer(this);
	mainRender_->CreateAnimation("Up", "spike.bmp", 0, 0, 1.0f, true);
	mainRender_->CreateAnimation("Right", "spike.bmp", 1, 1, 1.0f, true);
	mainRender_->CreateAnimation("Down", "spike.bmp", 2, 2, 1.0f, true);
	mainRender_->CreateAnimation("Left", "spike.bmp", 3, 3, 1.0f, true);
	mainRender_->ChangeAnimation("Up", "spike.bmp");

	bodyCollision_ = new RSCollision(this, eCollisionType::RECT, static_cast<int>(eCollisionCheckGroup::MAP));
	bodyCollision_->SetRect(-17.f, -14.0f, 17.f, 16.0f);
	level_->PushCollision(bodyCollision_);

	spikeCollision_ = new RSCollision(this, eCollisionType::RECT, static_cast<int>(eCollisionCheckGroup::SPIKE));
	spikeCollision_->SetRect(-15.f, -16.f, 15.f, -14.f);
	level_->PushCollision(spikeCollision_);
}

void Spike::UpdateBefore()
{
	mainRender_->UpdateAnimation();
}

void Spike::Update()
{
	RSCollision* playerCollision = spikeCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::PLAYER));
	if (nullptr != playerCollision)
	{
		Player* player = dynamic_cast<Player*>(playerCollision->GetActor());

		player->SpikeTrapped();
	}
}

void Spike::UpdateAfter()
{
}

void Spike::Render()
{
	mainRender_->Render();
}

void Spike::Cleanup()
{
}

void Spike::SetSpikeDirection(eDirection _direction)
{
	direction_ = _direction;
	switch (direction_)
	{
	case eDirection::UP:
		mainRender_->ChangeAnimation("Up", "spike.bmp");
		bodyCollision_->SetRect(-17.f, -14.0f, 17.f, 16.0f);
		spikeCollision_->SetRect(-15.f, -16.f, 15.f, -14.f);
		break;
	case eDirection::DOWN:
		mainRender_->ChangeAnimation("Down", "spike.bmp");
		bodyCollision_->SetRect(-17.f, -16.0f, 17.f, 14.0f);
		spikeCollision_->SetRect(-15.f, 14.f, 15.f, 16.f);
		break;
	case eDirection::LEFT:
		mainRender_->ChangeAnimation("Left", "spike.bmp");
		break;
	case eDirection::RIGHT:
		mainRender_->ChangeAnimation("Right", "spike.bmp");
		break;
	default:
		break;
	}
}


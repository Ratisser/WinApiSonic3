#include "StartLocation.h"

#include <RSCollision.h>

StartLocation::StartLocation()
	: bodyCollision_(nullptr)
{
}

StartLocation::~StartLocation()
{
	SAFE_DELETE(bodyCollision_);
}

void StartLocation::Start()
{
	bodyCollision_ = new RSCollision(this, eCollisionType::RECT, static_cast<int>(eCollisionCheckGroup::LOCATION));
	bodyCollision_->SetRect(0.0f, 0.0f, 320.f, 240.f);
	level_->PushCollision(bodyCollision_);

	SetLocation({ 2792.f, 920.f });
	SetRenderOrder(120);
}

void StartLocation::UpdateBefore()
{
}

void StartLocation::Update()
{
}

void StartLocation::UpdateAfter()
{
}

void StartLocation::Render()
{
	//bodyCollision_->DebugRender(RSResourceManager::GetInstance().GetBackBuffer()->GetDC());
}

void StartLocation::Cleanup()
{
}

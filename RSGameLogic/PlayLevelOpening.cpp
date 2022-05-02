#include "PlayLevelOpening.h"

PlayLevelOpening::PlayLevelOpening()
	: openingRender_(nullptr)
{
}

PlayLevelOpening::~PlayLevelOpening()
{
	SAFE_DELETE(openingRender_);

}

void PlayLevelOpening::Start()
{
	SetRenderOrder(50);
	RSImage* background = RSResourceManager::GetInstance().FindImage("stage1-1_dry.bmp");

	RSImage* opening = RSResourceManager::GetInstance().FindImage("stage1-1_opening.bmp");
	SetMovement(opening->GetSize().halffloat4());

	SetMovement({ -3434.f, 920.f });

	openingRender_ = new RSRenderer(this);
	openingRender_->SetImage(opening);

	SetCamEffectScale({ 0.8f, 1.0f });
}

void PlayLevelOpening::UpdateBefore()
{
}

void PlayLevelOpening::Update()
{
}

void PlayLevelOpening::UpdateAfter()
{
}

void PlayLevelOpening::Render()
{
	openingRender_->Render();
}

void PlayLevelOpening::Cleanup()
{
}

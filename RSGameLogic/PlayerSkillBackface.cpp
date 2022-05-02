#include "PlayerSkillBackface.h"

PlayerSkillBackface::PlayerSkillBackface()
	: fireRenderBackward_(nullptr)
	, electricSkillRender_(nullptr)
{
}

PlayerSkillBackface::~PlayerSkillBackface()
{
	SAFE_DELETE(fireRenderBackward_);
	SAFE_DELETE(electricSkillRender_);
}

void PlayerSkillBackface::Start()
{
	SetRenderOrder(999);
	fireRenderBackward_ = new RSRenderer(this);
	fireRenderBackward_->CreateAnimation("ShieldBackward", "fire.bmp", 0, 8, 0.05f, true);
	fireRenderBackward_->ChangeAnimation("ShieldBackward", "fire.bmp");
	fireRenderBackward_->Deactivate();

	electricSkillRender_ = new RSRenderer(this);
	electricSkillRender_->CreateAnimation("On", "electricSkill.bmp", 0, 4, 0.05f);
	electricSkillRender_->ChangeAnimation("On", "electricSkill.bmp");
	electricSkillRender_->Deactivate();
}

void PlayerSkillBackface::UpdateBefore()
{
	fireRenderBackward_->UpdateAnimation();
	electricSkillRender_->UpdateAnimation();
}

void PlayerSkillBackface::Update()
{
}

void PlayerSkillBackface::UpdateAfter()
{
}

void PlayerSkillBackface::Render()
{
	fireRenderBackward_->Render();
	electricSkillRender_->Render();
}

void PlayerSkillBackface::Cleanup()
{
}

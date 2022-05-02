#include "BlindMap.h"

#include <RSCollision.h>

#include <RSRenderer.h>

BlindMap::BlindMap()
	: mainRender_(nullptr)
{
}

BlindMap::~BlindMap()
{
	SAFE_DELETE(mainRender_);
}

void BlindMap::Start()
{
	RSImage* image = RSResourceManager::GetInstance().FindImage("stage1-1_hide.bmp");
	SetLocation(image->GetSize().halffloat4());
	mainRender_ = new RSRenderer(this);
	mainRender_->SetImage(image);

	SetRenderOrder(2000);
}

void BlindMap::UpdateBefore()
{
}

void BlindMap::Update()
{
}

void BlindMap::UpdateAfter()
{
}

void BlindMap::Render()
{
	mainRender_->Render();
}

void BlindMap::Cleanup()
{
}

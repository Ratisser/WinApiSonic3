#include "SonicSeaWave.h"

SonicSeaWave::SonicSeaWave()
	: renderer_(nullptr)
	, bOn_(false)
{

}

SonicSeaWave::~SonicSeaWave()
{
	SAFE_DELETE(renderer_);
}

void SonicSeaWave::Start()
{
	renderer_ = new RSRenderer(this, true, eRenderPivot::BOTTOM);
	renderer_->CreateAnimation("Idle", "splash.bmp", 0, 5, 0.034f, false);
	renderer_->ChangeAnimation("Idle", "splash.bmp");
	renderer_->SetPivot({ -52.f, -46.f });
	SetRenderOrder(1010);
}

void SonicSeaWave::UpdateBefore()
{
	renderer_->UpdateAnimation();
}

void SonicSeaWave::Update()
{

	if (bOn_)
	{
		if (renderer_->IsAnimationEnd())
		{
			Release();
		}
	}
	else
	{
		bOn_ = true;
		renderer_->ChangeAnimation("Idle", "splash.bmp", true);
	}
}

void SonicSeaWave::UpdateAfter()
{
}

void SonicSeaWave::Render()
{
	renderer_->Render();
}

void SonicSeaWave::Cleanup()
{
}

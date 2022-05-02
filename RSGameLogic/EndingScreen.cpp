#include "EndingScreen.h"

EndingScreen::EndingScreen()
	: mainRender_(nullptr)
{
}

EndingScreen::~EndingScreen()
{
	SAFE_DELETE(mainRender_);
}

void EndingScreen::Start()
{
	mainRender_ = new RSRenderer(this, false);

	mainRender_->SetImage("ending.bmp");
	mainRender_->SetPivot({ 160.f, 120.f });
}

void EndingScreen::UpdateBefore()
{
}

void EndingScreen::Update()
{
}

void EndingScreen::UpdateAfter()
{
}

void EndingScreen::Render()
{
	mainRender_->Render();
}

void EndingScreen::Cleanup()
{
}

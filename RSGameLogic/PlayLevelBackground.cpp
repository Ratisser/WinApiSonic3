#include "PlayLevelBackground.h"

#include <RSRenderer.h>
#include <RSResourceManager.h>

PlayLevelBackground::PlayLevelBackground()
	: backLevel0_(nullptr)
	, backLevel1_0_(nullptr)
	, backLevel1_1_(nullptr)
	, backLevel2_()
	, backLevel3_()
	, backLevel4_()
	, backLevel5_()
	, backLevel6_()
	, backLevel7_(nullptr)
{
	backLevel2_.reserve(LEVEL2_COUNT);
	backLevel3_.reserve(LEVEL2_COUNT);
	backLevel4_.reserve(LEVEL2_COUNT);
	backLevel5_.reserve(LEVEL2_COUNT);
	backLevel6_.reserve(LEVEL2_COUNT);
}

PlayLevelBackground::~PlayLevelBackground()
{
	SAFE_DELETE(backLevel0_);
	SAFE_DELETE(backLevel1_0_);
	SAFE_DELETE(backLevel1_1_);
	SAFE_DELETE(backLevel7_);

	std::vector<RSRenderer*>::iterator startIter = backLevel2_.begin();
	std::vector<RSRenderer*>::iterator endIter = backLevel2_.end();

	while (startIter != endIter)
	{
		SAFE_DELETE(*startIter);
		startIter++;
	}

	startIter = backLevel3_.begin();
	endIter = backLevel3_.end();

	while (startIter != endIter)
	{
		SAFE_DELETE(*startIter);
		startIter++;
	}

	startIter = backLevel4_.begin();
	endIter = backLevel4_.end();

	while (startIter != endIter)
	{
		SAFE_DELETE(*startIter);
		startIter++;
	}

	startIter = backLevel5_.begin();
	endIter = backLevel5_.end();

	while (startIter != endIter)
	{
		SAFE_DELETE(*startIter);
		startIter++;
	}

	startIter = backLevel6_.begin();
	endIter = backLevel6_.end();

	while (startIter != endIter)
	{
		SAFE_DELETE(*startIter);
		startIter++;
	}

	backLevel2_.clear();
	backLevel3_.clear();
	backLevel4_.clear();
	backLevel5_.clear();
	backLevel6_.clear();
}

void PlayLevelBackground::Start()
{
	RSImage* backLevel0 = RSResourceManager::GetInstance().FindImage("stage1-1_background3.bmp");
	RSImage* backLevel1 = RSResourceManager::GetInstance().FindImage("stage1-1_background2.bmp");

	backLevel0_ = new RSRenderer(this);
	backLevel1_0_ = new RSRenderer(this);
	backLevel1_1_ = new RSRenderer(this);

	backLevel0_->SetImage(backLevel0);
	backLevel1_0_->SetImage(backLevel1);
	backLevel1_1_->SetImage(backLevel1);

	SetLocation(backLevel0->GetSize().halffloat4());
	SetRenderOrder(50);

	for (size_t i = 0; i < LEVEL2_COUNT; i++)
	{
		RSRenderer* newRenderer = new RSRenderer(this);
		newRenderer->CreateAnimation("Level2", "stage1-1_background1.bmp", 19, 19, 1.0f);
		newRenderer->ChangeAnimation("Level2", "stage1-1_background1.bmp");
		backLevel2_.push_back(newRenderer);
	}

	for (size_t i = 0; i < LEVEL2_COUNT; i++)
	{
		RSRenderer* newRenderer = new RSRenderer(this);
		newRenderer->CreateAnimation("Level3", "stage1-1_background1.bmp", 18, 18, 1.0f);
		newRenderer->ChangeAnimation("Level3", "stage1-1_background1.bmp");
		backLevel3_.push_back(newRenderer);
	}

	for (size_t i = 0; i < LEVEL2_COUNT; i++)
	{
		RSRenderer* newRenderer = new RSRenderer(this);
		newRenderer->CreateAnimation("Level4", "stage1-1_background1.bmp", 17, 17, 1.0f);
		newRenderer->ChangeAnimation("Level4", "stage1-1_background1.bmp");
		backLevel4_.push_back(newRenderer);
	}

	for (size_t i = 0; i < LEVEL2_COUNT; i++)
	{
		RSRenderer* newRenderer = new RSRenderer(this);
		newRenderer->CreateAnimation("Level5", "stage1-1_background1.bmp", 16, 16, 1.0f);
		newRenderer->ChangeAnimation("Level5", "stage1-1_background1.bmp");
		backLevel5_.push_back(newRenderer);
	}

	for (size_t i = 0; i < LEVEL2_COUNT; i++)
	{
		RSRenderer* newRenderer = new RSRenderer(this);
		newRenderer->CreateAnimation("Level6", "stage1-1_background1.bmp", 11, 15, 0.3f, true);
		newRenderer->ChangeAnimation("Level6", "stage1-1_background1.bmp");
		backLevel6_.push_back(newRenderer);
	}

	backLevel7_ = new RSRenderer(this);
	backLevel7_->CreateAnimation("Level7_", "stage1-1_background1.bmp", 7, 7, 1.0f);
	backLevel7_->ChangeAnimation("Level7_", "stage1-1_background1.bmp");

	float pivotX = 0.0f;
	float pivotY = 320.0f;
	backLevel0_->SetPivot({ 0.0f, pivotY });
	backLevel1_0_->SetPivot({ pivotX, pivotY - 192.f });
	backLevel1_1_->SetPivot({ pivotX + 3840.f, pivotY - 192.f });

	pivotX -= 512.f;
	pivotY -= 292.f;

	for (size_t i = 0; i < LEVEL2_COUNT; i++)
	{
		backLevel2_[i]->SetPivot({ pivotX + i * 512.f, pivotY });
	}

	pivotY -= 14.f;

	for (size_t i = 0; i < LEVEL2_COUNT; i++)
	{
		backLevel3_[i]->SetPivot({ pivotX + i * 512.f, pivotY });
	}

	pivotY -= 5.f;

	for (size_t i = 0; i < LEVEL2_COUNT; i++)
	{
		backLevel4_[i]->SetPivot({ pivotX + i * 512.f, pivotY });
	}

	pivotX -= 512.f;
	pivotY -= 11.f;

	for (size_t i = 0; i < LEVEL2_COUNT; i++)
	{
		backLevel5_[i]->SetPivot({ pivotX + i * 512.f, pivotY });
	}

	pivotX -= 712.f;
	pivotY -= 14.f;

	for (size_t i = 0; i < LEVEL2_COUNT; i++)
	{
		backLevel6_[i]->SetPivot({ pivotX + i * 512.f, pivotY });
	}

	pivotX -= 50.f;
	pivotY -= 16.f;

	backLevel7_->SetPivot({pivotX, pivotY});
}

void PlayLevelBackground::UpdateBefore()
{
}

void PlayLevelBackground::Update()
{
	for (size_t i = 0; i < LEVEL2_COUNT; i++)
	{
		backLevel2_[i]->UpdateAnimation();
		backLevel3_[i]->UpdateAnimation();
		backLevel4_[i]->UpdateAnimation();
		backLevel5_[i]->UpdateAnimation();
		backLevel6_[i]->UpdateAnimation();
	}

	backLevel7_->UpdateAnimation();
}

void PlayLevelBackground::UpdateAfter()
{
}

void PlayLevelBackground::Render()
{
	SetCamEffectScale({ 0.33f, 0.3f });
	backLevel0_->Render();
	SetCamEffectScale({ 0.37f, 0.3f });
	backLevel1_0_->Render();
	backLevel1_1_->Render();

	SetCamEffectScale({ 0.05f, 0.3f });
	for (size_t i = 0; i < LEVEL2_COUNT; i++)
	{
		backLevel6_[i]->Render();
	}

	SetCamEffectScale({ 0.02f, 0.3f });
	backLevel7_->Render();

	SetCamEffectScale({ 0.25f, 0.3f });
	for (size_t i = 0; i < LEVEL2_COUNT; i++)
	{
		backLevel5_[i]->Render();
	}

	SetCamEffectScale({ 0.33f, 0.3f });
	for (size_t i = 0; i < LEVEL2_COUNT; i++)
	{
		backLevel4_[i]->Render();
	}

	SetCamEffectScale({ 0.35f, 0.3f });
	for (size_t i = 0; i < LEVEL2_COUNT; i++)
	{
		backLevel3_[i]->Render();
	}

	SetCamEffectScale({ 0.4f, 0.3f });
	for (size_t i = 0; i < LEVEL2_COUNT; i++)
	{
		backLevel2_[i]->Render();
	}
}

void PlayLevelBackground::Cleanup()
{
}

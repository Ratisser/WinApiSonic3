#include "PlayLevelSky.h"

PlayLevelSky::PlayLevelSky()
	: cloud00_(nullptr)
	, cloud01_(nullptr)
	, cloud10_(nullptr)
	, cloud11_(nullptr)
	, cloud20_(nullptr)
	, cloud21_(nullptr)
	, cloud30_(nullptr)
	, cloud31_(nullptr)
	, cloud40_(nullptr)
	, cloud41_(nullptr)
	, cloud50_(nullptr)
	, cloud51_(nullptr)
{
}

PlayLevelSky::~PlayLevelSky()
{
	SAFE_DELETE(cloud00_);
	SAFE_DELETE(cloud01_);
	SAFE_DELETE(cloud10_);
	SAFE_DELETE(cloud11_);
	SAFE_DELETE(cloud20_);
	SAFE_DELETE(cloud21_);
	SAFE_DELETE(cloud30_);
	SAFE_DELETE(cloud31_);
	SAFE_DELETE(cloud40_);
	SAFE_DELETE(cloud41_);
	SAFE_DELETE(cloud50_);
	SAFE_DELETE(cloud51_);
}

void PlayLevelSky::Start()
{
	cloud00_ = new RSRenderer(this);
	cloud01_ = new RSRenderer(this);
	cloud10_ = new RSRenderer(this);
	cloud11_ = new RSRenderer(this);
	cloud20_ = new RSRenderer(this);
	cloud21_ = new RSRenderer(this);
	cloud30_ = new RSRenderer(this);
	cloud31_ = new RSRenderer(this);
	cloud40_ = new RSRenderer(this);
	cloud41_ = new RSRenderer(this);
	cloud50_ = new RSRenderer(this);
	cloud51_ = new RSRenderer(this);

	SetRenderOrder(60);

	cloud00_->CreateAnimation("cloud", "stage1-1_background1.bmp", 6, 6, 1.0f);
	cloud01_->CreateAnimation("cloud", "stage1-1_background1.bmp", 6, 6, 1.0f);
	cloud10_->CreateAnimation("cloud", "stage1-1_background1.bmp", 5, 5, 1.0f);
	cloud11_->CreateAnimation("cloud", "stage1-1_background1.bmp", 5, 5, 1.0f);
	cloud20_->CreateAnimation("cloud", "stage1-1_background1.bmp", 3, 3, 1.0f);
	cloud21_->CreateAnimation("cloud", "stage1-1_background1.bmp", 3, 3, 1.0f);
	cloud30_->CreateAnimation("cloud", "stage1-1_background1.bmp", 2, 2, 1.0f);
	cloud31_->CreateAnimation("cloud", "stage1-1_background1.bmp", 2, 2, 1.0f);
	cloud40_->CreateAnimation("cloud", "stage1-1_background1.bmp", 1, 1, 1.0f);
	cloud41_->CreateAnimation("cloud", "stage1-1_background1.bmp", 1, 1, 1.0f);
	cloud50_->CreateAnimation("cloud", "stage1-1_background1.bmp", 0, 0, 1.0f);
	cloud51_->CreateAnimation("cloud", "stage1-1_background1.bmp", 0, 0, 1.0f);

	cloud00_->ChangeAnimation("cloud", "stage1-1_background1.bmp");
	cloud01_->ChangeAnimation("cloud", "stage1-1_background1.bmp");
	cloud10_->ChangeAnimation("cloud", "stage1-1_background1.bmp");
	cloud11_->ChangeAnimation("cloud", "stage1-1_background1.bmp");
	cloud20_->ChangeAnimation("cloud", "stage1-1_background1.bmp");
	cloud21_->ChangeAnimation("cloud", "stage1-1_background1.bmp");
	cloud30_->ChangeAnimation("cloud", "stage1-1_background1.bmp");
	cloud31_->ChangeAnimation("cloud", "stage1-1_background1.bmp");
	cloud40_->ChangeAnimation("cloud", "stage1-1_background1.bmp");
	cloud41_->ChangeAnimation("cloud", "stage1-1_background1.bmp");
	cloud50_->ChangeAnimation("cloud", "stage1-1_background1.bmp");
	cloud51_->ChangeAnimation("cloud", "stage1-1_background1.bmp");

	float pivotX = 0.0f;
	float pivotY = 124.f;

	cloud00_->SetPivot({ pivotX, pivotY });
	cloud01_->SetPivot({ pivotX + 512.f, pivotY });

	pivotY -= 16.0f;
	cloud10_->SetPivot({ pivotX, pivotY });
	cloud11_->SetPivot({ pivotX + 512.f, pivotY });

	pivotY -= 26.f;
	cloud20_->SetPivot({ pivotX, pivotY });
	cloud21_->SetPivot({ pivotX + 512.f, pivotY });

	pivotY -= 48.f;
	cloud30_->SetPivot({ pivotX, pivotY });
	cloud31_->SetPivot({ pivotX + 512.f, pivotY });

	pivotY -= 48.f;
	cloud40_->SetPivot({ pivotX, pivotY });
	cloud41_->SetPivot({ pivotX + 512.f, pivotY });

	pivotY -= 208.f;
	cloud50_->SetPivot({ pivotX, pivotY });
	cloud51_->SetPivot({ pivotX + 512.f, pivotY });
}

void PlayLevelSky::UpdateBefore()
{
	cloud00_->UpdateAnimation();
	cloud01_->UpdateAnimation();
	cloud10_->UpdateAnimation();
	cloud11_->UpdateAnimation();
	cloud20_->UpdateAnimation();
	cloud21_->UpdateAnimation();
	cloud30_->UpdateAnimation();
	cloud31_->UpdateAnimation();
	cloud40_->UpdateAnimation();
	cloud41_->UpdateAnimation();
	cloud50_->UpdateAnimation();
	cloud51_->UpdateAnimation();
}

void PlayLevelSky::Update()
{
	float deltaTime = RSTimer::GetInstance().GetDeltaTime();
	
	moveCloud(cloud00_, 10.f, deltaTime);
	moveCloud(cloud01_, 10.f, deltaTime);
	moveCloud(cloud10_, 15.f, deltaTime);
	moveCloud(cloud11_, 15.f, deltaTime);
	moveCloud(cloud20_, 25.f, deltaTime);
	moveCloud(cloud21_, 25.f, deltaTime);
	moveCloud(cloud30_, 35.f, deltaTime);
	moveCloud(cloud31_, 35.f, deltaTime);
	moveCloud(cloud40_, 30.f, deltaTime);
	moveCloud(cloud41_, 30.f, deltaTime);
	moveCloud(cloud50_, 30.f, deltaTime);
	moveCloud(cloud51_, 30.f, deltaTime);
}

void PlayLevelSky::UpdateAfter()
{
}

void PlayLevelSky::Render()
{
	SetCamEffectScale({ 0.01f, 0.3f });
	cloud51_->Render();
	cloud50_->Render();
	cloud41_->Render();
	cloud40_->Render();
	cloud31_->Render();
	cloud30_->Render();
	cloud21_->Render();
	cloud20_->Render();
	cloud11_->Render();
	cloud10_->Render();
	cloud01_->Render();
	cloud00_->Render();
}

void PlayLevelSky::Cleanup()
{
}

void PlayLevelSky::moveCloud(RSRenderer* _cloud, float _speed, float _deltaTime)
{
	float4 pivot;
	pivot = _cloud->GetPivot();
	pivot.x = pivot.x - _speed * _deltaTime;
	if (pivot.x < -512.f)
	{
		pivot.x = 512.f;
	}
	_cloud->SetPivot(pivot);
}

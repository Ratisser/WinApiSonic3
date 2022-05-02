#include "GoalPost.h"

#include <RSCollision.h>
#include <RSSoundPlayer.h>

#include "Player.h"
#include "PlayLevelUI.h"
#include "eCollisionCheckColor.h"

GoalPost::GoalPost()
	: state_(eGoalPostState::WAIT)
	, mainRender_(nullptr)
	, bottomCenterCollision_(nullptr)
	, bodyCollision_(nullptr)
	, centerLeftCollision_(nullptr)
	, centerRightCollision_(nullptr)
	, dropSound_(nullptr)
	, dropSpeed_(float4::ZERO)
	, touchDelay_(0.0f)
{
}

GoalPost::~GoalPost()
{
	SAFE_DELETE(mainRender_);
	SAFE_DELETE(bottomCenterCollision_);
	SAFE_DELETE(centerLeftCollision_);
	SAFE_DELETE(centerRightCollision_);
	SAFE_DELETE(bodyCollision_);
	SAFE_DELETE(dropSound_);
}

void GoalPost::Start()
{
	mainRender_ = new RSRenderer(this);
	mainRender_->CreateAnimation("Rotate", "goalPost.bmp", 0, 4, 0.03f, true);
	mainRender_->CreateAnimation("Sonic", "goalPost.bmp", 4, 4, 1.0f, false);
	mainRender_->ChangeAnimation("Rotate", "goalPost.bmp");
	mainRender_->Deactivate();

	bottomCenterCollision_ = new RSCollision(this, eCollisionType::POINT, static_cast<int>(eCollisionCheckGroup::GOAL_POST));
	bottomCenterCollision_->SetCheckColor(static_cast<DWORD>(eCollisionCheckColor::MAP));
	bottomCenterCollision_->SetPivot({ 0.0f, 23.f });

	centerLeftCollision_ = new RSCollision(this, eCollisionType::POINT, static_cast<int>(eCollisionCheckGroup::GOAL_POST));
	centerLeftCollision_->SetCheckColor(static_cast<DWORD>(eCollisionCheckColor::MAP));
	centerLeftCollision_->SetPivot({ -24.0f, 0.f });

	centerRightCollision_ = new RSCollision(this, eCollisionType::POINT, static_cast<int>(eCollisionCheckGroup::GOAL_POST));
	centerRightCollision_->SetCheckColor(static_cast<DWORD>(eCollisionCheckColor::MAP));
	centerRightCollision_->SetPivot({ 24.0f, 0.f });

	bodyCollision_ = new RSCollision(this, eCollisionType::RECT, static_cast<int>(eCollisionCheckGroup::GOAL_POST));
	bodyCollision_->SetRect(-24.0f, -24.0f, 24.0f, 24.0f);

	dropSound_ = new RSSoundPlayer("goalPostDrop.wav");

	SetRenderOrder(999);
}

void GoalPost::UpdateBefore()
{
	mainRender_->UpdateAnimation();
}

void GoalPost::Update()
{
	float deltaTime = RSTimer::GetInstance().GetDeltaTime();
	switch (state_)
	{
	case eGoalPostState::WAIT:
		break;
	case eGoalPostState::DROP:
		onDrop(deltaTime);
		break;
	case eGoalPostState::DROPPED:
		onDropped(deltaTime);
		break;
	case eGoalPostState::ROTATE_FINISH:
		onRotateFinish(deltaTime);
		break;
	default:
		break;
	}
}

void GoalPost::UpdateAfter()
{
}

void GoalPost::Render()
{
	mainRender_->Render();
}

void GoalPost::Cleanup()
{
}

void GoalPost::Drop(const float4& _location)
{
	SetLocation(_location);
	dropSpeed_.y = 20.f;
	mainRender_->Activate();
	dropSound_->Play();
	state_ = eGoalPostState::DROP;
}

void GoalPost::onDrop(float _deltaTime)
{
	touchDelay_ -= _deltaTime;
	if (nullptr != bottomCenterCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
	{
		RSSoundManager::GetInstance().PlaySoundByName("grab.wav");
		state_ = eGoalPostState::DROPPED;
	}

	RSCollision* playerCollision = bodyCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::PLAYER));
	if (nullptr != playerCollision && touchDelay_ < 0.0f)
	{
		Player* player = dynamic_cast<Player*>(playerCollision->GetActor());
		float4 toMove = location_ - player->GetLocation();

		if (player->GetGravitySpeed().y < 0)
		{
			toMove.x = toMove.x * 7;
			toMove.y = -150.f;

			dropSpeed_ = toMove;
			touchDelay_ = 0.5f;
			dropSound_->Stop();
			dropSound_->Play();
		}
	}

	if (nullptr != centerLeftCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
	{
		dropSpeed_.x *= -1;
	}

	if (nullptr != centerRightCollision_->GetCollisionGroupCheckOne(static_cast<int>(eCollisionCheckGroup::MAP)))
	{
		dropSpeed_.x *= -1;
	}

	if (dropSpeed_.x < 0)
	{
		dropSpeed_.x += 50.f * _deltaTime;
	}
	else if (dropSpeed_.x > 0)
	{
		dropSpeed_.x -= 50.f * _deltaTime;
	}

	dropSpeed_.y += 200.f * _deltaTime;

	SetMovement(dropSpeed_ * _deltaTime);
}

void GoalPost::onDropped(float _deltaTime)
{
	static float frameTime = 0.03f;
	frameTime += _deltaTime / 50.f;
	mainRender_->SetFrameTime("Rotate", frameTime);

	if (frameTime > 0.09f)
	{
		mainRender_->ChangeAnimation("Sonic", "goalPost.bmp", true);
	}
	
	if (frameTime > 0.11f)
	{
		Player* player = level_->FindActor<Player>("Player");
		PlayLevelUI* ui = level_->FindActor<PlayLevelUI>("PlayLevelUI");
		if (player->IsGround())
		{
			player->StageClear();
			ui->StageClear();
			state_ = eGoalPostState::ROTATE_FINISH;
		}
	}
}

void GoalPost::onRotateFinish(float _deltaTime)
{
}

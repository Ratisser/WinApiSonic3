#pragma once

#include <RSActor.h>

#include "eGoalPostState.h"

class RSCollision;
class RSSoundPlayer;
class GoalPost : public RSActor
{
public:
	GoalPost();
	~GoalPost();

	GoalPost(const GoalPost& _other) = delete;
	GoalPost(GoalPost&& _other) = delete;

	GoalPost& operator=(const GoalPost& _other) = delete;
	GoalPost& operator=(const GoalPost&& _other) = delete;

	virtual void Start() override;
	virtual void UpdateBefore() override;
	virtual void Update() override;
	virtual void UpdateAfter() override;
	virtual void Render() override;
	virtual void Cleanup() override;

	void Drop(const float4& _location);

private:
	void onDrop(float _deltaTime);
	void onDropped(float _deltaTime);
	void onRotateFinish(float _deltaTime);


private:
	RSRenderer*		mainRender_;
	RSCollision*	bottomCenterCollision_;
	RSCollision*	centerLeftCollision_;
	RSCollision*	centerRightCollision_;
	RSCollision*	bodyCollision_;
	RSSoundPlayer*	dropSound_;

	float4			dropSpeed_;
	float			touchDelay_;

	eGoalPostState	state_;
};


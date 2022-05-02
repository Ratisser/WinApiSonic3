#pragma once

#include <RSActor.h>

#include "eDirection.h"

class Spring : public RSActor
{
public:
	Spring();
	~Spring();

	Spring(const Spring& _other) = delete;
	Spring(Spring&& _other) = delete;

	Spring& operator=(const Spring& _other) = delete;
	Spring& operator=(const Spring&& _other) = delete;

	virtual void Start() override;
	virtual void UpdateBefore() override;
	virtual void Update() override;
	virtual void UpdateAfter() override;
	virtual void Render() override;
	virtual void Cleanup() override;

	void SetSpringDirection(eDirection _direction);
	void SetSpringPower(float _springPower);

private:
	const float COOLTIME = 0.2f;
	RSRenderer* mainRender_;
	RSCollision* collision_;
	RSCollision* bodyCollision_;

	eDirection direction_;

	float cooltime_;
	float springPower_;

	float4 powerDirection_;
};


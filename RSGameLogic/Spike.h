#pragma once

#include <RSActor.h>

#include "eDirection.h"

class RSCollision;
class Spike : public RSActor
{
public:
	Spike();
	~Spike();

	Spike(const Spike& _other) = delete;
	Spike(Spike&& _other) = delete;

	Spike& operator=(const Spike& _other) = delete;
	Spike& operator=(const Spike&& _other) = delete;

	virtual void Start() override;
	virtual void UpdateBefore() override;
	virtual void Update() override;
	virtual void UpdateAfter() override;
	virtual void Render() override;
	virtual void Cleanup() override;

	void SetSpikeDirection(eDirection _direction);

private:
	RSRenderer* mainRender_;
	RSCollision* bodyCollision_;
	RSCollision* spikeCollision_;

	eDirection direction_;
};


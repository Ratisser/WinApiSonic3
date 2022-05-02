#pragma once

#include <RSActor.h>

class RSCollision;
class StartLocation : public RSActor
{
public:
	StartLocation();
	~StartLocation();

	StartLocation(const StartLocation& _other) = delete;
	StartLocation(StartLocation&& _other) = delete;

	StartLocation& operator=(const StartLocation& _other) = delete;
	StartLocation& operator=(const StartLocation&& _other) = delete;

	virtual void Start() override;
	virtual void UpdateBefore() override;
	virtual void Update() override;
	virtual void UpdateAfter() override;
	virtual void Render() override;
	virtual void Cleanup() override;

private:
	RSCollision* bodyCollision_;
};


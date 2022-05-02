#pragma once

#include <RSActor.h>

class RSCollision;
class BossArea : public RSActor
{
public:
	BossArea();
	~BossArea();

	BossArea(const BossArea& _other) = delete;
	BossArea(BossArea&& _other) = delete;

	BossArea& operator=(const BossArea& _other) = delete;
	BossArea& operator=(const BossArea&& _other) = delete;

	virtual void Start() override;
	virtual void UpdateBefore() override;
	virtual void Update() override;
	virtual void UpdateAfter() override;
	virtual void Render() override;
	virtual void Cleanup() override;

private:
	RSCollision* bossArea_;
	RSCollision* bossAreaWallLeft_;
	RSCollision* bossAreaWallRight_;


	float delay_;

	bool bTriggered_;
	bool bTriggering_;
};


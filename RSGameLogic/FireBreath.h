#pragma once

#include <RSActor.h>
#include <list>

#include "eFireBreathState.h"

class RSSoundPlayer;
class FireBreath : public RSActor
{
public:
	FireBreath();
	~FireBreath();

	FireBreath(const FireBreath& _other) = delete;
	FireBreath(FireBreath&& _other) = delete;

	FireBreath& operator=(const FireBreath& _other) = delete;
	FireBreath& operator=(const FireBreath&& _other) = delete;

	virtual void Start() override;
	virtual void UpdateBefore() override;
	virtual void Update() override;
	virtual void UpdateAfter() override;
	virtual void Render() override;
	virtual void Cleanup() override;

	void Appear();
	void Attacked();
	bool IsLeftSide();

private:
	void onAppear(float _deltaTime);
	void onDownLeft(float _deltaTime);
	void onAttackLeft(float _deltaTime);
	void onUpLeft(float _deltaTime);
	void onMoveLeft(float _deltaTime);
	void onDownRight(float _deltaTime);
	void onAttackRight(float _deltaTime);
	void onUpRight(float _deltaTime);
	void onMoveRight(float _deltaTime);
	void onDestroyLeft(float _deltaTime);
	void onDestroyRight(float _deltaTime);
	void onSplashLeft(float _deltaTime);
	void onSplashRight(float _deltaTime);
	void onSplashAfter(float _deltaTime);

private:
	RSRenderer* mainRender_;
	RSRenderer* splashRender1_;
	RSRenderer* splashRender2_;
	RSRenderer* splashRender3_;
	RSRenderer* splashRender4_;

	RSRenderer* flameRender_;
	RSRenderer* flameRender1_;
	RSRenderer* flameRender2_;
	RSRenderer* flameRender3_;
	RSRenderer* flameRender4_;

	RSCollision* bodyCollision_;
	RSCollision* flameCollision_;

	RSSoundPlayer* destroySound_;

	eFireBreathState state_;

	std::list<RSRenderer*> destroyEffectList_;

	bool bCanHit_;
	bool bDestroy_;
	float hitDelay_;
	float nextStateDelay_;
	float hoveringFactor_;
	float destroyEffectDelay_;
	float destroyTime_;
	float blinkTime_;
	float splashTime_;

	int hp_;
};


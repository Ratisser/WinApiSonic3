#pragma once

#include <RSActor.h>

#include "eOpeningState.h"

class Player;
class OpeningController : public RSActor
{
public:
	OpeningController();
	~OpeningController();

	OpeningController(const OpeningController& _other) = delete;
	OpeningController(OpeningController&& _other) = delete;

	OpeningController& operator=(const OpeningController& _other) = delete;
	OpeningController& operator=(const OpeningController&& _other) = delete;

	virtual void Start() override;
	virtual void UpdateBefore() override;
	virtual void Update() override;
	virtual void UpdateAfter() override;
	virtual void Render() override;
	virtual void Cleanup() override;

	void onAppear(float _deltaTime);
	void onFly(float _deltaTime);
	void onSonicJump(float _deltaTime);
	void onSuperSonic(float _deltaTime);
	void onFallowSonic(float _deltaTime);
	void onKnucklesJump(float _deltaTime);
	void onKnucklesRunLeft(float _deltaTime);
	void onKnucklesRunRight(float _deltaTime);
	void onKnucklesLaugh(float _deltaTime);
	void onKnucklesRunout(float _deltaTime);
	void onEndOpening(float _deltaTime);


private:
	Player* player_;
	RSRenderer* sonicRender_;
	RSRenderer* aircraftRender_;
	RSRenderer* knucklesRender_;

	float4 sonicPos_;
	float4 aircraftPos_;
	float4 knucklesPos_;

	float4 moveSpeed_;
	float hoveringFactor_;

	eOpeningState state_;

	float timer_;
	float seaWaveTimer_;
};


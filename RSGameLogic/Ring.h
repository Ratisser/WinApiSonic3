#pragma once

#include <RSActor.h>
#include <list>

#include "eRingState.h"

class RSRenderer;
class RSCollision;
class RSSoundPlayer;
class Player;
class PlayLevelUI;
class EditLevelController;
class Ring : public RSActor
{
	friend EditLevelController;
public:
	Ring();
	~Ring();

	Ring(const Ring& _other) = delete;
	Ring(Ring&& _other) = delete;

	Ring& operator=(const Ring& _other) = delete;
	Ring& operator=(const Ring&& _other) = delete;

	virtual void Start() override;
	virtual void UpdateBefore() override;
	virtual void Update() override;
	virtual void UpdateAfter() override;
	virtual void Render() override;
	virtual void Cleanup() override;

	void Deactivate();
	bool IsCollected();
	void Drop(const float4& _location);

private:
	void dropStart(float _deltaTime);
	void dropUpdate(float _deltaTime);
	void dropAfter(float _deltaTime);

private:
	RSRenderer* mainRender_;
	RSCollision* collision_;
	RSCollision* groundCollision_;
	RSSoundPlayer* soundPlayer_;
	eRingState state_;

	float4 dropSpeed_;
	float dropStartDelay_;
	float dropUpdateDelay_;
	float dropAfterDelay_;
	float blinkTimer_;

	PlayLevelUI* ui_;
};


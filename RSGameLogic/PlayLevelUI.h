#pragma once

#include <RSActor.h>
#include <vector>

#include "eUIState.h"

class Player;
class RSRenderer;
class PlayLevelUI : public RSActor
{
public:
	PlayLevelUI();
	~PlayLevelUI();

	PlayLevelUI(const PlayLevelUI& _other) = delete;
	PlayLevelUI(PlayLevelUI&& _other) = delete;

	PlayLevelUI& operator=(const PlayLevelUI& _other) = delete;
	PlayLevelUI& operator=(const PlayLevelUI&& _other) = delete;

	virtual void Start() override;
	virtual void UpdateBefore() override;
	virtual void Update() override;
	virtual void UpdateAfter() override;
	virtual void Render() override;
	virtual void Cleanup() override;

	float GetTime() const;

	int GetRingCount() const;
	
	void StopTime();
	void StartTime();
	void ResetTime();
	void SubstractOneSecond();
	void AddRing();
	void AddRing(int _ringCount);
	void SubtractRing();
	void SetRing(int _ringCount);
	void AddLife();
	void SubtractLife();

	void StageStart();
	void StageClear();

private:
	void onStageStart(float _deltaTime);
	void onStartEnd(float _deltaTime);
	void onStageClear(float _deltaTime);
	void onClearCount(float _deltaTime);
	void onCountEnd(float _deltaTime);

private:
	RSRenderer* scoreRenderer_;
	RSRenderer* timeRenderer_;
	RSRenderer* ringRenderer_;
	RSRenderer* lifeRenderer_;
	RSRenderer* portraitRenderer_;
	
	RSRenderer* number_;
	RSRenderer* colon_;

	Player* player_;
	int life_;
	int score_;
	int ring_;
	float time_;
	float ringBlinkTimer_;

	bool bStopTime_;
	bool bOverHundredRing_;

	eUIState state_;
	float animationTime_;

	RSRenderer* stageStartRender0_;
	RSRenderer* stageStartRender1_;
	RSRenderer* stageStartRender2_;
	RSRenderer* stageStartRender3_;

	RSRenderer* stageClearRender0_;
	RSRenderer* stageClearRender1_;
	RSRenderer* stageClearRender2_;
	RSRenderer* stageClearRender3_;

	float4 scorePivot_;
	int timeBonus_;
	int ringBonus_;
	int total_;
};


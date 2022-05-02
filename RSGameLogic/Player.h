#pragma once
#include <RSActor.h>
#include <list>

#include "ePlayerState.h"
#include "eMonitor.h"
#include "eDirection.h"

class RSRenderer;
class RSCollision;
class RSSoundPlayer;
class PlayerSkill;
class Ring;
class OpeningController;
class Player : public RSActor
{
	friend PlayerSkill;
	friend OpeningController;
public:
	Player();
	~Player();

	Player(const Player& _other) = delete;
	Player(Player&& _other) = delete;

	Player& operator=(const Player& _other) = delete;
	Player& operator=(const Player&& _other) = delete;

public:
	virtual void Start() override;
	virtual void UpdateBefore() override;
	virtual void Update() override;
	virtual void UpdateAfter() override;
	virtual void Render() override;
	virtual void Cleanup() override;

public:
	void ChangeState(ePlayerState _state, float _delay = 0.0f);
	void ResetState();
	ePlayerState GetState();
	void DebugRender();

	void SetSkillType(eMonitor _skill);
	void SetSkill(PlayerSkill* _skill);
	float4 GetSpeed();
	float4 SetSpeed(const float4& _speed);
	float4 GetGravitySpeed();
	float4 SetGravitySpeed(const float4& _speed);
	bool IsGround();

	void PushRingToDrop(Ring* _ring);

	void SpringJump(float _power, eDirection _direction);
	void SpikeTrapped();
	void Attacked();
	void DropAllRings();

	void StopBGM();
	void StartBGM();
	void ChangeBGM(const std::string& _name);
	void ChangeBGMBossArea();

	void EnterBossArea();
	void ExitBossArea();
	void ExitBossAreaBGM();

	void StageClear();

private:
	// 초기화 수행
	void initInput();
	void initRendererAndAnimation();
	void initCollision();
	void initSoundPlayer();

private:
	void debugRenderOnOff();
	void processGravity(float _deltaTime);
	void mapCollisionSideCheck();
	void rollSideCheck();

	void setForwardRight();
	void setForwardLeft();

	void normalMove(float _deltaTime);
	void noLimitMove(float _deltaTime);

private:
#pragma region PlayerStateFunction
	void onWait(float _deltaTime);
	void onDelay(float _deltaTime);

	void onIdleLeft(float _deltaTime);
	void onIdleRight(float _deltaTime);

	void onBoringLeft(float _deltaTime);
	void onBoringRight(float _deltaTime);

	void onWalkLeft(float _deltaTime);
	void onWalkRight(float _deltaTime);

	void onWalkLeftAngled(float _deltaTime);
	void onWalkRightAngled(float _deltaTime);

	void onFullSpeedLeft(float _deltaTime);
	void onFullSpeedRight(float _deltaTime);

	void onFullSpeedLeftAngled(float _deltaTime);
	void onFullSpeedRightAngled(float _deltaTime);

	void onRollingLeft(float _deltaTime);
	void onRollingRight(float _deltaTime);
	void onRollingUp(float _deltaTime);

	void onJumpLeft(float _deltaTime);
	void onJumpRight(float _deltaTime);

	void onJumpShieldLeft(float _deltaTime);
	void onJumpShieldRight(float _deltaTime);

	void onJumpSpringLeft(float _deltaTime);
	void onJumpStringRight(float _deltaTime);

	void onSkidLeft(float _deltaTime);
	void onSkidRight(float _deltaTime);

	void onPushLeft(float _deltaTime);
	void onPushRight(float _deltaTime);

	void onFallingLeft(float _deltaTime);
	void onFallingRight(float _deltaTime);
	void onFallingUp(float _deltaTime);

	void onJumpBoard(float _deltaTime);
	void onCrouchLeft(float _deltaTime);
	void onCrouchRight(float _deltaTime);

	void onLookUpLeft(float _deltaTime);
	void onLookUpRight(float _deltaTime);

	void onSpinLeft(float _deltaTime);
	void onSpinRight(float _deltaTime);

	void onSpinDashLeft(float _deltaTime);
	void onSpinDashRight(float _deltaTime);

	void onWoodRunning(float _deltaTime);

	void onSpringJumpLeft(float _deltaTime);
	void onSpringJumpRight(float _deltaTime);

	void onDamagedLeft(float _deltaTime);
	void onDamagedRight(float _deltaTime);

	void onStageClear(float _deltaTime);

#pragma endregion

private:
#pragma region Constant
	const float NORMAL_SPEED_LIMIT = 500.f;
	const float HYPER_SPEED_LIMIT = 800.f;
	const float NORMAL_ACCELATION = 200.f;
	const float HYPER_ACCELATION = 280.f;
	const float FULL_SPEED_CHANGE = 400.f;
	const float JUMP_POWER = 300.f;
	const float JUMP_TIME = 0.4f;
	const float BLINK_COOLTIME = 0.05f;

	const float GRAVITY_POWER = 800.f;
#pragma endregion
private:
	std::unordered_map<ePlayerState, const char*> stateNameMap_;

private:
	//-----------------------------------
	// 이동
	//-----------------------------------
	float4 direction_;
	float4 upVector_;
	float4 forwardVector_;

	float accelation_;
	float4 gravityAccelation_;

	float4 playerSpeed_;
	float4 gravitySpeed_;
	float4 actualSpeed_;

	float fActualSpeedLimit_;
	float jumpTime_;

	bool bCanJump_;
	bool bGround_;
	bool bLeftAnglePossible_;
	bool bRightAnglePossible_;
	bool bAngled_;
	bool bSpin_;

	bool bRollRight_;
	bool bRollLeft_;
	bool bRollUp_;
	bool bRolling_; // 이건 소닉 이 도는 것이 아닌 원돌이를 의미
	
	float rotation_;
	float lerpRatio_;
	float woodRunTime_;

	//-----------------------------------
	// 스킬
	//-----------------------------------
	std::list<Ring*> electricEffectRingList_;
	std::vector<Ring*> ringsReadyToDrop_;
	eMonitor skillType_;
	PlayerSkill* skill_;
	float4 skillPosition_;

	bool bUsingBubbleSkill_;
	bool bUsingElectricSkill_;
	bool bUsingFireSkill_;

	bool bLeftRollStartForSkillLocation_;

	//-----------------------------------
	// 상태
	//-----------------------------------
	ePlayerState playerState_;
	ePlayerState nextState_;
	float currentStateTime_;
	float delayForNextState_;

	float blinkCooltime_;
	float blinkTime_;
	float invincibleTime_;
	bool bInvincible_;
	bool bBlink_;

	//-----------------------------------
	// 렌더러
	//-----------------------------------
	RSRenderer* mainRender_;
	bool bDebugRender_;

	//-------------------------------------
	// 충돌체
	//-------------------------------------
	RSCollision* bottomCenterCollision_;
	RSCollision* bottomRightCollision_;
	RSCollision* bottomLeftCollision_;
	RSCollision* topCenterCollision_;
	RSCollision* centerLeftCollision_;
	RSCollision* centerRightCollision_;

	// 도는 중에 좌우 forward vector를 얻기 위한 충돌체
	RSCollision* bottomRightRollCollision_;
	RSCollision* bottomLeftRollCollision_;
	
	RSCollision* bottomCenterRollRightCollision_;
	RSCollision* bottomCenterRollLeftCollision_;
	RSCollision* bottomCenterRollPostCollision_;
	RSCollision* bottomCenterRollUpCollision_;
	RSCollision* rollCheckSideCollision_;
	RSCollision* groundCheckRollCollision_;
	RSCollision* groundCheckCollision_;

	RSCollision* jumpBoardCheckCollision_;
	RSCollision* woodRunCheckCollision_;
	RSCollision* woodRunFinishCheckCollision_;

	RSCollision* electricFieldCollision_;

	//-------------------------------------
	// 사운드
	//-------------------------------------
	RSSoundPlayer* bgmPlayer_;
	RSSoundPlayer* jumpSound_;
	RSSoundPlayer* rollingSound_;
	RSSoundPlayer* spinSound_;
	RSSoundPlayer* spinDashSound_;

	std::list<RSRenderer*> effectList_;

	bool bBossArea_;
	bool bBossAreaBGM_;
};


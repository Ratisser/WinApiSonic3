#pragma once

#include <RSActor.h>

class Player;
class PlayerSkillBackface;
class PlayerSkill : public RSActor
{
	friend Player;
public:
	PlayerSkill();
	~PlayerSkill();

	PlayerSkill(const PlayerSkill& _other) = delete;
	PlayerSkill(PlayerSkill&& _other) = delete;

	PlayerSkill& operator=(const PlayerSkill& _other) = delete;
	PlayerSkill& operator=(const PlayerSkill&& _other) = delete;

	virtual void Start() override;
	virtual void UpdateBefore() override;
	virtual void Update() override;
	virtual void UpdateAfter() override;
	virtual void Render() override;
	virtual void Cleanup() override;

	void SetPlayer(Player* _player);
	void SetBackface(PlayerSkillBackface* _backface);

private:
	void bubbleShield();
	void electricShield();
	void fireShield();

	void bubbleSkillOn();
	void bubbleSkillOff();

	void electricSkillOn();

	void fireSkillLeftOn();
	void fireSkillRightOn();

	void electricShieldAnimationUpdate();

	void ShieldOff();

private:
	Player* player_;
	PlayerSkillBackface* backface_;
	RSRenderer* bubbleRender_;
	RSRenderer* electricRender_;
	RSRenderer* bubbleSkillRender_;
	RSRenderer* fireRenderForward_;
	RSRenderer* fireSkillRender_;

	float blinkTimer_;
	bool bBlink_;

	int electricAnimCount_;
	int fireAnimCount_;

	bool bFireSkillCasting_;
	bool bElectricSkillCasting_;
	bool bBubbleSkillCasting_;
};


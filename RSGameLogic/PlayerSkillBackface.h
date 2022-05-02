#pragma once

#include <RSActor.h>

class PlayerSkill;
class PlayerSkillBackface : public RSActor
{
	friend PlayerSkill;
public:
	PlayerSkillBackface();
	~PlayerSkillBackface();

	PlayerSkillBackface(const PlayerSkillBackface& _other) = delete;
	PlayerSkillBackface(PlayerSkillBackface&& _other) = delete;

	PlayerSkillBackface& operator=(const PlayerSkillBackface& _other) = delete;
	PlayerSkillBackface& operator=(const PlayerSkillBackface&& _other) = delete;

	virtual void Start() override;
	virtual void UpdateBefore() override;
	virtual void Update() override;
	virtual void UpdateAfter() override;
	virtual void Render() override;
	virtual void Cleanup() override;

private:
	RSRenderer* fireRenderBackward_;
	RSRenderer* electricSkillRender_;
};


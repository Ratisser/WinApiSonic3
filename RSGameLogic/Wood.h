#pragma once

#include <RSActor.h>

class Wood : public RSActor
{
public:
	Wood();
	~Wood();

	Wood(const Wood& _other) = delete;
	Wood(Wood&& _other) = delete;

	Wood& operator=(const Wood& _other) = delete;
	Wood& operator=(const Wood&& _other) = delete;

	virtual void Start() override;
	virtual void UpdateBefore() override;
	virtual void Update() override;
	virtual void UpdateAfter() override;
	virtual void Render() override;
	virtual void Cleanup() override;

	void WoodRun();

private:
	enum class eWoodState { IDLE, WOOD_RUN, MOVE, END };
	RSRenderer* mainRender_;
	RSRenderer* skinRender_;

	eWoodState state_;
};


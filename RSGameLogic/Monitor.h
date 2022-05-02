#pragma once

#include <RSActor.h>

#include "eMonitor.h"

class RSRenderer;
class RSCollision;
class RSSoundPlayer;
class EditLevelController;
class Monitor : public RSActor
{
	friend EditLevelController;
public:
	Monitor();
	virtual ~Monitor();

	Monitor(const Monitor& _other) = delete;
	Monitor(Monitor&& _other) = delete;

	Monitor& operator=(const Monitor& _other) = delete;
	Monitor& operator=(const Monitor&& _other) = delete;

	virtual void Start() override;
	virtual void UpdateBefore() override;
	virtual void Update() override;
	virtual void UpdateAfter() override;
	virtual void Render() override;
	virtual void Cleanup() override;

	void Break();
	void Pop();
	void SetMonitorType(eMonitor _monitorType);
	bool IsActivate();
	eMonitor GetMonitorEffect();

protected:
	enum class eMonitorState { ACTIVATE = 0, POP = 1, BREAK = 2, DONE = 3, EDIT};
	RSRenderer* mainRender_;
	RSRenderer* popRender_;
	RSCollision* collision_;
	RSSoundPlayer* popSound_;
	RSSoundPlayer* fireSound_;
	RSSoundPlayer* electricSound_;
	RSSoundPlayer* bubbleSound_;
	RSSoundPlayer* lifeSound_;
	eMonitorState state_;

	eMonitor monitorType_;
};


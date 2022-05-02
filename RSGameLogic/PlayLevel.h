#pragma once

#include <RSLevel.h>
#include <RSMath.h>

#include "eMonitor.h"

class Ring;
class RSSoundPlayer;
class PlayLevel : public RSLevel
{
public:
	PlayLevel();
	~PlayLevel();

	PlayLevel(const PlayLevel& _other) = delete;
	PlayLevel(PlayLevel&& _other) = delete;

	PlayLevel& operator=(const PlayLevel& _other) = delete;
	PlayLevel& operator=(const PlayLevel&& _other) = delete;


public:
	virtual void LevelUpdateBefore() override;
	virtual void LevelUpdate() override;
	virtual void LevelUpdateAfter() override;

protected:
	virtual void loading() override;

private:
	void loadRingAndMonitorFromFile();
	void loadSpring();
	void loadSpike();
	void addMonitor(eMonitor _monitorType, float _x, float _y);
};


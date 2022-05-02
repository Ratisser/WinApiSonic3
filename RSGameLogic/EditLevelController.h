#pragma once

#include <RSActor.h>
#include <list>

class RSRenderer;
class RSCollision;
class Ring;
class Monitor;
class EditLevelController : public RSActor
{
public:
	EditLevelController();
	~EditLevelController();

	EditLevelController(const EditLevelController& _other) = delete;
	EditLevelController(EditLevelController&& _other) = delete;

	EditLevelController& operator=(const EditLevelController& _other) = delete;
	EditLevelController& operator=(const EditLevelController&& _other) = delete;

	virtual void Start() override;
	virtual void UpdateBefore() override;
	virtual void Update() override;
	virtual void UpdateAfter() override;
	virtual void Render() override;
	virtual void Cleanup() override;

	void SaveFile();
	void LoadFile();

private:
	void ringUpdateBefore();
	void ringUpdate();
	void ringUpdateAfter();
	void ringRender();

	void monitorUpdateBefore();
	void monitorUpdate();
	void monitorUpdateAfter();
	void monitorRender();

private:
	enum class eEditMode { RING_EDIT, MONITOR_EDIT };
	RSRenderer* cursorRenderer_;
	RSCollision* cursorCollision_;
	eEditMode mode_;

	std::list<Ring*> ringList_;
	std::list<Monitor*> monitorList_;

	Ring* focusedRing_;
	Monitor* focusedMonitor_;
};


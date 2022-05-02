#pragma once

#include <RSActor.h>

class PlayLevelOpening : public RSActor
{
public:
	PlayLevelOpening();
	~PlayLevelOpening();

	PlayLevelOpening(const PlayLevelOpening& _other) = delete;
	PlayLevelOpening(PlayLevelOpening&& _other) = delete;

	PlayLevelOpening& operator=(const PlayLevelOpening& _other) = delete;
	PlayLevelOpening& operator=(const PlayLevelOpening&& _other) = delete;




	// RSActor을(를) 통해 상속됨
	virtual void Start() override;

	virtual void UpdateBefore() override;

	virtual void Update() override;

	virtual void UpdateAfter() override;

	virtual void Render() override;

	virtual void Cleanup() override;

private:
	RSRenderer* openingRender_;
};


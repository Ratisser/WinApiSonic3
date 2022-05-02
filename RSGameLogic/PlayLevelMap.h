#pragma once

#include <RSActor.h>

class RSRenderer;
class RSCollision;
class PlayLevelMap : public RSActor
{
public:
	PlayLevelMap();
	~PlayLevelMap();

	PlayLevelMap(const PlayLevelMap& _other) = delete;
	PlayLevelMap(PlayLevelMap&& _other) = delete;

	PlayLevelMap& operator=(const PlayLevelMap& _other) = delete;
	PlayLevelMap& operator=(const PlayLevelMap&& _other) = delete;

	virtual void Start() override;
	virtual void UpdateBefore() override;
	virtual void Update() override;
	virtual void UpdateAfter() override;
	virtual void Render() override;
	virtual void Cleanup() override;

private:
	RSRenderer* mainRender_;
	RSRenderer* mapCollisionRender_;
	RSRenderer* rollLeftRender_;
	RSRenderer* rollRightRender_;
	RSRenderer* rollUpRender_;



	RSCollision* mapCollision_;
	RSCollision* rollLeftCollision_;
	RSCollision* rollRightCollision_;
	RSCollision* rollUpCollision_;

	bool bShowCollision_;
};


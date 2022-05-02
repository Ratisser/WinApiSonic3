#pragma once

#include <RSActor.h>

class RSRenderer;
class PlayLevelSky : public RSActor
{
public:
	PlayLevelSky();
	~PlayLevelSky();

	PlayLevelSky(const PlayLevelSky& _other) = delete;
	PlayLevelSky(PlayLevelSky&& _other) = delete;

	PlayLevelSky& operator=(const PlayLevelSky& _other) = delete;
	PlayLevelSky& operator=(const PlayLevelSky&& _other) = delete;

	virtual void Start() override;
	virtual void UpdateBefore() override;
	virtual void Update() override;
	virtual void UpdateAfter() override;
	virtual void Render() override;
	virtual void Cleanup() override;

private:
	void moveCloud(RSRenderer* _cloud, float _speed, float _deltaTime);

private:
	RSRenderer* cloud00_;
	RSRenderer* cloud01_;
	RSRenderer* cloud10_;
	RSRenderer* cloud11_;
	RSRenderer* cloud20_;
	RSRenderer* cloud21_;
	RSRenderer* cloud30_;
	RSRenderer* cloud31_;
	RSRenderer* cloud40_;
	RSRenderer* cloud41_;
	RSRenderer* cloud50_;
	RSRenderer* cloud51_;
};
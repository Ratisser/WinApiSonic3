#pragma once

#include <RSActor.h>

class SonicSeaWave : public RSActor
{
public:
	SonicSeaWave(); 
	~SonicSeaWave();

	SonicSeaWave(const SonicSeaWave& _other) = delete; 
	SonicSeaWave(SonicSeaWave&& _other) = delete; 

	SonicSeaWave& operator=(const SonicSeaWave& _other) = delete;
	SonicSeaWave& operator=(const SonicSeaWave&& _other) = delete;

public:
	virtual void Start() override;
	virtual void UpdateBefore() override;
	virtual void Update() override;
	virtual void UpdateAfter() override;
	virtual void Render() override;
	virtual void Cleanup() override;

private:
	RSRenderer* renderer_;

	bool bOn_;
};


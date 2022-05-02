#pragma once

#include <RSActor.h>

class RSRenderer;
class RSCollision;
class BlindMap : public RSActor
{
public:
	BlindMap();
	~BlindMap();

	BlindMap(const BlindMap& _other) = delete;
	BlindMap(BlindMap&& _other) = delete;

	BlindMap& operator=(const BlindMap& _other) = delete;
	BlindMap& operator=(const BlindMap&& _other) = delete;

	// RSActor을(를) 통해 상속됨
	virtual void Start() override;
	virtual void UpdateBefore() override;
	virtual void Update() override;
	virtual void UpdateAfter() override;
	virtual void Render() override;
	virtual void Cleanup() override;

private:
	RSRenderer* mainRender_;
};


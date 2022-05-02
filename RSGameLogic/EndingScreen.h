#pragma once

#include <RSActor.h>

class EndingScreen : public RSActor
{
public:
	EndingScreen();
	~EndingScreen();

	EndingScreen(const EndingScreen& _other) = delete;
	EndingScreen(EndingScreen&& _other) = delete;

	EndingScreen& operator=(const EndingScreen& _other) = delete;
	EndingScreen& operator=(const EndingScreen&& _other) = delete;

	virtual void Start() override;
	virtual void UpdateBefore() override;
	virtual void Update() override;
	virtual void UpdateAfter() override;
	virtual void Render() override;
	virtual void Cleanup() override;

private:
	RSRenderer* mainRender_;
};


#pragma once

#include <RSActor.h>
#include <vector>

class RSRenderer;
class PlayLevelBackground : public RSActor
{
public:
	PlayLevelBackground();
	~PlayLevelBackground();

	PlayLevelBackground(const PlayLevelBackground& _other) = delete;
	PlayLevelBackground(PlayLevelBackground&& _other) = delete;

	PlayLevelBackground& operator=(const PlayLevelBackground& _other) = delete;
	PlayLevelBackground& operator=(const PlayLevelBackground&& _other) = delete;

	virtual void Start() override;
	virtual void UpdateBefore() override;
	virtual void Update() override;
	virtual void UpdateAfter() override;
	virtual void Render() override;
	virtual void Cleanup() override;

private:
	enum { LEVEL2_COUNT = 10 };
	RSRenderer* backLevel0_;
	RSRenderer* backLevel1_0_;
	RSRenderer* backLevel1_1_;
	std::vector<RSRenderer*> backLevel2_;
	std::vector<RSRenderer*> backLevel3_;
	std::vector<RSRenderer*> backLevel4_;
	std::vector<RSRenderer*> backLevel5_;
	std::vector<RSRenderer*> backLevel6_;
	RSRenderer* backLevel7_;
};


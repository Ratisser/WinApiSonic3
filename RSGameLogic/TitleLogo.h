#pragma once

#include <RSActor.h>
#include <RSMath.h>

class RSRenderer;
class RSSoundPlayer;
class TitleLogo : public RSActor
{
	enum class eTitleState
	{
		SEGA_LOGO,
		SONIC_APPEAR,
		LABEL_APPEAR,
		SELECT_MENU
	};

	enum class eTitleMenu
	{
		PLAY = 0,
		COMPETITION = 1
	};

public:
	TitleLogo();
	~TitleLogo();

	TitleLogo(const TitleLogo& _other) = delete;
	TitleLogo(TitleLogo&& _other) = delete;

	TitleLogo& operator=(const TitleLogo& _other) = delete;
	TitleLogo& operator=(const TitleLogo&& _other) = delete;

	virtual void Start() override;
	virtual void UpdateBefore() override;
	virtual void Update() override;
	virtual void UpdateAfter() override;
	virtual void Render() override;
	virtual void Cleanup() override;

private:
	void segaLogo();
	void sonicAppear();
	void labelAppear();
	void selectMenu();

private:
	RSRenderer* title_;
	RSRenderer* titleSonic_;
	RSRenderer* titleAircraft_;
	RSRenderer* titleLabel_;
	RSRenderer* titleUI_;
	RSRenderer* titleBlinder_;

	float fTitleLabelAnimTime_;
	float fTitleLabelAnimYFactor_;
	eTitleState eCurrentState_;
	eTitleMenu eCurrentMenu_;

	float4 aircraftDirection_;

	RSSoundPlayer* bgmPlayer_;

};


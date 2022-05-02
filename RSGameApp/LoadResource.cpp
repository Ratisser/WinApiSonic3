#include "LoadResource.h"

#include <RSSoundManager.h>
#include <RSResourceManager.h>
#include <RSImage.h>
#include <RSLevelManager.h>

#include <TitleLevel.h>
#include <PlayLevel.h>
#include <EndingLevel.h>
#include <EditLevel.h>

void LoadResource()
{
	// sound
	RSSoundManager::GetInstance().Initialize();
	RSSoundManager::GetInstance().CreateSound("title.mid", "sound\\title.mid");
	RSSoundManager::GetInstance().CreateSound("stage1-1.mid", "sound\\stage1-1.mid", true);
	RSSoundManager::GetInstance().CreateSound("boss.mid", "sound\\boss.mid");
	RSSoundManager::GetInstance().CreateSound("stageClear.mid", "sound\\stageClear.mid");
	RSSoundManager::GetInstance().CreateSound("ending2.mid", "sound\\ending2.mid");
	RSSoundManager::GetInstance().CreateSound("knuckles.mid", "sound\\knuckles.mid");

	RSSoundManager::GetInstance().CreateSound("jump.wav", "sound\\effect\\jump.wav");
	RSSoundManager::GetInstance().CreateSound("spin.wav", "sound\\effect\\spin.wav");
	RSSoundManager::GetInstance().CreateSound("rolling.wav", "sound\\effect\\rolling.wav");
	RSSoundManager::GetInstance().CreateSound("spinDash.wav", "sound\\effect\\spinDash.wav");
	RSSoundManager::GetInstance().CreateSound("ring.wav", "sound\\effect\\ring.wav");
	RSSoundManager::GetInstance().CreateSound("pop.wav", "sound\\effect\\pop.wav");
	RSSoundManager::GetInstance().CreateSound("spring.wav", "sound\\effect\\spring.wav");
	RSSoundManager::GetInstance().CreateSound("sting.wav", "sound\\effect\\sting.wav");
	RSSoundManager::GetInstance().CreateSound("skid.wav", "sound\\effect\\skid.wav");
	RSSoundManager::GetInstance().CreateSound("ringDrop.wav", "sound\\effect\\ringDrop.wav");
	RSSoundManager::GetInstance().CreateSound("attacked.wav", "sound\\effect\\attacked.wav");
	RSSoundManager::GetInstance().CreateSound("machineAttacked.wav", "sound\\effect\\machineAttacked.wav");

	RSSoundManager::GetInstance().CreateSound("click.wav", "sound\\effect\\click.wav");
	RSSoundManager::GetInstance().CreateSound("cashout.wav", "sound\\effect\\cashout.wav");
	RSSoundManager::GetInstance().CreateSound("goalPostDrop.wav", "sound\\effect\\goalPostDrop.wav");
	RSSoundManager::GetInstance().CreateSound("grab.wav", "sound\\effect\\grab.wav");

	RSSoundManager::GetInstance().CreateSound("electricShield.wav", "sound\\effect\\electricShield.wav");
	RSSoundManager::GetInstance().CreateSound("electricShieldSkill.wav", "sound\\effect\\electricShieldSkill.wav");
	RSSoundManager::GetInstance().CreateSound("fireShield.wav", "sound\\effect\\fireShield.wav");
	RSSoundManager::GetInstance().CreateSound("fireShieldSkill.wav", "sound\\effect\\fireShieldSkill.wav");
	RSSoundManager::GetInstance().CreateSound("bubbleShield.wav", "sound\\effect\\bubbleShield.wav");
	RSSoundManager::GetInstance().CreateSound("bubbleShieldSkill.wav", "sound\\effect\\bubbleShieldSkill.wav");
	RSSoundManager::GetInstance().CreateSound("life.mid", "sound\\effect\\life.mid");

	// titleLevel
	RSResourceManager::GetInstance().LoadImageFromFile("title.bmp", "image\\title.bmp");
	RSResourceManager::GetInstance().LoadImageFromFile("titleAircraftRight.bmp", "image\\titleAircraftRight.bmp");
	RSResourceManager::GetInstance().LoadImageFromFile("titleAircraftLeft.bmp", "image\\titleAircraftLeft.bmp");
	RSResourceManager::GetInstance().LoadImageFromFile("titleSonic.bmp", "image\\titleSonic.bmp");
	RSResourceManager::GetInstance().LoadImageFromFile("titleBlinder.bmp", "image\\titleBlinder.bmp");

	RSResourceManager::GetInstance().FindImage("title.bmp")->CutImage(float4(400.f, 400.f));
	RSResourceManager::GetInstance().FindImage("titleAircraftRight.bmp")->CutImage(float4(48.f, 24.f));
	RSResourceManager::GetInstance().FindImage("titleAircraftLeft.bmp")->CutImage(float4(48.f, 24.f));
	RSResourceManager::GetInstance().FindImage("titleSonic.bmp")->CutImage(float4(320.f, 240.f));

	// PlayLevel
	RSResourceManager::GetInstance().LoadImageFromFile("sonicRight.bmp", "image\\sonicRight.bmp");
	RSResourceManager::GetInstance().LoadImageFromFile("sonicRight_mask.bmp", "image\\sonicRight_mask.bmp");
	RSResourceManager::GetInstance().LoadImageFromFile("sonicLeft.bmp", "image\\sonicLeft.bmp");
	RSResourceManager::GetInstance().LoadImageFromFile("sonicLeft_mask.bmp", "image\\sonicLeft_mask.bmp");

	RSResourceManager::GetInstance().LoadImageFromFile("stage1-1_dry.bmp", "image\\stage1-1_dry.bmp");
	RSResourceManager::GetInstance().LoadImageFromFile("stage1-1_collision.bmp", "image\\stage1-1_collision.bmp");
	RSResourceManager::GetInstance().LoadImageFromFile("stage1-1_hide.bmp", "image\\stage1-1_hide.bmp");
	RSResourceManager::GetInstance().LoadImageFromFile("stage1-1_rollLeft.bmp", "image\\stage1-1_rollLeft.bmp");
	RSResourceManager::GetInstance().LoadImageFromFile("stage1-1_rollRight.bmp", "image\\stage1-1_rollRight.bmp");
	RSResourceManager::GetInstance().LoadImageFromFile("stage1-1_rollUp.bmp", "image\\stage1-1_rollUp.bmp");
	RSResourceManager::GetInstance().LoadImageFromFile("stage1-1_background1.bmp", "image\\stage1-1_background1.bmp");
	RSResourceManager::GetInstance().LoadImageFromFile("stage1-1_background2.bmp", "image\\stage1-1_background2.bmp");
	RSResourceManager::GetInstance().LoadImageFromFile("stage1-1_background3.bmp", "image\\stage1-1_background3.bmp");
	RSResourceManager::GetInstance().LoadImageFromFile("stage1-1_wood.bmp", "image\\stage1-1_wood.bmp");
	RSResourceManager::GetInstance().LoadImageFromFile("stage1-1_opening.bmp", "image\\stage1-1_opening.bmp");

	RSResourceManager::GetInstance().LoadImageFromFile("ring.bmp", "image\\ring.bmp");
	RSResourceManager::GetInstance().LoadImageFromFile("ui.bmp", "image\\ui.bmp");
	RSResourceManager::GetInstance().LoadImageFromFile("monitors.bmp", "image\\monitors.bmp");
	RSResourceManager::GetInstance().LoadImageFromFile("pop.bmp", "image\\pop.bmp");
	RSResourceManager::GetInstance().LoadImageFromFile("skidEffect.bmp", "image\\skidEffect.bmp");
	RSResourceManager::GetInstance().LoadImageFromFile("spike.bmp", "image\\spike.bmp");
	RSResourceManager::GetInstance().LoadImageFromFile("spring.bmp", "image\\spring.bmp");

	RSResourceManager::GetInstance().LoadImageFromFile("bubble.bmp", "image\\bubble.bmp");
	RSResourceManager::GetInstance().LoadImageFromFile("bubble2.bmp", "image\\bubble2.bmp");
	RSResourceManager::GetInstance().LoadImageFromFile("electric.bmp", "image\\electric.bmp");
	RSResourceManager::GetInstance().LoadImageFromFile("electricSkill.bmp", "image\\electricSkill.bmp");
	RSResourceManager::GetInstance().LoadImageFromFile("fire.bmp", "image\\fire.bmp");
	RSResourceManager::GetInstance().LoadImageFromFile("fireSkillLeft.bmp", "image\\fireSkillLeft.bmp");
	RSResourceManager::GetInstance().LoadImageFromFile("fireSkillRight.bmp", "image\\fireSkillRight.bmp");

	RSResourceManager::GetInstance().LoadImageFromFile("middleBossLeft.bmp", "image\\middleBossLeft.bmp");
	RSResourceManager::GetInstance().LoadImageFromFile("middleBossRight.bmp", "image\\middleBossRight.bmp");
	RSResourceManager::GetInstance().LoadImageFromFile("flameLeft.bmp", "image\\flameLeft.bmp");
	RSResourceManager::GetInstance().LoadImageFromFile("flameRight.bmp", "image\\flameRight.bmp");
	
	RSResourceManager::GetInstance().LoadImageFromFile("goalPost.bmp", "image\\goalPost.bmp");

	RSResourceManager::GetInstance().LoadImageFromFile("stage1-1_title.bmp", "image\\stage1-1_title.bmp");
	RSResourceManager::GetInstance().LoadImageFromFile("stage1-1_end.bmp", "image\\stage1-1_end.bmp");

	RSResourceManager::GetInstance().LoadImageFromFile("aircraft.bmp", "image\\aircraft.bmp");
	RSResourceManager::GetInstance().LoadImageFromFile("knucklesRight.bmp", "image\\knucklesRight.bmp");
	RSResourceManager::GetInstance().LoadImageFromFile("knucklesLeft.bmp", "image\\knucklesLeft.bmp");
	RSResourceManager::GetInstance().LoadImageFromFile("splash.bmp", "image\\splash.bmp");

	// EditLevel
	RSResourceManager::GetInstance().LoadImageFromFile("cursor.bmp", "image\\cursor.bmp");

	// EndingLevel
	RSResourceManager::GetInstance().LoadImageFromFile("ending.bmp", "image\\ending.bmp");

	// Cut image
	RSResourceManager::GetInstance().FindImage("sonicRight.bmp")->CutImage(float4(128.0f, 128.0f));
	RSResourceManager::GetInstance().FindImage("sonicLeft.bmp")->CutImage(float4(128.0f, 128.0f));
	RSResourceManager::GetInstance().FindImage("stage1-1_background1.bmp")->CutImage(float4(512.0f, 512.0f));
	RSResourceManager::GetInstance().FindImage("stage1-1_wood.bmp")->CutImage(float4(256.f, 512.0f));
	RSResourceManager::GetInstance().FindImage("ui.bmp")->CutImage(float4(40.f, 20.f));
	RSResourceManager::GetInstance().FindImage("monitors.bmp")->CutImage(float4(30.f, 34.f));
	RSResourceManager::GetInstance().FindImage("ring.bmp")->CutImage(float4(16.f, 16.f));
	RSResourceManager::GetInstance().FindImage("pop.bmp")->CutImage(float4(32.f, 32.f));
	RSResourceManager::GetInstance().FindImage("skidEffect.bmp")->CutImage(float4(16.f, 16.f));
	RSResourceManager::GetInstance().FindImage("spike.bmp")->CutImage(float4(32.f, 32.f));
	RSResourceManager::GetInstance().FindImage("spring.bmp")->CutImage(float4(32.f, 32.f));

	RSResourceManager::GetInstance().FindImage("bubble.bmp")->CutImage(float4(128.f, 128.f));
	RSResourceManager::GetInstance().FindImage("bubble2.bmp")->CutImage(float4(48.f, 48.f));
	RSResourceManager::GetInstance().FindImage("electric.bmp")->CutImage(float4(48.f, 48.f));
	RSResourceManager::GetInstance().FindImage("electricSkill.bmp")->CutImage(float4(64.f, 64.f));
	RSResourceManager::GetInstance().FindImage("fire.bmp")->CutImage(float4(48.f, 48.f));
	RSResourceManager::GetInstance().FindImage("fireSkillLeft.bmp")->CutImage(float4(64.f, 48.f));
	RSResourceManager::GetInstance().FindImage("fireSkillRight.bmp")->CutImage(float4(64.f, 48.f));

	RSResourceManager::GetInstance().FindImage("middleBossLeft.bmp")->CutImage(float4(96.f, 96.f));
	RSResourceManager::GetInstance().FindImage("middleBossRight.bmp")->CutImage(float4(96.f, 96.f));
	RSResourceManager::GetInstance().FindImage("flameLeft.bmp")->CutImage(float4(32.f, 32.f));
	RSResourceManager::GetInstance().FindImage("flameRight.bmp")->CutImage(float4(32.f, 32.f));

	RSResourceManager::GetInstance().FindImage("goalPost.bmp")->CutImage(float4(48.f, 48.f));

	RSResourceManager::GetInstance().FindImage("stage1-1_title.bmp")->CutImage(float4(320.f, 240.f));
	RSResourceManager::GetInstance().FindImage("stage1-1_end.bmp")->CutImage(float4(320.f, 240.f));

	RSResourceManager::GetInstance().FindImage("aircraft.bmp")->CutImage(float4(128.f, 128.f));
	RSResourceManager::GetInstance().FindImage("knucklesRight.bmp")->CutImage(float4(128.f, 128.f));
	RSResourceManager::GetInstance().FindImage("knucklesLeft.bmp")->CutImage(float4(128.f, 128.f));
	RSResourceManager::GetInstance().FindImage("splash.bmp")->CutImage(float4(96.f, 96.f));


	RSResourceManager::GetInstance().LoadImageFromFile("aircraft.bmp", "image\\aircraft.bmp");
	RSResourceManager::GetInstance().LoadImageFromFile("knucklesRight.bmp", "image\\knucklesRight.bmp");
	RSResourceManager::GetInstance().LoadImageFromFile("knucklesLeft.bmp", "image\\knucklesLeft.bmp");
	RSResourceManager::GetInstance().LoadImageFromFile("splash.bmp", "image\\splash.bmp");

	// 레벨 초기화
	RSLevelManager::GetInstance().CreateLevel<TitleLevel>("TitleLevel");
	RSLevelManager::GetInstance().CreateLevel<PlayLevel>("PlayLevel");
	RSLevelManager::GetInstance().CreateLevel<EndingLevel>("EndingLevel");
	RSLevelManager::GetInstance().CreateLevel<EditLevel>("EditLevel");

	// 시작 레벨을 여기서 정합니다.
	RSLevelManager::GetInstance().ChangeLevel("TitleLevel");
}
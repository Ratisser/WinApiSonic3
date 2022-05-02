#include <RSResourceManager.h>
#include <RSSoundManager.h>
#include <RSInput.h>
#include <RSMath.h>
#include <RSWindow.h>

#include "eDirection.h"

#include "PlayLevel.h"
#include "PlayLevelMap.h"
#include "Player.h"
#include "BlindMap.h"
#include "PlayLevelBackground.h"
#include "PlayLevelSky.h"
#include "PlayLevelUI.h"
#include "Ring.h"
#include "Monitor.h"
#include "PlayerSkill.h"
#include "PlayerSkillBackface.h"
#include "Wood.h"
#include "Spring.h"
#include "Spike.h"
#include "BossArea.h"
#include "FireBreath.h"
#include "GoalPost.h"
#include "PlayLevelOpening.h"
#include "StartLocation.h"
#include "OpeningController.h"


PlayLevel::PlayLevel()
{
}

PlayLevel::~PlayLevel()
{
}

void PlayLevel::LevelUpdateBefore()
{
}

void PlayLevel::LevelUpdate()
{
}

void PlayLevel::LevelUpdateAfter()
{
}

void PlayLevel::loading()
{
	Player* player = AddActor<Player>("Player");
	AddActor<PlayLevelMap>("PlayLevelMap");
	AddActor<BlindMap>("BlindMap");
	AddActor<PlayLevelBackground>("PlayLevelBackground");
	AddActor<Wood>("Wood");
	AddActor<PlayLevelSky>("PlayLevelSky");
	AddActor<PlayLevelOpening>("PlayLevelOpening");
	AddActor<PlayLevelUI>("PlayLevelUI");
	PlayerSkill* playerSkill = AddActor<PlayerSkill>("PlayerSkill");
	PlayerSkillBackface* backface = AddActor<PlayerSkillBackface>("PlayerSkillBackface");

	player->SetSkill(playerSkill);
	playerSkill->SetPlayer(player);
	playerSkill->SetBackface(backface);

	AddActor<BossArea>("BossArea");
	AddActor<StartLocation>("StartLocation");
	AddActor<FireBreath>("FireBreath");
	AddActor<GoalPost>("GoalPost");

	AddActor<OpeningController>("OpeningController");

	loadRingAndMonitorFromFile();
	loadSpring();
	loadSpike();

	SetCamSize(RSWindow::GetInstance().GetViewportSize());
}

void PlayLevel::loadRingAndMonitorFromFile()
{
	char buffer[256]{ 0, };
	GetCurrentDirectoryA(256, buffer);
	std::string path = buffer;
	if (-1 == path.find("Build"))
	{
		path += "\\..\\Build";
	}

	path += "\\";
	path += "sonic.dat";

	FILE* file = nullptr;

	if (0 != fopen_s(&file, path.c_str(), "rb"))
	{
		fopen_s(&file, path.c_str(), "wb");
		if (file != nullptr)
		{
			fclose(file);
		}
		return;
	}

	int listSize = 0;
	fread(&listSize, sizeof(int), 1, file);

	for (int i = 0; i < listSize; ++i)
	{
		int x = 0;
		int y = 0;

		fread(&x, sizeof(int), 1, file);
		fread(&y, sizeof(int), 1, file);

		Ring* newRing = AddActor<Ring>();
		newRing->SetLocation({ static_cast<float>(x), static_cast<float>(y) });
	}

	listSize = 0;
	fread(&listSize, sizeof(int), 1, file);
	for (int i = 0; i < listSize; ++i)
	{
		int x = 0;
		int y = 0;
		int type = 0;

		fread(&x, sizeof(int), 1, file);
		fread(&y, sizeof(int), 1, file);
		fread(&type, sizeof(int), 1, file);
		addMonitor(static_cast<eMonitor>(type), static_cast<float>(x), static_cast<float>(y));
	}

	fclose(file);
}

void PlayLevel::loadSpring()
{
	Spring* newSpring = AddActor<Spring>();
	newSpring->SetLocation({ 4457.f, 1097.f });
	newSpring->SetSpringDirection(eDirection::UP);

	newSpring = AddActor<Spring>();
	newSpring->SetLocation({ 4399.f, 940.f });
	newSpring->SetSpringDirection(eDirection::UP);

	newSpring = AddActor<Spring>();
	newSpring->SetLocation({ 4399.f, 940.f });
	newSpring->SetSpringDirection(eDirection::UP);

	newSpring = AddActor<Spring>();
	newSpring->SetLocation({ 4696.f, 1048.f });
	newSpring->SetSpringDirection(eDirection::LEFT);

	newSpring = AddActor<Spring>();
	newSpring->SetRenderOrder(101);
	newSpring->SetSpringPower(700.f);
	newSpring->SetLocation({ 8361.f, 1418.f });
	newSpring->SetSpringDirection(eDirection::UP);

	newSpring = AddActor<Spring>();
	newSpring->SetLocation({ 5913.f, 1208.f });
	newSpring->SetSpringDirection(eDirection::LEFT);

	newSpring = AddActor<Spring>();
	newSpring->SetLocation({ 6040.f, 1177.f });
	newSpring->SetSpringDirection(eDirection::LEFT);

	newSpring = AddActor<Spring>();
	newSpring->SetLocation({ 7703.f, 1336.f });
	newSpring->SetSpringDirection(eDirection::UP);

	newSpring = AddActor<Spring>();
	newSpring->SetLocation({ 8506.f, 1150.f });
	newSpring->SetSpringDirection(eDirection::RIGHT);

	newSpring = AddActor<Spring>();
	newSpring->SetLocation({ 8252.f, 762.f });
	newSpring->SetSpringDirection(eDirection::UP);

	newSpring = AddActor<Spring>();
	newSpring->SetLocation({ 4910.f, 880.f });
	newSpring->SetSpringDirection(eDirection::UP);
}

void PlayLevel::loadSpike()
{
	Spike* newSpike = AddActor<Spike>();
	newSpike->SetLocation({ 5225.f, 984.f });
	newSpike->SetSpikeDirection(eDirection::UP);

	newSpike = AddActor<Spike>();
	newSpike->SetLocation({ 5350.f, 984.f });
	newSpike->SetSpikeDirection(eDirection::UP);

	newSpike = AddActor<Spike>();
	newSpike->SetLocation({ 7352.f, 1400.f });
	newSpike->SetSpikeDirection(eDirection::UP);

	newSpike = AddActor<Spike>();
	newSpike->SetLocation({ 7384.f, 1400.f });
	newSpike->SetSpikeDirection(eDirection::UP);

	newSpike = AddActor<Spike>();
	newSpike->SetLocation({ 7416.f, 1400.f });
	newSpike->SetSpikeDirection(eDirection::UP);

	newSpike = AddActor<Spike>();
	newSpike->SetLocation({ 8438.f, 1115.f });
	newSpike->SetSpikeDirection(eDirection::UP);

	newSpike = AddActor<Spike>();
	newSpike->SetLocation({ 8332.f, 1025.f });
	newSpike->SetSpikeDirection(eDirection::DOWN);


}

void PlayLevel::addMonitor(eMonitor _monitorType, float _x, float _y)
{
	Monitor* monitor = AddActor<Monitor>();
	monitor->SetLocation({ _x, _y });
	monitor->SetMonitorType(_monitorType);
}
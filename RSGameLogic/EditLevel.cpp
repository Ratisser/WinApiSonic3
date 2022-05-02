#include "EditLevel.h"

#include <RSResourceManager.h>
#include <RSInput.h>
#include <RSMath.h>
#include <RSWindow.h>

#include "PlayLevelMap.h"
#include "PlayLevelBackground.h"
#include "PlayLevelSky.h"
#include "EditLevelController.h"
#include "Ring.h"
#include "PlayLevelOpening.h"

EditLevel::EditLevel()
{
}

EditLevel::~EditLevel()
{
}

void EditLevel::LevelUpdateBefore()
{
}

void EditLevel::LevelUpdate()
{
}

void EditLevel::LevelUpdateAfter()
{
}

void EditLevel::loading()
{
	AddActor<PlayLevelMap>("PlayLevelMap.h");
	AddActor<PlayLevelBackground>("PlayLevelBackground");
	AddActor<PlayLevelSky>("PlayLevelSky");
	AddActor<EditLevelController>("EditLevelController");
	AddActor<PlayLevelOpening>("PlayLevelOpening");

	SetCamSize(RSWindow::GetInstance().GetViewportSize());
}

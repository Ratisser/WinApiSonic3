#include "AppRelease.h"

#include <RSLevelManager.h>
#include <RSResourceManager.h>
#include <RSTimer.h>
#include <RSWindow.h>
#include <RSInput.h>
#include <RSSoundManager.h>

void AppRelease()
{
	RSLevelManager::GetInstance().Destroy();
	RSResourceManager::GetInstance().Destroy();
	RSSoundManager::GetInstance().Destroy();

	RSWindow::GetInstance().Destroy();
	RSTimer::GetInstance().Destroy();
	RSInput::GetInstance().Destroy();
}

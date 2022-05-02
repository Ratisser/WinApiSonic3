#include "EndingLevel.h"

#include "EndingScreen.h"

EndingLevel::EndingLevel()
{
}

EndingLevel::~EndingLevel()
{
}

void EndingLevel::LevelUpdateBefore()
{
}

void EndingLevel::LevelUpdate()
{
}

void EndingLevel::LevelUpdateAfter()
{
}

void EndingLevel::loading()
{
	AddActor<EndingScreen>();
}

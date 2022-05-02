#include "TitleLevel.h"
#include "TitleLogo.h"

#include <RSResourceManager.h>
#include <RSInput.h>

TitleLevel::TitleLevel()
{
}

TitleLevel::~TitleLevel()
{
}

void TitleLevel::LevelUpdateBefore()
{
}

void TitleLevel::LevelUpdate()
{
}

void TitleLevel::LevelUpdateAfter()
{
}

void TitleLevel::loading()
{
	AddActor<TitleLogo>("titleLogo");
}


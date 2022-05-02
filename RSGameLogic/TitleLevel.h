#pragma once

#include <RSLevel.h>

class TitleLevel : public RSLevel
{
public:
	TitleLevel();
	~TitleLevel();

	TitleLevel(const TitleLevel& _other) = delete;
	TitleLevel(TitleLevel&& _other) = delete;

	TitleLevel& operator=(const TitleLevel& _other) = delete;
	TitleLevel& operator=(const TitleLevel&& _other) = delete;

public:
	virtual void LevelUpdateBefore() override;

	virtual void LevelUpdate() override;

	virtual void LevelUpdateAfter() override;

protected:
	virtual void loading() override;
};


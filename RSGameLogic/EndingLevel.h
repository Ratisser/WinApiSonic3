#pragma once

#include <RSLevel.h>


class RSRenderer;
class EndingLevel : public RSLevel
{
public:
	EndingLevel();
	~EndingLevel();

	EndingLevel(const EndingLevel& _other) = delete;
	EndingLevel(EndingLevel&& _other) = delete;

	EndingLevel& operator=(const EndingLevel& _other) = delete;
	EndingLevel& operator=(const EndingLevel&& _other) = delete;

public:
	virtual void LevelUpdateBefore() override;

	virtual void LevelUpdate() override;

	virtual void LevelUpdateAfter() override;

protected:
	virtual void loading() override;

private:
};


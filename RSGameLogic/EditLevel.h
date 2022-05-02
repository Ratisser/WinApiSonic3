#pragma once

#include <RSLevel.h>

class EditLevel : public RSLevel
{
public:
	EditLevel();
	~EditLevel();

	EditLevel(const EditLevel& _other) = delete;
	EditLevel(EditLevel&& _other) = delete;

	EditLevel& operator=(const EditLevel& _other) = delete;
	EditLevel& operator=(const EditLevel&& _other) = delete;

	// RSLevel��(��) ���� ��ӵ�
	virtual void LevelUpdateBefore() override;

	virtual void LevelUpdate() override;

	virtual void LevelUpdateAfter() override;

	virtual void loading() override;

private:

};


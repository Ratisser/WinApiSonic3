#pragma once

#include <Windows.h>

enum class eCollisionCheckColor
{
	MAP = RGB(0, 0, 255),
	JUMP_BOARD = RGB(255, 255, 255),
	ROLL_RIGHT = RGB(255, 0, 0),
	ROLL_LEFT = RGB(0, 255, 0),
	ROLL_HALF_POST = RGB(255, 255, 0),
	ROLL_UP = RGB(0, 0, 0),
	ROLL_SIDE_CHECK = RGB(100, 100, 100),
	HORIZONTAL_RUN = RGB(0, 128, 0),
	HORIZONTAL_RUN_FINISH = RGB(0, 0, 128)
};
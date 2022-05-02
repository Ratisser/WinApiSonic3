#pragma once

enum class eCollisionType
{
	IMAGE,
	POINT,
	RECT,
	CIRCLE,
	LINE,
	AABBBOX, // 회전 안 하는 박스
	OBBBOX, // 회전하는 박스
	MAX
};
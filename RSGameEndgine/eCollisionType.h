#pragma once

enum class eCollisionType
{
	IMAGE,
	POINT,
	RECT,
	CIRCLE,
	LINE,
	AABBBOX, // ȸ�� �� �ϴ� �ڽ�
	OBBBOX, // ȸ���ϴ� �ڽ�
	MAX
};
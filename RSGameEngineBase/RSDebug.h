#pragma once

#include <assert.h>

#define SAFE_DELETE(a) if (nullptr != a) { delete a; a = nullptr; }

class RSDebug
{
public:
	RSDebug() = delete;
	~RSDebug() = delete;

	RSDebug(const RSDebug& _other) = delete;
	RSDebug(RSDebug&& _other) = delete;

	RSDebug& operator=(const RSDebug& _other) = delete;
	RSDebug& operator=(const RSDebug&& _other) = delete;

public:
	static void MsgBoxInfo(const char* _szMessage);
	static void MsgBoxError(const char* _szMessage);
	static void AssertFalse();
	static void SetMemoryLeakCheck();
	static void DebugMessage(const char* szDebugString);

	static void Assert(bool _state) { assert(_state); }
};


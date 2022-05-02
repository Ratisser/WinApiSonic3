#include "RSDebug.h"

#include <Windows.h>
#include <assert.h>
#include <crtdbg.h>

void RSDebug::MsgBoxInfo(const char* _szMessage)
{
	MessageBoxA(nullptr, _szMessage, "Info", MB_OK);
}

void RSDebug::MsgBoxError(const char* _szMessage)
{
	MessageBoxA(nullptr, _szMessage, "Error", MB_OK);
	assert(false);
}

void RSDebug::AssertFalse()
{
	assert(false);
}

void RSDebug::SetMemoryLeakCheck()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
}

void RSDebug::DebugMessage(const char* szDebugString)
{
	OutputDebugStringA(szDebugString);
}
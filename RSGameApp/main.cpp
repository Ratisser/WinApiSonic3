#include <RSWindow.h>
#include <RSLevel.h>
#include <RSDebug.h>
#include <RSTimer.h>
#include <RSResourceManager.h>
#include <RSLevelManager.h>
#include <RSSoundManager.h>

#include "Constant.h"
#include "AppRelease.h"
#include "LoadResource.h"

#define new new(_CLIENT_BLOCK,__FILE__,__LINE__)

void GameLoop()
{
	RSLevelManager::GetInstance().Update();
	RSLevelManager::GetInstance().Render();
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	RSDebug::SetMemoryLeakCheck();

	// 윈도우를 초기화 합니다.
	RSWindow::GetInstance().InitWindow(hInstance, L"RSWindow", L"Sonic3 & Knuckles", float4(100.0f, 100.0f), Resolution::QVGA);

	// 필요한 자원을 로드합니다.
	LoadResource();
	
	// 화면에 보여지는 해상도를 정합니다. (렌더링 해상도는 동일)
	RSWindow::GetInstance().SetClientSize(Resolution::VGA);

	// 게임이 시작됩니다.
	RSWindow::GetInstance().Loop(GameLoop);

	// 사용한 자원을 반환합니다.
	AppRelease();

	return 0;
}
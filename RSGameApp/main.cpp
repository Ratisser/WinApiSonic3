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

	// �����츦 �ʱ�ȭ �մϴ�.
	RSWindow::GetInstance().InitWindow(hInstance, L"RSWindow", L"Sonic3 & Knuckles", float4(100.0f, 100.0f), Resolution::QVGA);

	// �ʿ��� �ڿ��� �ε��մϴ�.
	LoadResource();
	
	// ȭ�鿡 �������� �ػ󵵸� ���մϴ�. (������ �ػ󵵴� ����)
	RSWindow::GetInstance().SetClientSize(Resolution::VGA);

	// ������ ���۵˴ϴ�.
	RSWindow::GetInstance().Loop(GameLoop);

	// ����� �ڿ��� ��ȯ�մϴ�.
	AppRelease();

	return 0;
}
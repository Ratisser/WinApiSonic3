#include "RSWindow.h"
#include "RSTimer.h"
#include "RSResourceManager.h"
#include "RSImage.h"
#include "RSInput.h"

#include <RSSoundManager.h>
#include <sstream>
#include <RSDebug.h>

RSWindow* RSWindow::instance_ = new RSWindow();

LRESULT CALLBACK WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam)
{
	return gAppHandle->WndProc(_hWnd, _message, _wParam, _lParam);
}

RSWindow::RSWindow()
	: hWnd_(NULL)
	, hInstance_(NULL)
	, hdc_(NULL)
	, clientSize_(float4::ZERO)
	, windowPos_(float4::ZERO)
	, viewportSize_(float4::ZERO)
{
}

LRESULT CALLBACK RSWindow::WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam)
{
	switch (_message)
	{
	case WM_CREATE:
	{
	}
	break;
	case WM_MOVE:
	{
		RECT winRect = { 0, };
		GetWindowRect(_hWnd, &winRect);
		windowPos_.x = static_cast<float>(winRect.left);
		windowPos_.y = static_cast<float>(winRect.bottom);
	}
	break;
	case WM_SIZE:
	{

	}
	break;
	case WM_DESTROY:
		ReleaseDC(hWnd_, hdc_);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(_hWnd, _message, _wParam, _lParam);
	}

	return 0;
}

void RSWindow::InitWindow(HINSTANCE _hInstance, const wchar_t* _windowClassName, const wchar_t* _windowTitleName, float4 _pos, float4 _size)
{
	gAppHandle = this;
	windowPos_ = _pos;
	clientSize_ = _size;
	viewportSize_ = _size;
	registerWindowClass(_hInstance, _windowClassName);
	createWindow(_hInstance, _windowClassName, _windowTitleName);
}

void RSWindow::Loop(void(*_loopFunc)())
{
	MSG msg;

	while (true)
	{
		if (0 != PeekMessageW(&msg, 0, 0, 0, PM_REMOVE))
		{
			if (WM_QUIT == msg.message)
			{
				break;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			RSTimer::GetInstance().UpdateTime();
			RSInput::GetInstance().update();
			RSSoundManager::GetInstance().Update();

			_loopFunc();

			static unsigned int frameCount = 0;
			static float timeElapsed = 0.0f;

			timeElapsed += RSTimer::GetInstance().GetDeltaTime();

			if (timeElapsed >= 1.0f)
			{
				float fps = (float)frameCount;

				std::wostringstream out;
				out.precision(8);
				out << L"Sonic3 & Knuckles - FPS: " << fps << L" Frame Time: " << RSTimer::GetInstance().GetDeltaTime() << L" second";
				SetWindowText(hWnd_, out.str().c_str());

				frameCount = 0;
				timeElapsed = 0;
			}

			frameCount++;
		}

	}
}

void RSWindow::SetClientSize(float4 _size)
{
	clientSize_ = _size;

	RECT winRect = { 0, 0, _size.ix(), _size.iy() };
	RECT curRect;
	GetWindowRect(hWnd_, &curRect);
	AdjustWindowRect(&winRect, WS_OVERLAPPEDWINDOW, false);
	MoveWindow(hWnd_, curRect.left, curRect.top, winRect.right - winRect.left, winRect.bottom - winRect.top, true);

	RSResourceManager::GetInstance().windowImage_->SetWindowDCHandle(hdc_, clientSize_);
}

float4 RSWindow::GetClientSize() const
{
	return clientSize_;
}

float4 RSWindow::GetViewportSize() const
{
	return viewportSize_;
}

void RSWindow::registerWindowClass(HINSTANCE _hInstance, const wchar_t* _windowClassName)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW; // 화면 재갱신 옵션
	wcex.lpfnWndProc = ::WndProc; // 프로시저 호출
	wcex.cbClsExtra = 0; // 신경안써도 됨
	wcex.cbWndExtra = 0; // 신경안써도 됨
	wcex.hInstance = _hInstance;

	wcex.hIcon = nullptr; // LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT3));
	wcex.hCursor = nullptr; // LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

	wcex.lpszMenuName = nullptr; // MAKEINTRESOURCEW(IDC_WINDOWSPROJECT3);
	wcex.lpszClassName = _windowClassName;
	wcex.hIconSm = nullptr; // LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	// 아래의 함수의 내용이 
	RegisterClassExW(&wcex);
}

void RSWindow::createWindow(HINSTANCE _hInstance, const wchar_t* _windowClassName, const wchar_t* _windowTitleName)
{
	hWnd_ = CreateWindowW(_windowClassName, _windowTitleName, WS_OVERLAPPEDWINDOW,
		windowPos_.ix(), windowPos_.iy(), CW_USEDEFAULT, 0, nullptr, nullptr, _hInstance, nullptr);

	if (0 == hWnd_)
	{
		RSDebug::AssertFalse();
		return;
	}

	hdc_ = ::GetDC(hWnd_);

	RSResourceManager::GetInstance().InitializeWindowImage(hdc_, clientSize_);

	// 소닉은 320 * 240 해상도로 고정되어 있습니다.
	RSResourceManager::GetInstance().InitializeBackBufferImage(hdc_, float4(320.f, 240.f));

	SetClientSize(clientSize_);

	ShowWindow(hWnd_, SW_SHOW);
	UpdateWindow(hWnd_);

	return;
}

RSWindow::~RSWindow()
{
}
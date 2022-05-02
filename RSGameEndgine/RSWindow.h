#pragma once

#include <Windows.h>
#include <string>
#include <RSMath.h>

LRESULT CALLBACK WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam);

class RSWindow
{

public:
	static RSWindow& GetInstance()
	{
		return *instance_;
	}

	static void Destroy()
	{
		if (nullptr != instance_)
		{
			delete instance_;
			instance_ = nullptr;
		}
	}

public:
	void InitWindow(HINSTANCE _hInstance, const wchar_t* _windowClassName, const wchar_t* _windowTitleName, float4 _pos, float4 _clientSize);
	LRESULT CALLBACK WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam);
	void Loop(void(*_loopFunc)());

	void SetClientSize(float4 _clientSize);
	float4 GetClientSize() const;
	float4 GetViewportSize() const;

	HWND GetHandle() { return hWnd_; }

public:
	~RSWindow();

	RSWindow(const RSWindow& _other) = delete;
	RSWindow(RSWindow&& _other) = delete;

	RSWindow& operator=(const RSWindow& _other) = delete;
	RSWindow& operator=(const RSWindow&& _other) = delete;

private:
	RSWindow();
	void registerWindowClass(HINSTANCE _hInstance, const wchar_t* _windowClassName);
	void createWindow(HINSTANCE _hInstance, const wchar_t* _windowClassName, const wchar_t* _windowTitleName);

private:
	static RSWindow* instance_;
	HINSTANCE hInstance_;
	HWND hWnd_;
	HDC hdc_;
	float4 clientSize_;
	float4 windowPos_;
	float4 viewportSize_;
};

static RSWindow* gAppHandle = nullptr;
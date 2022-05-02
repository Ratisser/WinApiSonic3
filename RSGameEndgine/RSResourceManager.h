#pragma once

#include <string>
#include <map>
#include <Windows.h>
#include <RSMath.h>
#include <RSNameHelper.h>

// 분류 :
// 용도 :
// 설명 :
class RSImage;
class RSWindow;
class RSResourceManager : public RSNameHelper
{
	friend RSWindow;
public:
	static RSResourceManager& GetInstance()
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
	bool InitializeWindowImage(HDC _hdc, const float4& _size);
	bool InitializeBackBufferImage(HDC _hdc, const float4& _size);
	bool LoadImageFromFile(std::string _name, std::string _path);
	RSImage* FindImage(std::string _name);
	RSImage* GetBackBuffer() const;
	RSImage* GetWindowImage() const;

	void ClearBackBuffer();
	void DoubleBuffering();

public:
	~RSResourceManager();

	RSResourceManager(const RSResourceManager& _other) = delete;
	RSResourceManager(RSResourceManager&& _other) = delete;

	RSResourceManager& operator=(const RSResourceManager& _other) = delete;
	RSResourceManager& operator=(const RSResourceManager&& _other) = delete;

private:
	RSResourceManager();

private:
	static RSResourceManager* instance_;
	std::map<std::string, RSImage*> images_;
	RSImage* windowImage_;
	RSImage* backBufferImage_;
};


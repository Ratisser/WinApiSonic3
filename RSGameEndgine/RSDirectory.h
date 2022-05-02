#pragma once

#include <Windows.h>
#include <string>

// �з� :
// �뵵 :
// ���� :
class RSDirectory
{
public:
	RSDirectory();
	~RSDirectory();

	RSDirectory(const RSDirectory& _other) = delete;
	RSDirectory(RSDirectory&& _other) = delete;

	RSDirectory& operator=(const RSDirectory& _other) = delete;
	RSDirectory& operator=(const RSDirectory&& _other) = delete;

public:
	static std::string GetCurrentDir()
	{
		char buffer[256];
		ZeroMemory(buffer, 256);
		GetCurrentDirectoryA(256, buffer);

		return std::string(buffer) + "\\";
	}

private:

};


#pragma once

#include <map>
#include <string>
#include <Windows.h>
#include <RSNameHelper.h>

class RSWindow;
class RSInput : public RSNameHelper
{
	friend RSWindow;
private:
	class RSKey : public RSNameHelper
	{
		friend RSInput;

		

	private:
		RSKey();
		~RSKey();

		void update();

	private:
		int key_;
		bool down_;
		bool up_;
		bool press_;
		bool free_;
	};

public:
	static RSInput& GetInstance()
	{
		return *instance_;
	}

	void Destroy()
	{
		if (nullptr != instance_)
		{
			delete instance_;
			instance_ = nullptr;
		}
	}

public:
	~RSInput();

	RSInput(const RSInput& _other) = delete;
	RSInput(RSInput&& _other) = delete;

	RSInput& operator=(const RSInput& _other) = delete;
	RSInput& operator=(const RSInput&& _other) = delete;

	bool CreateKey(const std::string& _keyName, int _key);

	bool IsKeyDown(const std::string& _keyName);
	bool IsKeyUp(const std::string& _keyName);
	bool IsKeyPress(const std::string& _keyName);
	bool IsKeyFree(const std::string& _keyName);
	bool IsKeyAvailable(const std::string& _keyName);

private:
	RSInput();
	void update();

private:
	static RSInput* instance_;

	std::map<std::string, RSKey*> keys_;

};


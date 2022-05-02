#pragma once
#include <string>

// �з� :
// �뵵 :
// ���� :
class RSNameHelper
{
public:
	RSNameHelper() = default;
	~RSNameHelper() = default;

	RSNameHelper(const RSNameHelper& _other) = delete;
	RSNameHelper(RSNameHelper&& _other) = delete;

	RSNameHelper& operator=(const RSNameHelper& _other) = delete;
	RSNameHelper& operator=(const RSNameHelper&& _other) = delete;

public:
	std::string GetName()
	{
		return name_;
	}

	void SetName(const std::string& _name)
	{
		name_ = _name;
	}

private:
	std::string name_;
};


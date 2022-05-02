#include "RSInput.h"

#include <RSDebug.h>

RSInput* RSInput::instance_ = new RSInput();

RSInput::RSInput()
{
}

RSInput::~RSInput()
{
	std::map<std::string, RSKey*>::iterator startIter = keys_.begin();
	std::map<std::string, RSKey*>::iterator endIter = keys_.end();

	while (startIter != endIter)
	{
		delete startIter->second;
		startIter->second = nullptr;
		startIter++;
	}

	keys_.clear();
}

void RSInput::RSKey::update()
{
	if (0 != GetAsyncKeyState(key_))
	{
		if (false == press_)
		{
			down_ = true;
			press_ = true;
			up_ = false;
			free_ = false;
		}
		else if (true == press_)
		{
			down_ = false;
			press_ = true;
			up_ = false;
			free_ = false;
		}
	}
	else
	{
		if (true == press_)
		{
			up_ = true;
			press_ = false;
			down_ = false;
			free_ = true;
		}
		else if (false == press_)
		{
			down_ = false;
			up_ = false;
			press_ = false;
			free_ = true;
		}
	}
}

RSInput::RSKey::RSKey()
	: key_(-1)
	, down_(false)
	, up_(false)
	, press_(false)
	, free_(true)
{
}

RSInput::RSKey::~RSKey()
{

}

bool RSInput::CreateKey(const std::string& _keyName, int _key)
{
	std::map<std::string, RSKey*>::iterator findIter = keys_.find(_keyName);
	std::map<std::string, RSKey*>::iterator endIter = keys_.end();

	if (findIter != endIter)
	{
		return false;
	}

	RSKey* newKey = new RSKey();
	newKey->SetName(_keyName);
	newKey->key_ = _key;

	keys_[_keyName] = newKey;

	return true;
}

bool RSInput::IsKeyDown(const std::string& _keyName)
{
	std::map<std::string, RSKey*>::iterator findIter = keys_.find(_keyName);
	std::map<std::string, RSKey*>::iterator endIter = keys_.end();

	if (findIter == endIter)
	{
		RSDebug::AssertFalse();
		return false;
	}

	return findIter->second->down_;
}

bool RSInput::IsKeyUp(const std::string& _keyName)
{
	std::map<std::string, RSKey*>::iterator findIter = keys_.find(_keyName);
	std::map<std::string, RSKey*>::iterator endIter = keys_.end();

	if (findIter == endIter)
	{
		RSDebug::AssertFalse();
		return false;
	}

	return findIter->second->up_;
}

bool RSInput::IsKeyPress(const std::string& _keyName)
{
	std::map<std::string, RSKey*>::iterator findIter = keys_.find(_keyName);
	std::map<std::string, RSKey*>::iterator endIter = keys_.end();

	if (findIter == endIter)
	{
		RSDebug::AssertFalse();
		return false;
	}

	return findIter->second->press_;
}

bool RSInput::IsKeyFree(const std::string& _keyName)
{
	std::map<std::string, RSKey*>::iterator findIter = keys_.find(_keyName);
	std::map<std::string, RSKey*>::iterator endIter = keys_.end();

	if (findIter == endIter)
	{
		RSDebug::AssertFalse();
		return false;
	}

	return findIter->second->free_;
}

bool RSInput::IsKeyAvailable(const std::string& _keyName)
{
	std::map<std::string, RSKey*>::iterator findIter = keys_.find(_keyName);
	std::map<std::string, RSKey*>::iterator endIter = keys_.end();

	if (findIter == endIter)
	{
		return false;
	}

	return true;
}

void RSInput::update()
{
	std::map<std::string, RSKey*>::iterator startIter = keys_.begin();
	std::map<std::string, RSKey*>::iterator endIter = keys_.end();

	while (startIter != endIter)
	{
		startIter->second->update();
		startIter++;
	}
}

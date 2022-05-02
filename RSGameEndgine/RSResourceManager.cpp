#include "RSResourceManager.h"
#include "RSImage.h"

#include <RSDebug.h>

RSResourceManager* RSResourceManager::instance_ = new RSResourceManager();

RSResourceManager::RSResourceManager()
	: windowImage_(nullptr)
	, backBufferImage_(nullptr)
	, images_()
{
}

bool RSResourceManager::LoadImageFromFile(std::string _name, std::string _path)
{
	std::map<std::string, RSImage*>::iterator findIter = images_.find(_name);

	if (findIter != images_.end())
	{
		//RSDebug::AssertFalse();
		return false;
	}

	RSImage* newImage = new RSImage();
	newImage->Load(_path);

	images_.insert(std::pair<std::string, RSImage*>(_name, newImage));
	return true;
}

RSImage* RSResourceManager::FindImage(std::string _name)
{
	std::map<std::string, RSImage*>::iterator findIter = images_.find(_name);

	if (findIter == images_.end())
	{
		RSDebug::AssertFalse();
		return nullptr;
	}

	return findIter->second;
}

void RSResourceManager::ClearBackBuffer()
{
	backBufferImage_->ClearImage();
}

void RSResourceManager::DoubleBuffering()
{
	//windowImage_->BitCopyByImageSize(backBufferImage_, float4::ZERO);
	windowImage_->StretchCopyByImageSize(backBufferImage_, float4::ZERO);
}

RSImage* RSResourceManager::GetBackBuffer() const
{
	return backBufferImage_;
}

RSImage* RSResourceManager::GetWindowImage() const
{
	return windowImage_;
}

bool RSResourceManager::InitializeWindowImage(HDC _hdc, const float4& _size)
{
	if (NULL == _hdc)
	{
		RSDebug::AssertFalse();
		return false;
	}

	windowImage_ = new RSImage();
	windowImage_->SetWindowDCHandle(_hdc, _size);
	return true;
}

bool RSResourceManager::InitializeBackBufferImage(HDC _hdc, const float4& _size)
{
	if (NULL == _hdc)
	{
		RSDebug::AssertFalse();
		return false;
	}

	backBufferImage_ = new RSImage();
	backBufferImage_->CreateBitmap(_hdc, _size);

	return true;
}




RSResourceManager::~RSResourceManager()
{
	std::map<std::string, RSImage*>::iterator startIter = images_.begin();
	std::map<std::string, RSImage*>::iterator endIter = images_.end();

	while (startIter != endIter)
	{
		delete startIter->second;
		startIter++;
	}

	images_.clear();

	if (nullptr != backBufferImage_)
	{
		delete backBufferImage_;
		backBufferImage_ = nullptr;
	}

	if (nullptr != windowImage_)
	{
		delete windowImage_;
		windowImage_ = nullptr;
	}
}
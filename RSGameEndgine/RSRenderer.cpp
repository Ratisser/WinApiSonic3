#include "RSRenderer.h"
#include "RSImage.h"
#include "RSResourceManager.h"
#include "RSActor.h"

#include <RSDebug.h>

RSRenderer::RSRenderer(RSActor* _actor, bool _bCameraEffect, eRenderPivot _renderPivot)
	: actor_(_actor)
	, image_(nullptr)
	, maskImage_(nullptr)
	, pivot_(float4::ZERO)
	, renderSize_(float4::ZERO)
	, imagePos_(float4::ZERO)
	, imageSize_(float4::ZERO)
	, imagePivot_(float4::ZERO)
	, transparentColor_(0)
	, currentAnimation_(nullptr)
	, renderPivot_(_renderPivot)
	, bCameraEffect_(_bCameraEffect)
	, bActivation_(true)
	, alpha_(255.f)
	, rotate_(0.0f)
{
}

RSRenderer::~RSRenderer()
{
	std::map<std::string, Animation*>::iterator startIter = animations_.begin();
	std::map<std::string, Animation*>::iterator endIter = animations_.end();

	while (startIter != endIter)
	{
		delete startIter->second;
		startIter->second = nullptr;
		startIter++;
	}

	animations_.clear();
}

bool RSRenderer::SetImage(RSImage* _image)
{
	if (nullptr == _image)
	{
		RSDebug::AssertFalse();
		return false;
	}

	image_ = _image;
	imageSize_ = image_->GetSize();
	renderSize_ = imageSize_;

	return true;
}

bool RSRenderer::SetImage(const std::string& _imageName)
{
	image_ = RSResourceManager::GetInstance().FindImage(_imageName);
	if (nullptr == image_)
	{
		RSDebug::AssertFalse();
		return false;
	}

	imageSize_ = image_->GetSize();
	renderSize_ = imageSize_;

	return true;
}

bool RSRenderer::SetImageNoSize(const std::string& _imageName)
{
	image_ = RSResourceManager::GetInstance().FindImage(_imageName);
	if (nullptr == image_)
	{
		RSDebug::AssertFalse();
		return false;
	}

	return true;
}

bool RSRenderer::SetMask(const std::string& _imageName)
{
	maskImage_ = RSResourceManager::GetInstance().FindImage(_imageName);
	if (nullptr == maskImage_)
	{
		RSDebug::AssertFalse();
		return false;
	}
	
	return true;
}

void RSRenderer::UnsetMask()
{
	maskImage_ = nullptr;
}

void RSRenderer::SetPivot(const float4& _pivot)
{
	pivot_ = _pivot;
}

void RSRenderer::SetRenderSize(const float4& _renderSize)
{
	renderSize_ = _renderSize;
}

void RSRenderer::SetImagePos(const float4& _imagePos)
{
	imagePos_ = _imagePos;
}

void RSRenderer::SetImageSize(const float4& _imageSize)
{
	imageSize_ = _imageSize;
}

void RSRenderer::SetImagePivot(const float4& _imagePivot)
{
	imagePivot_ = _imagePivot;
}

void RSRenderer::SetRotate(float _rotate)
{
	rotate_ = _rotate;
}

void RSRenderer::SetCutIndex(size_t _index)
{
	if (nullptr == image_)
	{
		RSDebug::AssertFalse();
		return;
	}

	imagePos_ = image_->GetCutImagePos(_index);
	imageSize_ = image_->GetCutImageSize(_index);

	renderSize_ = imageSize_;
}

void RSRenderer::CreateAnimation(const std::string& _animationName, const std::string& _imageName, int _startIndex, int _endIndex, float _frameTime, bool _bLoop)
{
	std::map<std::string, Animation*>::iterator findIter = animations_.find(_animationName);

	if (findIter != animations_.end())
	{
		RSDebug::AssertFalse();
	}

	RSImage* image = RSResourceManager::GetInstance().FindImage(_imageName);
	Animation* newAnimation = new Animation(image, this, _startIndex, _endIndex, _frameTime, _bLoop);
	newAnimation->SetName(_animationName);

	animations_.insert(std::pair<std::string, Animation*>(_animationName, newAnimation));
}

void RSRenderer::ChangeAnimation(const std::string& _animationName, const std::string& _imageName, bool _forceChange)
{
	if (nullptr != currentAnimation_ && _forceChange == false && _animationName == currentAnimation_->GetName())
	{
		return;
	}

	Animation* findAnim = findAnimation(_animationName);
	if (nullptr == findAnim)
	{
		RSDebug::AssertFalse();
		return;
	}
	SetImageNoSize(_imageName);
	currentAnimation_ = findAnim;
	currentAnimation_->Reset();
}

void RSRenderer::SetFrameTime(const std::string& _animationName, float _frameTime)
{
	Animation* animation = findAnimation(_animationName);
	animation->frameTime_ = _frameTime;
}

bool RSRenderer::IsAnimationEnd()
{
	return currentAnimation_->bEnd_;
}

void RSRenderer::Activate()
{
	bActivation_ = true;
}

void RSRenderer::Deactivate()
{
	bActivation_ = false;
}

void RSRenderer::SwitchActivation()
{
	bActivation_ = !bActivation_;
}

void RSRenderer::UpdateAnimation()
{
	if (nullptr == currentAnimation_)
	{
		RSDebug::AssertFalse();
	}
	if (bActivation_)
	{
		currentAnimation_->Update();
	}
}

RSRenderer::Animation* RSRenderer::findAnimation(const std::string& _animationName)
{
	std::map<std::string, Animation*>::iterator findIter = animations_.find(_animationName);

	if (findIter == animations_.end())
	{
		return nullptr;
	}

	return findIter->second;
}

void RSRenderer::Render()
{
	if (false == bActivation_)
	{
		return;
	}

	RSImage* backBuffer = RSResourceManager::GetInstance().GetBackBuffer();

	float4 renderPos;

	if (bCameraEffect_)
	{
		renderPos += actor_->GetCamEffectLocation();
	}
	else
	{
		renderPos += actor_->GetLocation();
	}

	renderPos += pivot_ + imagePivot_ - imageSize_.halffloat4(); // lefttop을 넘겨주기 위해 반을 뺀다.

	if (rotate_ == 0.0f)
	{
		backBuffer->TransparentCopy(image_,
			renderPos,
			renderSize_,
			imagePos_,
			imageSize_,
			RGB(255, 0, 255));
	}
	else
	{
		backBuffer->PlgCopy(image_, renderPos, renderSize_, imagePos_, imageSize_, maskImage_, rotate_);
	}
}


RSRenderer::Animation::Animation(RSImage* _image, RSRenderer* _renderer, int _startIndex, int _endIndex, float _frameTime, bool _bLoop)
	: image_(_image)
	, renderer_(_renderer)
	, startIndex_(_startIndex)
	, endIndex_(_endIndex)
	, currentIndex_(_endIndex)
	, frameTime_(_frameTime)
	, currentFrameTime_(0.0f)
	, bLoop_(_bLoop)
	, bEnd_(false)
{
	if (nullptr == image_)
	{
		RSDebug::AssertFalse();
	}

	if (nullptr == renderer_)
	{
		RSDebug::AssertFalse();
	}
}

RSRenderer::Animation::~Animation()
{
}

void RSRenderer::Animation::Reset()
{
	currentIndex_ = startIndex_;
	currentFrameTime_ = 0.0f;
	bEnd_ = false;
}

void RSRenderer::Animation::Update()
{
	bEnd_ = false;
	renderer_->SetCutIndex(currentIndex_);
	float deltaTime = RSTimer::GetInstance().GetDeltaTime();
	currentFrameTime_ += deltaTime;

	if (currentFrameTime_ >= frameTime_)
	{
		currentFrameTime_ = 0.0f;
		currentIndex_++;

		if (currentIndex_ > endIndex_)
		{
			if (bLoop_)
			{
				currentIndex_ = startIndex_;
			}
			else
			{
				--currentIndex_;
				bEnd_ = true;
			}
		}
	}

}

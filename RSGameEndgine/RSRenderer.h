#pragma once

#include <string>
#include <RSMath.h>
#include <RSNameHelper.h>
#include <map>

#include "eRenderPivot.h"

class RSActor;
class RSImage;
class RSRenderer
{
	friend RSActor;

	class Animation;
public:
	RSRenderer(RSActor* _actor, bool _bCameraEffect = true, eRenderPivot _renderPivot = eRenderPivot::CENTER);
	~RSRenderer();

	RSRenderer(const RSRenderer& _other) = delete;
	RSRenderer(RSRenderer&& _other) = delete;

	RSRenderer& operator=(const RSRenderer& _other) = delete;
	RSRenderer& operator=(const RSRenderer&& _other) = delete;

	bool SetImage(RSImage* _image);
	bool SetImage(const std::string& _imageName);
	bool SetImageNoSize(const std::string& _imageName);
	bool SetMask(const std::string& _imageName);
	void UnsetMask();
	void SetPivot(const float4& _pivot);
	void SetRenderSize(const float4& _renderSize);

	void SetImagePos(const float4& _imagePos);
	void SetImageSize(const float4& _imageSize);
	void SetImagePivot(const float4& _imagePivot);

	void SetRotate(float _rotate);

	float4 GetPivot() { return pivot_; }
	float4 GetImagePivot() { return imagePivot_; }
	float GetRotate() { return rotate_; }

	void SetCutIndex(size_t _index);

	void CreateAnimation(const std::string& _animationName, const std::string& _imageName, int _startIndex, int _endIndex, float _frameTime, bool _bLoop = false);
	void ChangeAnimation(const std::string& _animationName, const std::string& _imageName, bool _forceChange = false);
	void SetFrameTime(const std::string& _animationName, float _frameTime);
	bool IsAnimationEnd();

	bool IsActivate() { return bActivation_; }
	void Activate();
	void Deactivate();
	void SwitchActivation();

	void UpdateAnimation();
	void Render();
private:
	Animation* findAnimation(const std::string& _animationName);

private:
	std::map<std::string, Animation*> animations_;
	Animation* currentAnimation_;
	RSActor* actor_;
	RSImage* image_;
	RSImage* maskImage_;

	float4 pivot_;
	float4 renderSize_;

	float4 imagePos_;
	float4 imageSize_;
	float4 imagePivot_;

	eRenderPivot renderPivot_;

	bool bCameraEffect_;
	bool bActivation_;
	unsigned int transparentColor_;

	float alpha_;
	float rotate_;
};






class RSRenderer::Animation : public RSNameHelper
{
	friend RSRenderer;

	void Reset();
	void Update();
	bool IsEnd() { return bEnd_; }

private:
	Animation(RSImage* _image, RSRenderer* _renderer, int _startIndex, int _endIndex, float _frameTime, bool _bLoop = false);
	~Animation();

private:
	RSImage* image_;
	RSRenderer* renderer_;
	size_t startIndex_;
	size_t endIndex_;
	size_t currentIndex_;
	float frameTime_;
	float currentFrameTime_;
	bool bLoop_;
	bool bEnd_;
};
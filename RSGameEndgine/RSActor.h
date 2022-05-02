#pragma once

#include <RSMath.h>
#include "RSResourceManager.h"
#include "RSImage.h"
#include "RSInput.h"
#include "RSRenderer.h"
#include "RSLevel.h"

#include <RSTimer.h>
#include <RSDebug.h>
#include <RSNameHelper.h>


// 분류 :
// 용도 :
// 설명 :
class RSLevel;
class RSActor : public RSNameHelper
{
	friend RSLevel;
public:
	RSActor();
	virtual ~RSActor();

	RSActor(const RSActor& _other) = delete;
	RSActor(RSActor&& _other) = delete;

	RSActor& operator=(const RSActor& _other) = delete;
	RSActor& operator=(const RSActor&& _other) = delete;

public:

	float4	GetLocation() const { return location_; }
	int		GetUpdateOrder() const { return updateOrder_; }
	int		GetRenderOrder() const { return renderOrder_; }
	RSLevel* GetLevel() const { return level_; }

	void	SetLocation(const float4& _location) { location_ = _location; }
	void	SetMovement(const float4& _movement) { location_ += _movement; }
	void	SetUpdateOrder(int _updateOrder) { updateOrder_ = _updateOrder; }
	void	SetRenderOrder(int _renderOrder) { renderOrder_ = _renderOrder; }
	void	SetLevel(RSLevel* _level) { level_ = _level; }
	void	SetCamEffectScale(float4 _scale) { cameraEffectScale_ = _scale; }

	float4	GetCamEffectLocation();

	void Release();

public:
	virtual void Start() = 0;
	virtual void UpdateBefore() = 0;
	virtual void Update() = 0;
	virtual void UpdateAfter() = 0;
	virtual void Render() = 0;
	virtual void Cleanup() = 0;


protected:
	RSLevel* level_;
	float4 location_;
	int updateOrder_;
	int renderOrder_;

	bool bRelease_;

	float4 cameraEffectScale_;
};


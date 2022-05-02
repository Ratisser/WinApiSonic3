#include "RSActor.h"
#include "RSActor.h"
#include "RSLevel.h"

RSActor::RSActor()
	: updateOrder_(0)
	, renderOrder_(0)
	, level_(nullptr)
	, cameraEffectScale_(1.0f, 1.0f)
	, bRelease_(false)
{
}

RSActor::~RSActor()
{
}

float4 RSActor::GetCamEffectLocation()
{
	float4 camPos = level_->GetCamPos();
	float4 camSize = level_->GetCamSize();

	return location_ - (camPos * cameraEffectScale_) + camSize.halffloat4();
}

void RSActor::Release()
{
	bRelease_ = true;
}

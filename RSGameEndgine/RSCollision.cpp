#include "RSCollision.h"
#include "RSCollision.h"

#include <RSDebug.h>

#include "RSImage.h"
#include "RSActor.h"
#include "RSResourceManager.h"
#include "RSLevel.h"

bool (*RSCollision::collisionCheckFunction_[static_cast<size_t>(eCollisionType::MAX)][static_cast<size_t>(eCollisionType::MAX)])(const RSCollision& lhs, const RSCollision& rhs) = { 0, };

RSCollision::RSCollision(RSActor* _actor, eCollisionType _collisionType, int _groupIndex, float4 _pivot, float4 _size)
	: actor_(_actor)
	, collisionType_(_collisionType)
	, groupIndex_(_groupIndex)
	, pivot_(_pivot)
	, size_(_size)
	, image_(nullptr)
	, checkColor_(0)
	, radius_(0.0f)
	, rect_(FRect::ZERO)
{
	if (nullptr == _actor)
	{
		RSDebug::AssertFalse();
	}
}

RSCollision::~RSCollision()
{
}

void RSCollision::SetImage(const std::string& _imageName)
{
	image_ = RSResourceManager::GetInstance().FindImage(_imageName);
}

void RSCollision::SetCheckColor(DWORD _color)
{
	checkColor_ = _color;
}

void RSCollision::SetGroupIndex(int _groupIndex)
{
	groupIndex_ = _groupIndex;
}

void RSCollision::SetPivot(float4 _pivot)
{
	pivot_ = _pivot;
}

void RSCollision::SetRadius(float _radius)
{
	radius_ = _radius;
}

void RSCollision::SetRect(FRect _rect)
{
	rect_ = _rect;
}

void RSCollision::SetRect(float _left, float _top, float _right, float _bottom)
{
	rect_.left = _left;
	rect_.right = _right;
	rect_.top = _top;
	rect_.bottom = _bottom;
}

RSActor* RSCollision::GetActor()
{
	return actor_;
}

float4 RSCollision::GetPivot()
{
	return pivot_;
}

float4 RSCollision::GetCollisionPoint() const
{
	return actor_->GetLocation() + pivot_;
}

FRect RSCollision::GetCollisionRect() const
{
	FRect rect;
	float4 location = actor_->GetLocation() + pivot_;
	rect.left = rect_.left + location.x;
	rect.right = rect_.right + location.x;
	rect.top = rect_.top + location.y;
	rect.bottom = rect_.bottom + location.y;
	return rect;
}

eCollisionType RSCollision::GetCollisionType()
{
	return collisionType_;
}

int RSCollision::GetGroupIndex()
{
	return groupIndex_;
}

bool RSCollision::IsColliding(const RSCollision& _other)
{
	if (nullptr == collisionCheckFunction_[static_cast<size_t>(collisionType_)][static_cast<size_t>(_other.collisionType_)])
	{
		RSDebug::MsgBoxError("Unimplemented collision check function used");
	}

	return collisionCheckFunction_[static_cast<size_t>(collisionType_)][static_cast<size_t>(_other.collisionType_)](*this, _other);
}

RSCollision* RSCollision::GetCollisionGroupCheckOne(int _groupIndex)
{
	std::list<RSCollision*>& collisionList = actor_->GetLevel()->GetCollisionList(_groupIndex);

	std::list<RSCollision*>::iterator iter = collisionList.begin();
	std::list<RSCollision*>::iterator endIter = collisionList.end();

	while (iter != endIter)
	{
		RSCollision* currentObject = *iter;
		if (IsColliding(*currentObject))
		{
			return currentObject;
		}

		iter++;
	}

	return nullptr;
}

std::list<RSCollision*> RSCollision::GetCollisionGroupCheck(int _groupIndex)
{
	std::list<RSCollision*> ret;

	std::list<RSCollision*>& collisionList = actor_->GetLevel()->GetCollisionList(_groupIndex);

	std::list<RSCollision*>::iterator iter = collisionList.begin();
	std::list<RSCollision*>::iterator endIter = collisionList.end();

	while (iter != endIter)
	{
		RSCollision* currentObject = *iter;
		if (IsColliding(*currentObject))
		{
			ret.push_back(currentObject);
		}

		iter++;
	}

	return ret;
}

void RSCollision::DebugRender(HDC _hdc)
{
	float4 location = actor_->GetCamEffectLocation();
	switch (collisionType_)
	{
	case eCollisionType::RECT:
	{
		Rectangle(_hdc, static_cast<int>(location.x + rect_.left), static_cast<int>(location.y + rect_.top), static_cast<int>(location.x + rect_.right), static_cast<int>(location.y + rect_.bottom));
	}
		break;
	case eCollisionType::CIRCLE:
	{
		
		int radius = static_cast<int>(radius_);
		Ellipse(_hdc, location.ix() - radius, location.iy() - radius, location.ix() + radius, location.iy() + radius);
	}
		break;
	default:
		break;
	}


}

void RSCollision::collisionCheckFunctionInitialize_()
{
	for (size_t y = 0; y < static_cast<size_t>(eCollisionType::MAX); y++)
	{
		for (size_t x = 0; x < static_cast<size_t>(eCollisionType::MAX); x++)
		{
			collisionCheckFunction_[y][x] = nullptr;
		}
	}

	collisionCheckFunction_[static_cast<size_t>(eCollisionType::POINT)][static_cast<size_t>(eCollisionType::IMAGE)] = &RSCollision::collisionCheckPointToImage;
	collisionCheckFunction_[static_cast<size_t>(eCollisionType::IMAGE)][static_cast<size_t>(eCollisionType::POINT)] = &RSCollision::collisionCheckImageToPoint;

	collisionCheckFunction_[static_cast<size_t>(eCollisionType::RECT)][static_cast<size_t>(eCollisionType::RECT)] = &RSCollision::collisionCheckRectToRect;

	collisionCheckFunction_[static_cast<size_t>(eCollisionType::CIRCLE)][static_cast<size_t>(eCollisionType::CIRCLE)] = &RSCollision::collisionCheckCircleToCircle;

	collisionCheckFunction_[static_cast<size_t>(eCollisionType::POINT)][static_cast<size_t>(eCollisionType::CIRCLE)] = &RSCollision::collisionCheckPointToCircle;
	collisionCheckFunction_[static_cast<size_t>(eCollisionType::CIRCLE)][static_cast<size_t>(eCollisionType::POINT)] = &RSCollision::collisionCheckCircleToPoint;

	collisionCheckFunction_[static_cast<size_t>(eCollisionType::POINT)][static_cast<size_t>(eCollisionType::RECT)] = &RSCollision::collisionCheckPointToRect;
	collisionCheckFunction_[static_cast<size_t>(eCollisionType::RECT)][static_cast<size_t>(eCollisionType::POINT)] = &RSCollision::collisionCheckRectToPoint;

	collisionCheckFunction_[static_cast<size_t>(eCollisionType::RECT)][static_cast<size_t>(eCollisionType::CIRCLE)] = &RSCollision::collisionCheckRectToCircle;
	collisionCheckFunction_[static_cast<size_t>(eCollisionType::CIRCLE)][static_cast<size_t>(eCollisionType::RECT)] = &RSCollision::collisionCheckCircleToRect;
}

bool RSCollision::collisionCheckPointToImage(const RSCollision& lhs, const RSCollision& rhs)
{
	if (nullptr == rhs.image_)
	{
		RSDebug::AssertFalse();
		return false;
	}

	float4 collisionPoint = lhs.GetCollisionPoint();
	DWORD pointColor = rhs.image_->GetPixelColor(collisionPoint.ix(), collisionPoint.iy());

	return pointColor == lhs.checkColor_;
}

bool RSCollision::collisionCheckImageToPoint(const RSCollision& lhs, const RSCollision& rhs)
{
	return collisionCheckPointToImage(rhs, lhs);
}

bool RSCollision::collisionCheckCircleToCircle(const RSCollision& lhs, const RSCollision& rhs)
{
	float distance = lhs.GetCollisionPoint().DistanceTo(rhs.GetCollisionPoint());

	return 0 >= (distance - lhs.radius_ - rhs.radius_);
}

bool RSCollision::collisionCheckRectToRect(const RSCollision& lhs, const RSCollision& rhs)
{
	return false;
}

bool RSCollision::collisionCheckPointToCircle(const RSCollision& lhs, const RSCollision& rhs)
{
	float distance = lhs.GetCollisionPoint().DistanceTo(rhs.GetCollisionPoint());

	return 0 >= (distance - lhs.radius_ - rhs.radius_);
}

bool RSCollision::collisionCheckCircleToPoint(const RSCollision& lhs, const RSCollision& rhs)
{
	float distance = lhs.GetCollisionPoint().DistanceTo(rhs.GetCollisionPoint());

	return 0 >= (distance - lhs.radius_ - rhs.radius_);
}

bool RSCollision::collisionCheckPointToRect(const RSCollision& lhs, const RSCollision& rhs)
{
	float4 point = lhs.GetCollisionPoint();
	FRect rect = rhs.GetCollisionRect();
	if (point.x < rect.left)
	{
		return false;
	}

	if (point.x > rect.right)
	{
		return false;
	}

	if (point.y < rect.top)
	{
		return false;
	}

	if (point.y > rect.bottom)
	{
		return false;
	}

	return true;
}

bool RSCollision::collisionCheckRectToPoint(const RSCollision& lhs, const RSCollision& rhs)
{
	FRect rect = lhs.GetCollisionRect();
	float4 point = rhs.GetCollisionPoint();

	if (point.x < rect.left)
	{
		return false;
	}

	if (point.x > rect.right)
	{
		return false;
	}

	if (point.y < rect.top)
	{
		return false;
	}

	if (point.y > rect.bottom)
	{
		return false;
	}

	return true;
}

bool RSCollision::collisionCheckRectToCircle(const RSCollision& lhs, const RSCollision& rhs)
{
	FRect rect = lhs.rect_;
	rect.left -= rhs.radius_;
	rect.top -= rhs.radius_;
	rect.right += rhs.radius_;
	rect.bottom += rhs.radius_;

	float4 point = rhs.GetCollisionPoint();

	if (point.x < rect.left)
	{
		return false;
	}

	if (point.x > rect.right)
	{
		return false;
	}

	if (point.y < rect.top)
	{
		return false;
	}

	if (point.y > rect.bottom)
	{
		return false;
	}

	return true;
}

bool RSCollision::collisionCheckCircleToRect(const RSCollision& lhs, const RSCollision& rhs)
{
	return collisionCheckRectToCircle(rhs, lhs);
}

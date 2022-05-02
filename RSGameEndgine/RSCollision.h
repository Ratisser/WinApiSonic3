#pragma once

#include <Windows.h>
#include <string>
#include <RSMath.h>
#include <list>

#include "eCollisionType.h"

class RSImage;
class RSActor;
class RSLevel;
class RSLevelManager;
class RSCollision
{
	friend RSActor;
	friend RSLevel;
	friend RSLevelManager;
public:
	RSCollision(RSActor* _actor, eCollisionType _collisionType, int _groupIndex, float4 _pivot = float4::ZERO, float4 _size = float4::ZERO);
	~RSCollision();

	RSCollision(const RSCollision& _other) = delete;
	RSCollision(RSCollision&& _other) = delete;

	RSCollision& operator=(const RSCollision& _other) = delete;
	RSCollision& operator=(const RSCollision&& _other) = delete;

	void SetImage(const std::string& _imageName);
	void SetCheckColor(DWORD _color);
	void SetGroupIndex(int _groupIndex);
	void SetPivot(float4 _pivot);
	void SetRadius(float _radius);
	void SetRect(FRect _rect);
	void SetRect(float _left, float _top, float _right, float _bottom);
	RSActor* GetActor();
	float4 GetPivot();
	float4 GetCollisionPoint() const;
	FRect GetCollisionRect() const;
	eCollisionType GetCollisionType();
	int GetGroupIndex();

	bool IsColliding(const RSCollision& _other);
	RSCollision* GetCollisionGroupCheckOne(int _groupIndex);
	std::list<RSCollision*> GetCollisionGroupCheck(int _groupIndex);

	void DebugRender(HDC _hdc);

private:
	static void collisionCheckFunctionInitialize_();
	static bool collisionCheckPointToImage(const RSCollision& lhs, const RSCollision& rhs);
	static bool collisionCheckImageToPoint(const RSCollision& lhs, const RSCollision& rhs);
	static bool collisionCheckCircleToCircle(const RSCollision& lhs, const RSCollision& rhs);
	static bool collisionCheckRectToRect(const RSCollision& lhs, const RSCollision& rhs);
	static bool collisionCheckPointToCircle(const RSCollision& lhs, const RSCollision& rhs);
	static bool collisionCheckCircleToPoint(const RSCollision& lhs, const RSCollision& rhs);
	static bool collisionCheckPointToRect(const RSCollision& lhs, const RSCollision& rhs);
	static bool collisionCheckRectToPoint(const RSCollision& lhs, const RSCollision& rhs);
	static bool collisionCheckRectToCircle(const RSCollision& lhs, const RSCollision& rhs);
	static bool collisionCheckCircleToRect(const RSCollision& lhs, const RSCollision& rhs);

private:
	static bool (*collisionCheckFunction_[static_cast<size_t>(eCollisionType::MAX)][static_cast<size_t>(eCollisionType::MAX)])(const RSCollision& lhs, const RSCollision& rhs);

private:
	RSActor* actor_;
	eCollisionType collisionType_;
	int groupIndex_;
	float4 pivot_;
	float4 size_;

	RSImage* image_;
	DWORD checkColor_;

	float radius_;
	FRect rect_;
};

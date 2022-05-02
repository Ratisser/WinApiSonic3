#pragma once
#pragma once

#include <math.h>
#include <random>

#define PI 3.141592f
#define TODEG(a) (a * 57.295779f) // a/(2*PI)*360
#define TORAD(a) (a * 0.017453f) // a/180*PI
class float4;
typedef class float4 FRect;
class float4
{
public:
	static const float4 LEFT;
	static const float4 RIGHT;
	static const float4 UP;
	static const float4 DOWN;
	static const float4 ZERO;

public:
	union
	{
		struct
		{
			float x;
			float y;
			float z;
			float w;
		};
		struct
		{
			float left;
			float top;
			float right;
			float bottom;
		};
	};

	static float4 Lerp(float4 _startPos, float4 _endPos, float _Ratio, bool _isRatioCheck = true)
	{
		float4 Len = _endPos - _startPos;

		if (true == _isRatioCheck)
		{
			if (0 >= _Ratio)
			{
				_Ratio = 0;
			}

			if (1.0f <= _Ratio)
			{
				_Ratio = 1.0f;
			}
		}

		return _startPos + Len * _Ratio;
	}

	// CW dgree from -y axis
	__forceinline float GetDegreeYAxis() {
		return TODEG((float)(((x < 0) ? TORAD(360.0f) : 0.0f) + (acos(-y) * ((x < 0) ? -1 : 1))));
	}

	// counter clockwise dgree from right X axis
	__forceinline float GetDegree() {
		return TODEG((float)(((-y < 0) ? TORAD(360.0f) : 0.0f) + (acos(x) * ((-y < 0) ? -1 : 1))));
	}

	__forceinline float GetDegreeLeftXAxis() {
		return TODEG((float)(((-y < 0) ? TORAD(360.0f) : 0.0f) + (acos(-x) * ((-y < 0) ? -1 : 1))));
	}

	__forceinline float DistanceTo(const float4& other) {
		return sqrtf((x - other.x) * (x - other.x) + (y - other.y) * (y - other.y) + (z - other.z) * (z - other.z));
	}

	__forceinline float4 Direction(const float4& _other) {
		float length;
		float4 ret;
		ret.x = _other.x - x;
		ret.y = _other.y - y;
		ret.z = _other.z - z;
		length = ret.x * ret.x + ret.y * ret.y + ret.z * ret.z;
		ret.x = ret.x * ret.x * ((ret.x < 0) ? -1 : 1);
		ret.y = ret.y * ret.y * ((ret.y < 0) ? -1 : 1);
		ret.z = ret.z * ret.z * ((ret.z < 0) ? -1 : 1);
		ret.x /= length;
		ret.y /= length;
		ret.z /= length;
		return ret;
	}

	__forceinline float Length() {
		return sqrtf(x * x + y * y + z * z);
	}

	// this rotate is clockwise when +number, counter clockwise when -number
	__forceinline float4 Rotate2DByDegree(float _degree) {
		float fRadian = TORAD(_degree);
		float4 ret;
		ret.x = x * cosf(fRadian) - y * sinf(fRadian);
		ret.y = x * sinf(fRadian) + y * cosf(fRadian);
		return ret;
	}

	// this rotate is clockwise when +number, counter clockwise when -number
	__forceinline float4 Rotate2DByRadian(float _radian) {
		float4 ret;
		ret.x = x * cosf(_radian) - y * sinf(_radian);
		ret.y = x * sinf(_radian) + y * cosf(_radian);
		return ret;
	}

	float4 operator+(const float4 _other) const
	{
		float4 ReturnValue;

		ReturnValue.x = this->x + _other.x;
		ReturnValue.y = this->y + _other.y;
		ReturnValue.z = this->z + _other.z;
		ReturnValue.w = this->w + _other.w;
		return ReturnValue;
	}

	float4 operator-(const float4 _other) const
	{
		float4 ReturnValue;

		ReturnValue.x = this->x - _other.x;
		ReturnValue.y = this->y - _other.y;
		ReturnValue.z = this->z - _other.z;
		ReturnValue.w = this->w - _other.w;
		return ReturnValue;
	}

	float4 operator*(const float _other) const
	{
		float4 ReturnValue;

		ReturnValue.x = this->x * _other;
		ReturnValue.y = this->y * _other;
		ReturnValue.z = this->z * _other;
		ReturnValue.w = this->w * _other;
		return ReturnValue;
	}



	float4 operator*(const float4 _other) const
	{
		float4 ReturnValue;

		ReturnValue.x = this->x * _other.x;
		ReturnValue.y = this->y * _other.y;
		ReturnValue.z = this->z * _other.z;
		ReturnValue.w = this->w * _other.w;
		return ReturnValue;
	}

	float4 operator/(const float4 _other) const
	{
		float4 ReturnValue;

		ReturnValue.x = this->x / _other.x;
		ReturnValue.y = this->y / _other.y;
		ReturnValue.z = this->z / _other.z;
		ReturnValue.w = this->w / _other.w;
		return ReturnValue;
	}


	float4& operator+=(const float4 _other)
	{
		this->x += _other.x;
		this->y += _other.y;
		this->z += _other.z;
		this->w += _other.w;
		return *this;
	}

	float4& operator-=(const float4 _other)
	{
		this->x -= _other.x;
		this->y -= _other.y;
		this->z -= _other.z;
		this->w -= _other.w;
		return *this;
	}

	float4& operator*=(const float4 _other)
	{
		this->x *= _other.x;
		this->y *= _other.y;
		this->z *= _other.z;
		this->w *= _other.w;
		return *this;
	}

	float4& operator/=(const float4 _other)
	{
		this->x /= _other.x;
		this->y /= _other.y;
		this->z /= _other.z;
		this->w /= _other.w;
		return *this;
	}

	// 대입연산자
	float4& operator=(const float4& _other)
	{
		x = _other.x;
		y = _other.y;
		z = _other.z;
		w = _other.w;

		return *this;
	}

public:
	int ix() const
	{
		return static_cast<int>(x);
	}

	int iy() const
	{
		return static_cast<int>(y);
	}

	int iz() const
	{
		return static_cast<int>(z);
	}


	float hx() const
	{
		return x * 0.5f;
	}

	float hy() const
	{
		return y * 0.5f;
	}

	float hz() const
	{
		return z * 0.5f;
	}

	float4 halffloat4() const
	{
		return float4(hx(), hy(), hz());
	}


	int ihx() const
	{
		return static_cast<int>(hx());
	}

	int ihy() const
	{
		return static_cast<int>(hy());
	}

	int ihz() const
	{
		return static_cast<int>(hz());
	}



public:
	float4()
		: x(0.0f), y(0.0f), z(0.0f), w(1.0f)
	{
	}

	float4(float _x, float _y)
		: x(_x), y(_y), z(0.0f), w(1.0f)
	{
	}

	float4(float _x, float _y, float _z, float _w = 1.0f)
		: x(_x), y(_y), z(_z), w(_w)
	{
	}

	~float4() {

	}

public:		// delete constructer
	float4(const float4& _other)
		: x(_other.x), y(_other.y), z(_other.z), w(_other.w)
	{

	}

};

class RSRandom
{
public:
	RSRandom()
		: mt_()
	{
		std::random_device rng_;
		mt_.seed(rng_());
	}

	RSRandom(__int64 _Seed)
		: mt_(_Seed)
	{
	}

	~RSRandom()
	{
	}

public:
	int RandomInt(int _min, int _max)
	{
		std::uniform_int_distribution<int> Dis(_min, _max);
		return Dis(mt_);
	}

	float RandomFloat(float _min, float _max)
	{
		std::uniform_real_distribution<float> Dis(_min, _max);
		return Dis(mt_);
	}


private:
	std::mt19937_64 mt_;
};
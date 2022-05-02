#include "RSImage.h"
#include "RSWindow.h"

#include <RSMath.h>
#include <RSDebug.h>

#pragma comment(lib, "msimg32.lib ")

RSImage::RSImage()
	: imageHandle_(NULL)
	, oldHandle_(NULL)
	, bitmapInfo_({ 0, })
	, imageDC_(NULL)
{
}

RSImage::~RSImage()
{
	oldHandle_ = static_cast<HBITMAP>(SelectObject(imageDC_, oldHandle_));

	DeleteObject(oldHandle_);
	DeleteDC(imageDC_);
}

inline HDC RSImage::GetDC() const
{
	return imageDC_;
}

inline float4 RSImage::GetSize()
{
	return { static_cast<float>(bitmapInfo_.bmWidth), static_cast<float>(bitmapInfo_.bmHeight) };
}

bool RSImage::Load(std::string _path)
{
	imageHandle_ = static_cast<HBITMAP>(LoadImageA(NULL, _path.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));

	if (NULL == imageHandle_)
	{
		RSDebug::AssertFalse();
		return false;
	}

	imageDC_ = CreateCompatibleDC(nullptr);

	oldHandle_ = static_cast<HBITMAP>(SelectObject(imageDC_, static_cast<HGDIOBJ>(imageHandle_)));

	getBitmapInfo();

	return true;
}

bool RSImage::SetWindowDCHandle(HDC _hdc, const float4& _renderSize)
{
	if (NULL == _hdc)
	{
		RSDebug::AssertFalse();
		return false;
	}

	imageDC_ = _hdc;

	bitmapInfo_.bmWidth = _renderSize.ix();
	bitmapInfo_.bmHeight = _renderSize.iy();

	return true;
}

bool RSImage::CreateBitmap(HDC _hdc, const float4& _size)
{
	if (nullptr == _hdc)
	{
		RSDebug::AssertFalse();
		return false;
	}

	imageHandle_ = CreateCompatibleBitmap(_hdc, _size.ix(), _size.iy());
	imageDC_ = CreateCompatibleDC(nullptr);

	oldHandle_ = static_cast<HBITMAP>(SelectObject(imageDC_, imageHandle_));

	getBitmapInfo();

	return true;
}

void RSImage::ClearImage()
{
	PatBlt(imageDC_, 0, 0, bitmapInfo_.bmWidth, bitmapInfo_.bmHeight, BLACKNESS);
}

void RSImage::BitCopy(RSImage* _src, const float4& _leftTop, const float4& _size, const float4& _imagePos)
{
	BitBlt(imageDC_, _leftTop.ix(), _leftTop.iy(), _size.ix(), _size.iy(), _src->GetDC(), _imagePos.ix(), _imagePos.iy(), SRCCOPY);
}

void RSImage::BitCopyWithCenterPoint(RSImage* _src, const float4& _center, const float4& _size, const float4& _imagePos)
{
	BitBlt(imageDC_, _center.ix() - _size.ihx(), _center.iy() - _size.ihy(), _size.ix(), _size.iy(), _src->GetDC(), _imagePos.ix(), _imagePos.iy(), SRCCOPY);
}

void RSImage::BitCopyWithBottomPoint(RSImage* _src, const float4& _bottom, const float4& _size, const float4& _imagePos)
{
	BitBlt(imageDC_, _bottom.ix() - _size.ihx(), _bottom.iy() - _size.iy(), _size.ix(), _size.iy(), _src->GetDC(), _imagePos.ix(), _imagePos.iy(), SRCCOPY);
}

void RSImage::BitCopyByImageSize(RSImage* _src, const float4& _leftTop, const float4& _imagePos)
{
	BitBlt(imageDC_,
		_leftTop.ix(), _leftTop.iy(),
		_src->GetSize().ix(), _src->GetSize().iy(),
		_src->GetDC(),
		_imagePos.ix(), _imagePos.iy(),
		SRCCOPY);
}

void RSImage::BitCopyByImageSizeWithCenterPoint(RSImage* _src, const float4& _center, const float4& _imagePos)
{
	BitBlt(imageDC_,
		_center.ix() - _src->GetSize().ihx(), _center.iy() - _src->GetSize().ihy(),
		_src->GetSize().ix(), _src->GetSize().iy(),
		_src->GetDC(),
		_imagePos.ix(), _imagePos.iy(),
		SRCCOPY);
}

void RSImage::BitCopyByImageSizeWithBottomPoint(RSImage* _src, const float4& _bottom, const float4& _imagePos)
{
	BitBlt(imageDC_,
		_bottom.ix(), _bottom.iy() - _src->GetSize().ihy(),
		_src->GetSize().ix(), _src->GetSize().iy(),
		_src->GetDC(),
		_imagePos.ix(), _imagePos.iy(),
		SRCCOPY);
}

void RSImage::TransparentCopy(RSImage* _src, const float4& _leftTop, const float4& _size, const float4& _imagePos, const float4& _imageSize, UINT _transColor)
{
	TransparentBlt(imageDC_,
		_leftTop.ix(),
		_leftTop.iy(),
		_size.ix(),
		_size.iy(),
		_src->GetDC(),
		_imagePos.ix(),
		_imagePos.iy(),
		_imageSize.ix(),
		_imageSize.iy(),
		_transColor
	);
}

void RSImage::AlphaCopy(RSImage* _src, const float4& _leftTop, const float4& _size, const float4& _imagePos, const float4& _imageSize, int _alpha)
{
	BLENDFUNCTION blend;
	blend.BlendOp = AC_SRC_OVER;
	blend.BlendFlags = 0;
	blend.SourceConstantAlpha = _alpha;
	blend.AlphaFormat = AC_SRC_ALPHA;

	AlphaBlend(imageDC_,
		_leftTop.ix(),
		_leftTop.iy(),
		_size.ix(),
		_size.iy(),
		_src->GetDC(),
		_imagePos.ix(),
		_imagePos.iy(),
		_imageSize.ix(),
		_imageSize.iy(),
		blend
	);
}

void RSImage::PlgCopy(RSImage* _src, const float4& _leftTop, const float4& _size, const float4& _imagePos, const float4& _imageSize, RSImage* _maskImage, float _rotate)
{
	float4 fPoints[3];
	POINT points[3];

	fPoints[0] = _leftTop;
	fPoints[1] = _leftTop;
	fPoints[2] = _leftTop;
	fPoints[1].x += _size.x;
	fPoints[2].y += _size.y;

	for (size_t i = 0; i < 3; i++)
	{
		fPoints[i] = fPoints[i] - _leftTop - _size.halffloat4();
		fPoints[i] = fPoints[i].Rotate2DByDegree(-_rotate);
		fPoints[i] = fPoints[i] + _leftTop + _size.halffloat4();
	}

	for (size_t i = 0; i < 3; i++)
	{
		points[i].x = static_cast<int>(fPoints[i].x);
		points[i].y = static_cast<int>(fPoints[i].y);
	}

	if (nullptr == _maskImage)
	{
		PlgBlt(imageDC_,
			points,
			_src->imageDC_,
			_imagePos.ix(),
			_imagePos.iy(),
			_imageSize.ix(),
			_imageSize.iy(),
			nullptr,
			_imagePos.ix(),
			_imagePos.iy()
		);
	}
	else
	{
		PlgBlt(imageDC_,
			points,
			_src->imageDC_,
			_imagePos.ix(),
			_imagePos.iy(),
			_imageSize.ix(),
			_imageSize.iy(),
			_maskImage->imageHandle_,
			_imagePos.ix(),
			_imagePos.iy()
		);
	}
}

void RSImage::StretchCopyByImageSize(RSImage* _src, const float4& _leftTop, const float4& _imagePos)
{
	StretchBlt(imageDC_, 0, 0, bitmapInfo_.bmWidth, bitmapInfo_.bmHeight,
		_src->GetDC(),
		_leftTop.ix(), _leftTop.iy(),
		_src->GetSize().ix(), _src->GetSize().iy(),
		SRCCOPY);
}

float4 RSImage::GetCutImagePos(size_t _index)
{
	if (_index >= cutImagePos_.size())
	{
		RSDebug::AssertFalse();
		return float4::ZERO;
	}
	return cutImagePos_[_index];
}

float4 RSImage::GetCutImageSize(size_t _index)
{
	if (_index >= cutImageSize_.size())
	{
		RSDebug::AssertFalse();
		return float4::ZERO;
	}
	return cutImageSize_[_index];
}

DWORD RSImage::GetPixelColor(int _x, int _y)
{
	return GetPixel(imageDC_, _x, _y);
}

bool RSImage::IsCut()
{
	return cutImagePos_.size() > 0;
}

void RSImage::CutImage(const float4 _cutSize)
{
	if (0.0f >= _cutSize.x)
	{
		RSDebug::AssertFalse();
		return;
	}

	if (0.0f >= _cutSize.y)
	{
		RSDebug::AssertFalse();
		return;
	}

	size_t countX = static_cast<size_t>(GetSize().x / _cutSize.x);
	size_t countY = static_cast<size_t>(GetSize().y / _cutSize.y);

	size_t totalCount = countX * countY;

	float4 startImagePos = float4::ZERO;

	for (size_t y = 0; y < countY; y++)
	{
		for (size_t x = 0; x < countX; x++)
		{
			startImagePos.x = x * _cutSize.x;
			startImagePos.y = y * _cutSize.y;
			cutImagePos_.push_back(float4(startImagePos));
			cutImageSize_.push_back(float4(_cutSize));
		}
	}
}


void RSImage::getBitmapInfo()
{
	if (nullptr == imageDC_)
	{
		RSDebug::AssertFalse();
		return;
	}

	HBITMAP CheckBitMap = static_cast<HBITMAP>(GetCurrentObject(imageDC_, OBJ_BITMAP));
	GetObjectA(CheckBitMap, sizeof(BITMAP), &bitmapInfo_);
}

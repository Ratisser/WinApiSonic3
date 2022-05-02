#pragma once

#include <Windows.h>
#include <RSMath.h>
#include <string>
#include <vector>

// 분류 :
// 용도 :
// 설명 :
class RSResourceManager;
class RSImage
{
	friend RSResourceManager;
public:

	~RSImage();

	RSImage(const RSImage& _other) = delete;
	RSImage(RSImage&& _other) = delete;

	RSImage& operator=(const RSImage& _other) = delete;
	RSImage& operator=(const RSImage&& _other) = delete;

	HDC GetDC() const;
	float4 GetSize();

	bool Load(std::string _path);
	bool SetWindowDCHandle(HDC _hdc, const float4& _renderSize);
	bool CreateBitmap(HDC _hdc, const float4& _size);

	void ClearImage();
	void BitCopy(RSImage* _src, const float4& _leftTop, const float4& _size, const float4& _imagePos = float4::ZERO);
	void BitCopyWithCenterPoint(RSImage* _src, const float4& _center, const float4& _size, const float4& _imagePos = float4::ZERO);
	void BitCopyWithBottomPoint(RSImage* _src, const float4& _bottom, const float4& _size, const float4& _imagePos = float4::ZERO);

	void BitCopyByImageSize(RSImage* _src, const float4& _leftTop, const float4& _imagePos = float4::ZERO);
	void BitCopyByImageSizeWithCenterPoint(RSImage* _src, const float4& _center, const float4& _imagePos = float4::ZERO);
	void BitCopyByImageSizeWithBottomPoint(RSImage* _src, const float4& _bottom, const float4& _imagePos = float4::ZERO);

	void TransparentCopy(RSImage* _src, const float4& _leftTop, const float4& _size, const float4& _imagePos, const float4& _imageSize, UINT _transColor);
	void AlphaCopy(RSImage* _src, const float4& _leftTop, const float4& _size, const float4& _imagePos, const float4& _imageSize, int _alpha);

	void PlgCopy(RSImage* _src, const float4& _leftTop, const float4& _size, const float4& _imagePos, const float4& _imageSize, RSImage* _maskImage, float _rotate);


	void StretchCopyByImageSize(RSImage* _src, const float4& _leftTop, const float4& _imagePos = float4::ZERO);

	float4 GetCutImagePos(size_t _index);
	float4 GetCutImageSize(size_t _index);
	DWORD GetPixelColor(int _x, int _y);

	bool IsCut();
	void CutImage(const float4 _cutSize);

private:
	RSImage();
	void getBitmapInfo();

private:
	HBITMAP imageHandle_;
	HBITMAP oldHandle_;
	BITMAP bitmapInfo_;
	HDC imageDC_;

	std::vector<float4> cutImagePos_;
	std::vector<float4> cutImageSize_;

};


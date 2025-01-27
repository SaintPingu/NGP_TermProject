#pragma once
#include "stdafx.h"

static const COLORREF transRGB = RGB(145, 91, 145);
const HBRUSH transBrush = CreateSolidBrush(transRGB);

class Image abstract {
private:
	HBITMAP hBitmap{};
	BLENDFUNCTION bFunction{};

protected:
	RECT rectImage = { 0, };
	POINT drawSize = { 0, };
	void Load(const WCHAR* fileName, const POINT& imgSize, BYTE alpha = 0xff);
	void Render(const HDC& hdc, const RECT& rectDraw, const RECT& rectImage);
	float scaleX = 1;
	float scaleY = 1;

public:
	void RenderRotation(const HDC& hdc, Vector2 vPoints[4], const RECT* rectImage = nullptr);
	void SetAlpha(BYTE alpha);

	inline RECT GetRectImage() const
	{
		return rectImage;
	}
	inline POINT GetDrawSize() const
	{
		return drawSize;
	}
	inline void GetScale(float& scaleX, float& scaleY) const
	{
		scaleX = this->scaleX;
		scaleY = this->scaleY;
	}
	inline BYTE GetAlpha() const
	{
		return bFunction.SourceConstantAlpha;
	}
	inline void ReduceAlpha(BYTE amount)
	{
		CheckOverflowSub(bFunction.SourceConstantAlpha, amount);
	}
	inline void IncreaseAlpha(BYTE amount)
	{
		CheckOverflowAdd(bFunction.SourceConstantAlpha, amount);
	}
};

class ObjectImage : public Image {
private:
	POINT bodyDrawPoint = { 0, };
	POINT bodySize = { 0, };
	bool isScaled = false;

public:
	void Load(const WCHAR* fileName, const POINT& imgSize, const POINT& bodyDrawPoint = { 0, 0 }, const POINT& bodySize = { 0, 0 });
	void Render(const HDC& hdc, const RECT& rectBody, const RECT* rectImage = nullptr);
	void Render(const RECT& rectDest, const HDC& hdc);
	void ScaleImage(float scaleX, float scaleY);

	inline POINT GetBodyDrawPoint() const
	{
		return bodyDrawPoint;
	}
	inline POINT GetBodySize() const
	{
		return bodySize;
	}
};

class EffectImage : public Image {
private:
	int maxFrame = 0;
public:
	void Load(const WCHAR* fileName, const POINT& imgSize, int maxFrame = 0, BYTE alpha = 0xff);
	void Render(const HDC& hdc, const POINT& drawPoint, const RECT* rectImage = nullptr);
	void Render(const HDC& hdc, const RECT& rectDest, const RECT* rectImage = nullptr);
	void ScaleImage(float scaleX, float scaleY);

	inline int GetMaxFrame() const
	{
		return maxFrame;
	}
};


class GUIImage : public Image {
public:
	void Load(const WCHAR* fileName, const POINT& imgSize, BYTE alpha = 0xff);
	void Render(const HDC& hdc, const RECT& rectDest);
	void RenderBlack(const HDC& hdc, const RECT& rectDest);
	void RenderGauge(const HDC& hdc, const RECT& rectDest, float current, float max);
};


class ISprite abstract {
protected:
	int frame = 0;
public:
	static RECT GetRectImage(const Image& image, int frame, int spriteRow = 0);
};

class IAnimatable abstract : public ISprite {
private:
	Action action = Action::Idle;
protected:
	bool isRevFrame = false;

	inline Action GetAction() const
	{
		return action;
	}
	inline void SetAction(Action action, int frame)
	{
		this->frame = frame;
		this->action = action;
		this->isRevFrame = false;
	}
};
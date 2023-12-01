#pragma once

class Image abstract {
protected:
	RECT rectImage = { 0, };
	POINT drawSize = { 0, };
	void Load(const POINT& imgSize);
	float scaleX = 1;
	float scaleY = 1;

public:

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
};

class ObjectImage : public Image {
private:
	POINT bodyDrawPoint = { 0, };
	POINT bodySize = { 0, };
	bool isScaled = false;

public:
	void Load(const POINT& imgSize, const POINT& bodySize = { 0, 0 });
	void ScaleImage(float scaleX, float scaleY);

	inline POINT GetBodySize() const
	{
		return bodySize;
	}
};

class EffectImage : public Image {
private:
	int maxFrame = 0;
public:
	void Load(const POINT& imgSize, int maxFrame = 0/*, BYTE alpha = 0xff*/);
	void ScaleImage(float scaleX, float scaleY);

	inline int GetMaxFrame() const
	{
		return maxFrame;
	}
};

class ISprite abstract {
protected:
	int frame = 0;
	RECT GetRectImage(const Image& image, int frame, int spriteRow = 0) const;
};

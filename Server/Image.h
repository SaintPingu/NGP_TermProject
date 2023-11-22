#pragma once

class Image abstract {
protected:
	RECT rectImage = { 0, };
	void Load();
	float scaleX = 1;
	float scaleY = 1;

public:

	inline RECT GetRectImage() const
	{
		return rectImage;
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
	void Load(const POINT& bodySize = { 0, 0 });
	void ScaleImage(float scaleX, float scaleY);

	inline POINT GetBodySize() const
	{
		return bodySize;
	}
};
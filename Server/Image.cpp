#include "stdafx.h"
#include "Image.h"


void Image::Load(const POINT& imgSize)
{
	this->rectImage = { 0, 0, imgSize.x, imgSize.y };
	drawSize.x = rectImage.right - rectImage.left;
	drawSize.y = rectImage.bottom - rectImage.top;
}

void ObjectImage::Load(const POINT& imgSize, const POINT& bodySize)
{
	Image::Load(imgSize);
	this->bodySize = bodySize;
}


void ObjectImage::ScaleImage(float scaleX, float scaleY)
{
	if (isScaled == true)
	{
		return;
	}
	isScaled = true;

	bodySize.x = (LONG)((float)bodySize.x * scaleX);
	bodySize.y = (LONG)((float)bodySize.y * scaleY);

	this->scaleX = scaleX;
	this->scaleY = scaleY;
}

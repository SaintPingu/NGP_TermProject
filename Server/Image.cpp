#include "stdafx.h"
#include "Image.h"


void Image::Load()
{

}

void ObjectImage::Load(const POINT& bodySize)
{
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

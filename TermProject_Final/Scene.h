#pragma once
#include "stdafx.h"
#include "Image.h"

class Scene {
public:
	virtual void Init() abstract;
	virtual void Render(HDC hdc) abstract;
	virtual void Animate() abstract;
};

#pragma once
#include "stdafx.h"
#include "Image.h"

class Scene {
public:
	virtual void Init() abstract;
	virtual void Render(HDC hdc) abstract;
	virtual void Animate() abstract;
	virtual void GetInput(CommandList* cmdList) abstract;

	virtual void WriteData(void* data) abstract;
	virtual bool ProcessCommand() abstract;
};

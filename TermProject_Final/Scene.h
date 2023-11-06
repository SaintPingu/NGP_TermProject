#pragma once
class Scene {
public:
	virtual void Init() abstract;
	virtual void Render(HDC hdc, const RECT& rectWindow) abstract;
	virtual void Animate() abstract;
};
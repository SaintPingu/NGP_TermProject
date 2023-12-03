#pragma once
#include "Image.h"

class GameObject abstract {
private:
	ObjectImage image;

	Vector2 posCenter;
	POINT bodySize = { 0, };
	FRECT rectBody = { 0, };

protected:
	Dir direction = Dir::Empty;

	GameObject() {};
	GameObject(ObjectImage image, const Vector2& pos);
	void Render(const HDC& hdc, const RECT* rectImage = nullptr);
	void Init(ObjectImage image, const Vector2& pos);
	

	inline const ObjectImage& GetImage()
	{
		return image;
	}

public:
	FRECT GetRectBody(const Vector2& pos) const;

	bool IsCollide(const RECT& rectSrc, RECT* lprcDst = nullptr) const;

	inline constexpr FRECT GetRectBody() const
	{
		return rectBody;
	}
	inline constexpr Vector2 GetPosCenter() const
	{
		return posCenter;
	}
	inline float GetBodyWidth() const
	{
		return rectBody.right - rectBody.left;
	}
	inline float GetBodyHeight() const
	{
		return rectBody.bottom - rectBody.top;
	}
	inline void SetObjectImage(ObjectImage img)
	{
		image = img;
	}
	void SetPos(const Vector2& pos);
};

class IMovable abstract {
private:
	virtual void SetPosDest() abstract;
	bool isMove = false;
public:
	virtual void Move() abstract;

	inline void StartMove()
	{
		isMove = true;
	}
	inline void StopMove()
	{
		isMove = false;
	}
	inline bool IsMove() const
	{
		return isMove;
	}
};

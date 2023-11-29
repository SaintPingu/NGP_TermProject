#pragma once
#include "image.h"

class GameObject abstract {
private:
	ObjectImage* image;

	Vector2 posCenter;
	POINT bodySize{};
	FRECT rectBody{};

protected:
	Dir direction = Dir::Empty;

	GameObject() {};
	GameObject(ObjectImage& image, const Vector2& pos);
	void Init(ObjectImage& image, const Vector2& pos);
	void SetPos(const Vector2& pos);

	inline const ObjectImage& GetImage()
	{
		return *image;
	}

public:
	FRECT GetRectBody(const Vector2& pos) const;

	bool IsCollide(const RECT& rectSrc, RECT* lprcDst = nullptr) const;

	inline FRECT GetRectBody() const
	{
		return rectBody;
	}
	inline Vector2 GetPosCenter() const
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
};

inline float GetRadius(float x, float y)
{
	return sqrtf((x * x) + (y * y));
}

class IControllable abstract {
private:
	virtual void SetPosDest() abstract;
	bool isMove = false;
public:
	virtual void SetMove() abstract;
	virtual void Move() abstract;
	virtual void Stop(Dir dir) abstract;

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

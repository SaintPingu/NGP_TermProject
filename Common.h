#pragma once
#include <cmath>
#include <vector>
#include <cassert>

#define WINDOWSIZE_X 500
#define WINDOWSIZE_Y 750
#define MSEC 1000

#define PI 3.141592
#define DEGREE_TO_RADIAN(degree) ((PI/180) * (degree))
#define RADIAN_TO_DEGREE(radian) ((180/PI) * (radian))


#define SINGLETON_PATTERN(TYPE)				\
private:									\
   static TYPE* mInst;						\
public:										\
   static TYPE* Inst()						\
   {										\
      if (!mInst) mInst = new TYPE;			\
      return mInst;							\
   }										\
   static void Destroy() {					\
      if (nullptr != mInst) {				\
         delete mInst;						\
         mInst = nullptr;					\
      }										\
   }                  

#define SINGLETON_PATTERN_DEFINITION(TYPE)  \
   TYPE* TYPE::mInst = nullptr;

// 2023-11-06-MON (장재문) - Common.h 에 추가 -> 서버-클라이언트 ( 패킷을 송수신에 쓰일 자료형 )
using BYTE   = unsigned char;
using int8   = __int8;
using int16  = __int16;
using int32  = __int32;
using int64  = __int64;
using uint8  = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;



enum class Scene { Intro = 0, Town, Stage, Phase, Battle };
enum class Action { Idle = 0, Attack, Hurt, Death };

enum class Pokemon { Null = 0, Moltres, Articuno, Thunder };
enum class SubPokemon { Null = 0, Pikachu = 0, Squirtle, Charmander };
enum class Type { Empty = 0, Fire, Elec, Water, Dark };
enum class Skill { Empty = 0, Identity, Sector, Circle };

enum class StageElement { Water = 0, Fire, Elec, Dark, Town, Null };


enum class Dir { Empty = 0, Left, Right, Up, Down, LD, LU, RD, RU };

// Direction 연산자 오버로딩
// ex) Left - LD = LD - Left = Down
//     Left + Up = LU
constexpr Dir operator-(Dir lhs, Dir rhs)
{
	if (lhs == rhs)
	{
		return Dir::Empty;
	}
	else if (lhs < rhs)
	{
		Dir temp = lhs;
		lhs = rhs;
		rhs = temp;
	}

	Dir result = Dir::Empty;
	switch (lhs)
	{
	case Dir::LD:
		if (rhs == Dir::Left)
		{
			result = Dir::Down;
		}
		else if (rhs == Dir::Down)
		{
			result = Dir::Left;
		}
		break;
	case Dir::LU:
		if (rhs == Dir::Left)
		{
			result = Dir::Up;
		}
		else if (rhs == Dir::Up)
		{
			result = Dir::Left;
		}
		break;
	case Dir::RD:
		if (rhs == Dir::Right)
		{
			result = Dir::Down;
		}
		else if (rhs == Dir::Down)
		{
			result = Dir::Right;
		}
		break;
	case Dir::RU:
		if (rhs == Dir::Right)
		{
			result = Dir::Up;
		}
		else if (rhs == Dir::Up)
		{
			result = Dir::Right;
		}
		break;
	default:
		break;
	}

	return result;
}
constexpr Dir operator+(Dir lhs, Dir rhs)
{
	if (lhs == rhs)
	{
		return lhs;
	}
	else if (lhs < rhs)
	{
		Dir temp = lhs;
		lhs = rhs;
		rhs = temp;
	}

	switch (rhs)
	{
	case Dir::Left:
	{
		switch (lhs)
		{
		case Dir::Up:	return Dir::LU;
		case Dir::Down:	return Dir::LD;
		}
	}
	break;
	case Dir::Right:
	{
		switch (lhs)
		{
		case Dir::Up:	return Dir::RU;
		case Dir::Down:	return Dir::RD;
		}
	}
	break;
	case Dir::Up:
	{
		switch (lhs)
		{
		case Dir::Left:		return Dir::LU;
		case Dir::Right:	return Dir::RU;
		}
	}
	break;
	case Dir::Down:
	{
		switch (lhs)
		{
		case Dir::Left:		return Dir::LD;
		case Dir::Right:	return Dir::RD;
		}
	}
	break;
	default:
		break;
	}

	return Dir::Empty;
}



template <typename T>
inline constexpr int GetSign(T num)
{
	return num / abs(num);
}


// POINT 연산자 오버로딩
// ex) POINT a,b;
// possible : a - b, a + b;
inline constexpr POINT operator-(const POINT& lhs, const POINT& rhs)
{
	return { lhs.x - rhs.x, lhs.y - rhs.y };
}
inline constexpr POINT operator+(const POINT& lhs, const POINT& rhs)
{
	return { lhs.x + rhs.x, lhs.y + rhs.y };
}

struct Vector2 {
	float x = 0;
	float y = 0;

	inline constexpr Vector2 operator+(const Vector2& rhs) const
	{
		return { x + rhs.x, y + rhs.y };
	}
	inline constexpr Vector2 operator-(const Vector2& rhs) const
	{
		return { x - rhs.x, y - rhs.y };
	}
	inline constexpr Vector2 operator*(const float& rhs) const
	{
		return { x * rhs, y * rhs };
	}
	inline constexpr Vector2 operator/(const float& rhs) const
	{
		return { x / rhs, y / rhs };
	}
	inline constexpr operator POINT () const
	{
		return { (LONG)x, (LONG)y };
	}
	inline constexpr Vector2 operator=(const POINT& rhs)
	{
		return { static_cast<float>(rhs.x), static_cast<float>(rhs.y) };
	}
	inline constexpr bool operator==(const Vector2& rhs) const
	{
		return (x == rhs.x && y == rhs.y) ? true : false;
	}
	inline constexpr bool operator!=(const Vector2& rhs) const
	{
		return (x != rhs.x || y != rhs.y) ? true : false;
	}

	inline Vector2 Normalized() const
	{
		return *this / GetNorm(*this);
	}
	inline float Norm() const
	{
		return sqrtf(x * x + y * y);
	}
	static inline float GetNorm(Vector2 v)
	{
		return sqrtf(v.x * v.x + v.y * v.y);
	}
	static inline constexpr float Dot(const Vector2& lhs, const Vector2& rhs)
	{
		return (lhs.x * rhs.x) + (lhs.y * rhs.y);
	}
	static inline constexpr float GetZAxis(const Vector2& lhs, const Vector2& rhs)
	{
		return (lhs.x * rhs.y) - (rhs.x - lhs.y);
	}
	static inline float GetTheta(const Vector2& lhs, const Vector2& rhs)
	{
		float dot = Vector2::Dot(lhs, rhs);
		return acos(dot / (Vector2::GetNorm(lhs) * Vector2::GetNorm(rhs)));
	}

	static inline constexpr Vector2 Up()
	{
		return { 0, -1 };
	}
	static inline constexpr Vector2 Down()
	{
		return { 0, 1 };
	}
	static inline constexpr Vector2 Left()
	{
		return { -1, 0 };
	}
	static inline constexpr Vector2 Right()
	{
		return { 1, 0 };
	}
	static inline constexpr Vector2 Zero()
	{
		return { 0, 0 };
	}
	static inline constexpr Vector2 GetDest(Vector2 posCenter, Vector2 vector, float speed = 1)
	{
		return posCenter + (vector * speed);
	}
	static Vector2 Lerp(const Vector2& src, const Vector2& dst, float alpha)
	{
		Vector2 transform;
		transform.x = (src.x * (1 - alpha)) + (dst.x * alpha);
		transform.y = (src.y * (1 - alpha)) + (dst.y * alpha);
		return transform;
	}
	// Linear transform to find the orthogonal vector of the edge
	static Vector2 Get_Normalized_Proj_Axis(const Vector2& crntPoint, const Vector2& nextPoint)
	{
		const float axisX = -(nextPoint.y - crntPoint.y);
		const float axisY = nextPoint.x - crntPoint.x;
		const float magnitude = hypot(axisX, axisY);

		Vector2 axisNormalized = { 0, };
		axisNormalized.x = axisX / magnitude;
		axisNormalized.y = axisY / magnitude;

		return axisNormalized;
	}
};

typedef struct FRECT {
	float left = 0;
	float top = 0;
	float right = 0;
	float bottom = 0;

	inline constexpr operator RECT () const
	{
		return { (LONG)left, (LONG)top, (LONG)right, (LONG)bottom };
	}
	inline constexpr FRECT operator=(const RECT& rhs)
	{
		return { (float)rhs.left, (float)rhs.right, (float)rhs.top, (float)rhs.bottom };
	}
}FRECT;


Vector2 Rotate(Vector2 vector, float degree);
void Rotate(const Vector2& vSrc, const Vector2& vDst, Vector2& unitVector, float t);

bool OutOfRange(const RECT& rect, const RECT& rectRange);
void GetRotationPos(const RECT& rect, const Vector2& unitVector, Vector2 basisVector, Vector2 vPoints[4]);

void SetRectByWindow(RECT& rect);

RECT GetRotatedBody(const Vector2 vPoints[4]);

void PaintHitbox(HDC hdc, RECT rectBody);

void CheckOverflowAdd(BYTE& lhs, const BYTE& rhs);
void CheckOverflowSub(BYTE& lhs, const BYTE& rhs);
void ComputeProjections(const std::vector<Vector2>& bounds_a, const std::vector<Vector2>& bounds_b, const Vector2& axisNormalized, std::vector<double>& projections_a, std::vector<double>& projections_b);
bool IsOverlapping(const std::vector<double>& projections_a, const std::vector<double>& projections_b);

bool SATIntersect(const FRECT& rectSrc, const Vector2 vSrc[4]);
void ScaleRect(FRECT& rect, float scaleX, float scaleY);
FRECT GetRect(const Vector2& posCenter, float radius);
bool IntersectRect2(const RECT& rect1, const RECT& rect2);
float CalculateDamage(float damage, Type destType, Type srcType);

#pragma once
#pragma warning(disable:4018)
#pragma warning(disable:4244)
#pragma warning(disable:4267)
#include <cmath>
#include <vector>
#include <cassert>
#include <memory>
#include <string>
// 2023-11-19-SUN (장재문) - 서버/클라이언트 네트워크를 위한 mutex/atomic 라이브러리 추가  
#include <mutex>
#include <atomic>

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

// 2023-11-06-MON (장재문) - Common.h 에 추가 -> 서버-클라이언트 ( 패킷 송수신에 쓰일 자료형 )
using BYTE   = unsigned char;
using int8   = __int8;
using int16  = __int16;
using int32  = __int32;
using int64  = __int64;
using uint8  = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;
constexpr size_t size_uint32 = sizeof(uint32);

// 2023-11-14-TUE (민동현) - Common.h 에 추가 -> Command
using Command = BYTE;
using PacketBuffer = std::vector<BYTE>;
using Packet = std::vector<BYTE>;

template<typename T>
using Atomic = std::atomic<T>;
using Mutex = std::mutex;

// 2023-11-06-MON (장재문) - Common.h 에 추가 -> 서버/클라이언트 네트워크에서 쓰일 TResult enum 
/// +------------------
///	 SERVER ERROR CODE 
/// ------------------+	
enum class TResult : short
{
	NONE = 0,
	FAIL,
	SUCCESS,

	SOCKET_EXISTED,							// 소켓이 이미 존재함
	ERROR_CLOSE_SOCKET,						// 소켓 닫기 에러
	CLIENT_CAN_NOT_ACCEPT_ANYMORE,			// 클라이언트를 더이상 받을 수 없음
	CLIENT_DISCONNECT,						// 클라이언트 연결 끊어짐 
	ERROR_SYNC_ISSUE,						// 동기화 문제 오류

	// SOCKET 관련 FAIL
	SERVER_SOCKET_CREATE_FAIL,
	SERVER_SOCKET_CONNECT_FAIL,
	SERVER_SOCKET_BIND_FAIL,
	SERVER_SOCKET_LISTEN_FAIL,
	ACCEPT_API_ERROR,

	// RECV 관련 FAIL 
	RECV_API_ERROR,
	RECV_REMOTE_CLOSE,

	// SEND 관련 FAIL
	SEND_REMOTE_CLOSE,
	SEND_SIZE_ZERO,
	SEND_ERROR,

	// CLIENT 관련 FAIL
	CLIENT_NOT_CONNECTED,  
	FORCING_CLOSE,

	// 12-03 민동현 : 수신취소를 위한 코드 필요
	// 송/수신 관련
	CANCLE_RECV								// 수신 취소
};

//  2023-11-19-SUN (장재문) 패킷 송수신 플래그
enum class ConnectFlag
{
	none,

	RecvStart,
	RecvFinish,

	SendStart,
	SendFinish,
	END,
};


enum class SceneType { Intro = 0, Lobby, Stage, Phase, Battle };
enum class Action { Idle = 0, Attack, Hurt, Death };

enum class Pokemon { Null = 0, Moltres, Articuno, Thunder };
enum class SubPokemon { Null = 0, Pikachu = 0, Squirtle, Charmander };
enum class Type { Empty = 0, Fire, Elec, Water, Dark };
enum class EnemyType { Melee = 0, Range, Boss };
enum class BulletType { Empty = 0, Main_Fire, Main_Elec, Main_Water, Sub_Fire, Sub_Elec, Sub_Water, Enemy, Boss, _count };
enum class EffectType { Empty = 0, Explode_Fire, Explode_Water, Explode_Elec, Explode_Dark, Cloud_Fire, Cloud_Water, Cloud_Elec, Cloud_Dark };
enum class Skill { Empty = 0, Identity, Sector, Circle };

enum class StageElement { Water = 0, Fire, Elec, Dark, Lobby, Null };


enum class Dir { Left = 0, Right, Up, Down, LD, LU, RD, RU, Empty};

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

void RenderHitbox(HDC hdc, RECT rectBody);

void CheckOverflowAdd(BYTE& lhs, const BYTE& rhs);
void CheckOverflowSub(BYTE& lhs, const BYTE& rhs);
void ComputeProjections(const std::vector<Vector2>& bounds_a, const std::vector<Vector2>& bounds_b, const Vector2& axisNormalized, std::vector<double>& projections_a, std::vector<double>& projections_b);
bool IsOverlapping(const std::vector<double>& projections_a, const std::vector<double>& projections_b);

bool SATIntersect(const FRECT& rectSrc, const Vector2 vSrc[4]);
void ScaleRect(FRECT& rect, float scaleX, float scaleY);
FRECT GetRect(const Vector2& posCenter, float radius);
bool IntersectRect2(const RECT& rect1, const RECT& rect2);
float CalculateDamage(float damage, Type destType, Type srcType);


class CommandList {
public:
	CommandList();
	~CommandList();

	std::vector<BYTE> buffer;

	void PushCommand(BYTE cmd, void* data, size_t size);

	std::vector<BYTE> GetCmdList();
};

template<class T>
void PushData(PacketBuffer& buffer, T* data, size_t size)
{
	buffer.insert(buffer.end(), data, data + size);
}


// 2023-11-14-TUE (민동현) - Types.h -> Common.h로 이동
/// +------------------
///	 SERVER LOBBY CMD
/// ------------------+	
enum class ServerLobbyCmd : BYTE
{
	None,
	Quit,
	GoStage,

};
/// +------------------
///	 SERVER STAGE CMD
/// ------------------+	
enum class ServerStageCmd : BYTE
{
	None,
	GoBattle,

};
/// +------------------
///	 SERVER BATTLE CMD
/// ------------------+	
enum class ServerBattleCmd : BYTE
{
	End,
	Loss,
	Win,
	AcceptSkillQ,
	Hit,
	UpdateMP,
	CreateEffect,
	BossPos,

};

/// +------------------
///	 CLIENT LOBBY CMD
/// ------------------+	
enum class ClientLobbyCmd : BYTE
{
	MoveLeft,
	MoveRight,
	MoveUp,
	MoveDown,
	Stop,

};
/// +------------------
///	 CLIENT STAGE CMD
/// ------------------+	
enum class ClientStageCmd : BYTE
{
	None,
	EnterStage,
	ExitStage,
	GoLobby,

};
/// +------------------
///	 CLIENT BATTLE CMD
/// ------------------+	
enum class ClientBattleCmd : BYTE
{
	MoveLeft,
	MoveRight,
	MoveUp,
	MoveDown,
	Stop,

	SkillQ,
	SkillW,
	SkillE,
};










const ULONG MAX_SAMPLE_COUNT = 50; // Maximum frame time sample count

class Timer
{
	SINGLETON_PATTERN(Timer)

public:
	Timer();
	virtual ~Timer();

	void Tick(float lockFPS = 0.0f);
	void Start();
	void Stop();
	void Reset();

	unsigned long GetFrameRate(std::wstring& string);
	float GetTimeElapsed() const { return mTimeElapsed; }
	float GetTotalTime();

private:
	double							mTimeScale{};
	float							mTimeElapsed{};

	__int64							mBasePerfCount{};
	__int64							mPausedPerfCount{};
	__int64							mStopPerfCount{};
	__int64							mCurrentPerfCount{};
	__int64							m_nLastPerfCount{};

	__int64							mPerfFreqPerSec{};

	float							mFrameTime[MAX_SAMPLE_COUNT]{};
	ULONG							m_nSampleCount{};

	unsigned long					mCrntFrameRate{};
	unsigned long					mFPS{};
	float							mFPSTimeElapsed{};

	bool							mIsStopped{};
};

inline float DeltaTime()
{
	return Timer::Inst()->GetTimeElapsed();
}


// 2023-11-22-WED (민동현) - Common.h 에 추가 -> rectWindow를 상수값으로 표현
constexpr RECT rectWindow{ 0, 0, 484, 711 };








// 소켓 함수 오류 출력
void err_display(const char* msg);









// 2023-11-06 (장재문) : 패킷 내부 데이터 생성 / bitset을 이용해 비트별로 쪼개가며 구현할 예정입니다.. + 기획서에 PlayerCount[n] 으로 되어있는데 여기서 왜 [n]이지..? + 최종 데이터에서는 vector 빼야함 - 보낼 거기 때문이지..
// 2023-11-08 (장재문) : 패킷 내부 데이터 정리 / 바이트 정리 - 세부 사항 업데이트 중..


/* +--------------------------------------------------------------------------
	LobbyData
	{
		* PlayerLobbyData
			[ 1 ][ 9 ][ 9 ]...[ 9 ]					( 1 + 9 * n ) BYTE
	}
   ---------------------------------------------------------------------------+ */

   /// +--------------
   ///	    LOBBY
   /// --------------+	
namespace Lobby
{
	/// +--------------------
	///	    PLAYER - LOBBY
	/// --------------------+	
	struct PlayerLobbyData // 9 BYTE
	{
		BYTE			Pid_Mov_Dir;	// 1 BYTE - ( 0b [7][6][5][4][3]PlayerID [2]IsMoving [1][0]Dir )
		Vector2			Pos;			// 4 BYTE + 4 BYTE
	};

	/// +--------------
	///	  LOBBY DATA
	/// --------------+	
	struct LobbyData        // 1 + ( 9 * n BYTE )
	{
		BYTE						 PlayerCnt;		// 2
		PlayerLobbyData* PlayersData;   // [Player1-LobbyData][Player2-LobbyData] 동적 생성  
	};

};

namespace Stage
{
	struct ClientStageData {
		BYTE	Fly_Gnd;
	};
}


/* +--------------------------------------------------------------------------
	BattleData
	{
		* PlayerBattleData
			[ 1 ][ 8 ]								( 1 + 8 ) BYTE

		* EnemyBattleData
			[ 1 ][ 9 ][ 9 ][ 9 ]...[ 9 ]			( 1 + 9 * n ) BYTE

		* BulletsBattleData
			[ 1 ][ 17 ][ 17 ][ 17 ]...[ 17 ]		( 1 + 17 * n ) BYTE

		* BossSkillBattleData
			[ 1 ][ 9 ][ 9 ][ 9 ]...[ 9 ]			( 1 + 9 * n ) BYTE
	}
   ---------------------------------------------------------------------------+ */


   /// +--------------
   ///	    BATTLE
   /// --------------+	
namespace Battle
{
	/// +--------------------
	///	    PALYER - BATTLE
	/// --------------------+	
	struct PlayerBattleData   // 9 BYTE
	{
		BYTE		PlayerID;
		Vector2		Pos;
	};

	struct EnemyBattleData
	{
		/// +--------------
		///	  ENEMY DATA
		/// --------------+	
		struct Data          // 9 BYTE
		{
			int			ID;
			BYTE		TypeDirActPad;    // 1 BYTE - ( 0b [7][6] Type [5][4] Dir [3][2] Action [1][0] Padding)
			Vector2		Pos;
		};

		BYTE					EnemyCnt;	// 1  BYTE
		Data* Enemies;     // [Data][Data][Data]...[Data] - EnemyCnt 개수 만큼 동적 배열 
	};

	/// +----------------------
	///	   BOSS SKILL - BATTLE
	/// ----------------------+	
	struct BossSkillBattleData
	{
		struct Data {
			BYTE	type;
			Vector2 pos;
		};
		BYTE					EffectCnt;  // 1  BYTE
		Data*					Effects;	// [Effect][Effect][Effect]...[Effect] - EffectCnt 개수 만큼 동적 배열 
	};

	/// +--------------
	///	  BULLET DATA
	/// --------------+	
	struct BulletsBattleData
	{
		/// +--------------------
		///	    BULLET - BATTLE
		/// --------------------+	
		struct Data	  // 17 BYTE
		{
			BYTE		bulletType;
			POINT		Pos;
			Vector2		Dir;
		};

		BYTE				    BulletCnt;		// 1  BYTE
		Data* BulletsData;	// [Data][Data]...[Data] - BulletCnt 개수 만큼 동적 배열 
	};


	/// +--------------
	///	  BATTLE DATA
	/// --------------+	
	const UINT MaxBattlePlayerCnt = 2;
	struct BattleData
	{
		PlayerBattleData		PlayerBattleData[MaxBattlePlayerCnt];
		EnemyBattleData			EnemyData;
		BulletsBattleData		BulletData;
		BossSkillBattleData		BossEffectData;
	};

	struct EffectData {
		BYTE	type;
		POINT	pos;
	};
};


// remove data from begin
inline void RemoveData(PacketBuffer& buffer, size_t size)
{
	buffer.erase(buffer.begin(), buffer.begin() + size);
}
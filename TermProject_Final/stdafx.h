#pragma once
#pragma warning(disable:28251)
#pragma warning(disable:4244)


// 2023-11-10-FRI (장재문) : 소켓 관련 헤더 삽입 + ** timeproc 에러나고 있습니다! **
#include <WinSock2.h>
#include <MSWSock.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#include <iostream>


#include <Windows.h>
#include <gdiplus.h>
#include <time.h>
#include <tchar.h>
#include <atlImage.h>
#include <mmsystem.h>
#include <fmod.hpp>
#include <algorithm>
#include <functional>
#include <bitset>
#include <set>
#include <deque>

#include <string>
#include <unordered_map>
#include <map>

#include "..\Common.h"

#define	WIN32_LEAN_AND_MEAN
#undef WINVER
#define WINVER 0x6000
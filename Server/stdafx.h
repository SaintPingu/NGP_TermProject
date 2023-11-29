#pragma once



// 2023-11-05-SUN (장재문) : Common.h 시 헤더 충돌 링크 오류가 발생해 임시로 껐습니다.. 오류 : [ LNK2005 ...에 이미 정의되어 있습니다. ]
// 2023-11-06-MON (장재문) : Common.h시 링크 오류 - 팀원 (민동현) 이 해결 후 Commit 해서 주석을 없애고 사용하기 시작하겠습니다.. 


// +---------------------------------
// * 소켓 관련 헤더 - <window.h> 이전에 선언된어야 한다.
// *                 warning : 'AF_IPX' 매크로 재정의 및 오류 코드들이 쏟아져 나온다. 
//					 이는 Window.h 와 Winsock.h 가 충돌이 나면서 생기는 경고 및 오류이다. 
#include <WinSock2.h>
#include <MSWSock.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
// ---------------------------------+

#include "Windows.h"
#include "define.h"
#include "Types.h"



#include <iostream>
#include <thread>
#include <bitset>


#include <list>
#include <queue>
#include <stack>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>




#include "../Common.h"
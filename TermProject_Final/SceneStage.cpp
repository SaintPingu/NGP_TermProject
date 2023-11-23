#include "stdafx.h"
#include "SceneStage.h"
#include "Framework.h"
#include "SceneManager.h"
#include "InputManager.h"

#define TARGET_IMAGESIZE_X 512
#define TARGET_IMAGESIZE_Y 512
#define CAMSIZE_X 162

#define STAGE_NUM 5

#define MAPSCROLL_SPEED 4

#define ALPHA 220

#define SELECTPOKEMONSIZE 135

#define VK_R 0x52

void SceneStage::Init()
{
}

void SceneStage::Render(HDC hdc)
{
}

void SceneStage::Animate()
{
}

void SceneStage::GetInput(CommandList* cmdList)
{
	RECT rect;
	target->_cam = { target->_rectDraw.left - CAMSIZE_X, rectWindow.top, target->_rectDraw.right + CAMSIZE_X, rectWindow.bottom };

	target->_select = false;
	target->_select_index = StageElement::Null;

	// 타겟이 맵 오브젝트 위에 올라가 있을 경우 선택 flag를  true로 설정
	for (int i = 0; i < STAGE_NUM; i++)
	{
		if (IntersectRect(&rect, &rectStage[i], &target->_rectDraw))
		{
			target->_select = true;
			target->_select_index = static_cast<StageElement>(i); // 타겟이 놓여있는 위치에 따라 인덱스를 바꿈 ex) 표적이 Dark면 index 값은 3
		}
	}

	if (KEY_PRESSED(VK_LEFT) && target->_rectDraw.left > rectWindow.left && !_select_pokemon)
	{
		target->_rectDraw.left -= MAPSCROLL_SPEED;
		target->_rectDraw.right -= MAPSCROLL_SPEED;
		_dialogflag = false;

		if (moveX > 0)
		{
			moveX -= 10;

			for (int i = 0; i < STAGE_NUM; i++)
			{
				rectStage[i].left += 10;
				rectStage[i].right += 10;
			}
		}
	}
	if (KEY_PRESSED(VK_RIGHT) && target->_rectDraw.right < rectWindow.right && !_select_pokemon)
	{
		target->_rectDraw.left += MAPSCROLL_SPEED;
		target->_rectDraw.right += MAPSCROLL_SPEED;
		_dialogflag = false;

		if (moveX < 450)
		{
			moveX += 10;

			for (int i = 0; i < STAGE_NUM; i++)
			{
				rectStage[i].left -= 10;
				rectStage[i].right -= 10;
			}
		}

	}
	if (KEY_PRESSED(VK_UP) && target->_rectDraw.top > rectWindow.top && !_select_pokemon)
	{
		target->_rectDraw.top -= MAPSCROLL_SPEED * 2;
		target->_rectDraw.bottom -= MAPSCROLL_SPEED * 2;
		_dialogflag = false;
	}
	if (KEY_PRESSED(VK_DOWN) && target->_rectDraw.bottom < rectWindow.bottom && !_select_pokemon)
	{
		target->_rectDraw.top += MAPSCROLL_SPEED * 2;
		target->_rectDraw.bottom += MAPSCROLL_SPEED * 2;
		_dialogflag = false;
	}

	if (KEY_TAP(VK_RETURN) && target->_select == true)
	{
		_enter_select = true;

		if (target->_select_index == StageElement::Lobby)
		{
			moveX = 300;
			SceneMgr->LoadScene(SceneType::Lobby);
			return;
		}

		switch (_phase)
		{
		case StageElement::Water:
		{
			if (target->_select_index == StageElement::Water)
			{
				_select_pokemon = true;
			}
			else
			{
				_dialogflag = true;
			}
		}
		break;
		case StageElement::Fire:
		{
			if (target->_select_index == StageElement::Water ||
				target->_select_index == StageElement::Fire)
			{
				_select_pokemon = true;
			}
			else
			{
				_dialogflag = true;
			}
		}
		break;
		case StageElement::Elec:
		{
			if (target->_select_index == StageElement::Water ||
				target->_select_index == StageElement::Fire ||
				target->_select_index == StageElement::Elec)
			{
				_select_pokemon = true;
			}
			else
			{
				_dialogflag = true;
			}
		}
		break;
		case StageElement::Dark:
		{
			if (target->_select_index == StageElement::Water ||
				target->_select_index == StageElement::Fire ||
				target->_select_index == StageElement::Elec ||
				target->_select_index == StageElement::Dark)
			{
				_select_pokemon = true;
			}
			else
			{
				_dialogflag = true;
			}
		}
		break;

		}
	}

	if (SceneMgr->IsLoading() == false && KEY_TAP(VK_BACK))
	{
		_select_pokemon = false;
		_ready_Air_pokemon = false;
		_ready_Land_pokemon = false;
		_enter_select = false;
		_finger = 0;
	}
	if (KEY_TAP(VK_RETURN))
	{
		_ready_Air_pokemon = false;
		_ready_Land_pokemon = false;
		_enter_select = true;
		_finger = 0;
	}
}

void SceneStage::ProcessCommand()
{
}

#include "stdafx.h"
#include "SceneBattle.h"
#include "InputManager.h"


Battle::Battle()
{
	_water.Load(L"images\\battle\\Water_spstage.bmp");
	_fire.Load(L"images\\battle\\Fire_spstage.bmp");
	_electronic.Load(L"images\\battle\\Electronic_spstage.bmp");
	_dark.Load(L"images\\battle\\Dark_spstage.bmp");
}

void Battle::Init()
{
	_rectImage = { 0, 0, 500, 1500 };
	_rectDraw = { 0, -750, 500, 750 };
	_rectDraw2 = { 0, -2250, 500, -750 };
	_shakeCount = 0;
	_shakeWindow = 0;
	_mapSpeed = 5;
}

void Battle::Render(HDC hdc, StageElement _select_index)
{
	switch (_select_index)
	{
	case StageElement::Water:
		_water.Draw(hdc, _rectDraw, _rectImage);
		_water.Draw(hdc, _rectDraw2, _rectImage);
		break;
	case StageElement::Fire:
		_fire.Draw(hdc, _rectDraw, _rectImage);
		_fire.Draw(hdc, _rectDraw2, _rectImage);
		break;
	case StageElement::Elec:
		_electronic.Draw(hdc, _rectDraw, _rectImage);
		_electronic.Draw(hdc, _rectDraw2, _rectImage);
		break;
	case StageElement::Dark:
		_dark.Draw(hdc, _rectDraw, _rectImage);
		_dark.Draw(hdc, _rectDraw2, _rectImage);
		break;
	}

	if (_rectDraw.bottom >= 3000)
	{
		_rectDraw.top = -1500;
		_rectDraw.bottom = 0;
	}
	if (_rectDraw2.bottom >= 3000)
	{
		_rectDraw2.top = -1500;
		_rectDraw2.bottom = 0;
	}

	if (isShake == true)
	{
		constexpr int shakeAmount = 5;
		if (_shakeWindow % 5 == 0 && _rectDraw.left == 0)
		{
			_rectDraw.left -= 5;
			_rectDraw.right -= 5;
			_rectDraw2.left -= 5;
			_rectDraw2.right -= 5;
		}
		if (_shakeWindow % 5 == 4 && _rectDraw.left == -5)
		{
			_rectDraw.left += 5;
			_rectDraw.right += 5;
			_rectDraw2.left += 5;
			_rectDraw2.right += 5;
		}

		++_shakeWindow;
		if (--_shakeCount <= 0)
		{
			_shakeWindow = 0;
			isShake = false;
		}
	}
	else
	{
		_rectDraw.left = 0;
		_rectDraw.right = 500;
		_rectDraw2.left = 0;
		_rectDraw2.right = 500;
	}
}

void SceneBattle::Init()
{
	battle.Init();
}

void SceneBattle::Render(HDC hdc)
{
	battle.Render(hdc, stage);
	/*boss->Render(hdc);
	player->Render(hdc);
	enemies->Render(hdc);
	player->RenderSkill(hdc);
	effects->Render(hdc);
	gui->Render(hdc);*/
}

void SceneBattle::Animate()
{
	/*player->Animate();
	enemies->Animate();
	boss->AnimateSkill();
	boss->Animate();*/
}

void SceneBattle::GetInput(CommandList* cmdList)
{
	if (KEY_TAP('O')) {
		//player->Heal();
	}
	if(KEY_TAP('P')) {
		//player->FullMP();
	}
	if(KEY_TAP('I')) {
		//player->InvincibleMode();
	}
	if(KEY_TAP('Q')) {
		//player->ActiveSkill(Skill::Identity);
	}
	if(KEY_TAP('W')) {
		//player->ActiveSkill(Skill::Sector);
	}
	if(KEY_TAP('E')) {
		//player->ActiveSkill(Skill::Circle);
	}

	bool isMove = false;
	if (KEY_TAP(VK_LEFT))
	{
		//player->Stop(Dir::Right);
		//player->SetDirection(Dir::Left);
		isMove = true;
	}
	if (KEY_TAP(VK_RIGHT))
	{
		/*player->Stop(Dir::Left);
		player->SetDirection(Dir::Right);*/
		isMove = true;
	}
	if (KEY_TAP(VK_UP))
	{
		/*player->Stop(Dir::Down);
		player->SetDirection(Dir::Up);*/
		isMove = true;
	}
	if (KEY_TAP(VK_DOWN))
	{
		/*player->Stop(Dir::Up);
		player->SetDirection(Dir::Down);*/
		isMove = true;
	}
}

void SceneBattle::WriteData(void* data)
{
}

void SceneBattle::ProcessCommand()
{
}

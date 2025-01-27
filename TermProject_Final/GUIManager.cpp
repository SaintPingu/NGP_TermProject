#include "stdafx.h"
#include "GUIManager.h"
#include "Framework.h"
#include "player.h"
//#include "enemy.h"
#include "image.h"
//#include "timer.h"
//#include "boss.h"
#include "scene.h"
#include "sound.h"
#include "SceneManager.h"
#include "SceneLobby.h"
#include "SceneStage.h"
#include "SceneBattle.h"

GUIManager::GUIManager()
{
	constexpr int fieldLength = 360; // 페이즈인가? 잘모르겠어서 기본설정

	constexpr int main_guiHeight = 80;

	constexpr int box_guiWidth = 92;
	constexpr int gague_guiWidth = 80;

	mainGUI = new GUIImage();
	gagueGUI_main = new GUIImage();
	gagueGUI_border = new GUIImage();
	gagueGUI_hp = new GUIImage();
	gagueGUI_mp = new GUIImage();

	icon_Q = new GUIImage();
	icon_W = new GUIImage();
	icon_E = new GUIImage();

	moveBarGUI = new GUIImage();
	icon_pokemon = new GUIImage();
	gaugeMoveBarGUI = new GUIImage();

	hurtGUI_Fire.gui = new GUIImage();
	hurtGUI_Water.gui = new GUIImage();
	hurtGUI_Elec.gui = new GUIImage();
	hurtGUI_Dark.gui = new GUIImage();

	switch (GetPlayerFlyType())
	{
	case Type::Elec:
		icon_Q->Load(_T("images\\battle\\icon_elec_Q.png"), { 35, 35 });
		icon_pokemon->Load(_T("images\\battle\\icon_thunder.png"), { 480, 480 });
		gaugeMoveBarGUI->Load(_T("images\\battle\\gague_bar_elec.png"), { 14, 217 });
		break;
	case Type::Water:
		icon_Q->Load(_T("images\\battle\\icon_water_Q.png"), { 260, 260 });
		icon_pokemon->Load(_T("images\\battle\\icon_articuno.png"), { 480, 480 });
		gaugeMoveBarGUI->Load(_T("images\\battle\\gague_bar_water.png"), { 14, 217 });
		break;
	case Type::Fire:
		icon_Q->Load(_T("images\\battle\\icon_fire_Q.png"), { 130, 130 });
		icon_pokemon->Load(_T("images\\battle\\icon_moltres.png"), { 480, 480 });
		gaugeMoveBarGUI->Load(_T("images\\battle\\gague_bar_fire.png"), { 14, 217 });
		break;
	}
	switch (GetPlayerGndType())
	{
	case Type::Elec:
		icon_W->Load(_T("images\\battle\\icon_elec_W.png"), { 130, 130 });
		icon_E->Load(_T("images\\battle\\icon_elec_E.png"), { 130, 130 });
		break;
	case Type::Water:
		icon_W->Load(_T("images\\battle\\icon_water_W.png"), { 130, 130 });
		icon_E->Load(_T("images\\battle\\icon_water_E.png"), { 130, 130 });
		break;
	case Type::Fire:
		icon_W->Load(_T("images\\battle\\icon_fire_W.png"), { 130, 130 });
		icon_E->Load(_T("images\\battle\\icon_fire_E.png"), { 130, 130 });
		break;
	}

	rectMain = rectWindow;
	rectMain.top = rectMain.bottom - main_guiHeight;

	constexpr POINT skillBoxSize = { 38,39 };
	constexpr POINT boxStartPoint = { 59, 21 };
	rectSkill_Q.left = rectMain.left + boxStartPoint.x;
	rectSkill_Q.top = rectMain.top + boxStartPoint.y;
	rectSkill_Q.right = rectSkill_Q.left + skillBoxSize.x;
	rectSkill_Q.bottom = rectSkill_Q.top + skillBoxSize.y;

	rectSkill_W = rectSkill_Q;
	rectSkill_W.left += box_guiWidth;
	rectSkill_W.right += box_guiWidth + 1;

	rectSkill_E = rectSkill_W;
	rectSkill_E.left += box_guiWidth;
	rectSkill_E.right += box_guiWidth;

	constexpr POINT gagueSize = { 60,60 };
	constexpr POINT gagueStartPoint = { 30, 10 };
	rectHP.left = rectSkill_E.right + gagueStartPoint.x;
	rectHP.right = rectHP.left + gagueSize.x;
	rectHP.top = rectMain.top + gagueStartPoint.y;
	rectHP.bottom = rectHP.top + gagueSize.y;

	rectMP = rectHP;
	rectMP.left += gague_guiWidth;
	rectMP.right += gague_guiWidth;

	constexpr POINT moveBarSize = { 20, 220 };
	constexpr POINT moveBarStartPoint = { WINDOWSIZE_X - 40, 220 };
	rectMoveBar.left = moveBarStartPoint.x;
	rectMoveBar.top = moveBarStartPoint.y;
	rectMoveBar.right = rectMoveBar.left + moveBarSize.x;
	rectMoveBar.bottom = rectMoveBar.top + moveBarSize.y;

	constexpr POINT gagueMoveBarSize = { 14,214 };
	constexpr POINT gagueMoveBarStartPoint = { 3, 4 };
	rectGaugeMoveBar.left = rectMoveBar.left + gagueMoveBarStartPoint.x;
	rectGaugeMoveBar.top = rectMoveBar.top + gagueMoveBarStartPoint.y;
	rectGaugeMoveBar.right = rectGaugeMoveBar.left + gagueMoveBarSize.x;
	rectGaugeMoveBar.bottom = rectGaugeMoveBar.top + gagueMoveBarSize.y;

	constexpr POINT pokemonIconSize = { 24, 24 };
	const int pokemonIconCenterX = rectMoveBar.left + ((rectMoveBar.right - rectMoveBar.left) / 2);
	rectPokemonIcon.left = pokemonIconCenterX - (pokemonIconSize.x / 2);
	rectPokemonIcon.right = rectPokemonIcon.left + pokemonIconSize.x;
	rectPokemonIcon.top = rectMoveBar.bottom - (pokemonIconSize.y / 2);
	rectPokemonIcon.bottom = rectPokemonIcon.top + pokemonIconSize.y;

	constexpr float moveBarHeight = moveBarSize.y - moveBarSize.x;
	constexpr int corrValue = 5;
	iconMoveMaxY = (rectMoveBar.top - (pokemonIconSize.y / 2)) + corrValue;
	iconMoveMinY = (rectMoveBar.bottom + (pokemonIconSize.y / 2));
	//iconMoveAmount = (moveBarHeight / fieldLength) / ELAPSE_BATTLE_GUI;

	mainGUI->Load(_T("images\\battle\\battleGUI.png"), { 500, 80 });
	gagueGUI_main->Load(_T("images\\battle\\gauge.png"), { 130, 130 });
	gagueGUI_border->Load(_T("images\\battle\\gauge_border.png"), { 130, 130 });
	gagueGUI_hp->Load(_T("images\\battle\\gauge_hp.png"), { 130, 130 });
	gagueGUI_mp->Load(_T("images\\battle\\gauge_mp.png"), { 130, 130 });
	moveBarGUI->Load(_T("images\\battle\\moveBar.png"), { 20, 223 });

	hurtGUI_Fire.gui->Load(_T("images\\battle\\frame_hurt_fire.png"), { 700, 1000 }, 0x00);
	hurtGUI_Water.gui->Load(_T("images\\battle\\frame_hurt_water.png"), { 239, 371 }, 0x00);
	hurtGUI_Elec.gui->Load(_T("images\\battle\\frame_hurt_elec.png"), { 239, 371 }, 0x00);
	hurtGUI_Dark.gui->Load(_T("images\\battle\\frame_hurt_dark.png"), { 239, 371 }, 0x00);
}

void GUIManager::Animate(HWND& hWnd)
{
	Update(hWnd);
}

void GUIManager::Render(HDC hdc)
{
	//std::unordered_map<int, std::shared_ptr<Player>> players;// 배틀의 플레이어 연결해줘야함 

	// MaxHP  MaxMP 가 몇인가? GetMaxHP 임시로 설정했음.

	mainGUI->Render(hdc, rectMain);

	gagueGUI_main->Render(hdc, rectHP);
	gagueGUI_hp->RenderGauge(hdc, rectHP, player->GetHP(), player->GetMaxHP());
	gagueGUI_border->Render(hdc, rectHP);

	gagueGUI_main->Render(hdc, rectMP);
	gagueGUI_mp->RenderGauge(hdc, rectMP, player->GetMP(), player->GetMaxMP());
	gagueGUI_border->Render(hdc, rectMP);

	icon_Q->Render(hdc, rectSkill_Q);
	icon_W->Render(hdc, rectSkill_W);
	icon_E->Render(hdc, rectSkill_E);

	if (isIconStop == false)
	{
		moveBarGUI->Render(hdc, rectMoveBar);
		gaugeMoveBarGUI->RenderGauge(hdc, rectGaugeMoveBar, iconMoveMinY - rectPokemonIcon.bottom, iconMoveMaxY);
		icon_pokemon->Render(hdc, rectPokemonIcon);
	}

	hurtGUI_Fire.gui->Render(hdc, rectWindow);
	hurtGUI_Water.gui->Render(hdc, rectWindow);
	hurtGUI_Elec.gui->Render(hdc, rectWindow);
	hurtGUI_Dark.gui->Render(hdc, rectWindow);
}

void GUIManager::Update(const HWND& hWnd)
{
	hurtGUI_Fire.ReduceAlpha();
	hurtGUI_Water.ReduceAlpha();
	hurtGUI_Elec.ReduceAlpha();
	hurtGUI_Dark.ReduceAlpha();
	
	if (player->IsDeath() == true)
	{
		return;
	}

	rectPokemonIcon.top -= iconMoveAmount;
	rectPokemonIcon.bottom -= iconMoveAmount;
	if (rectPokemonIcon.top <= iconMoveMaxY)
	{
		const int corrValue = rectPokemonIcon.top - iconMoveMaxY;
		rectPokemonIcon.top += corrValue;
		rectPokemonIcon.bottom += corrValue;
		isIconStop = true;

		//boss->Create();
	}
}
RECT GUIManager::GetRectDisplay() const
{
	RECT rectDisplay = rectWindow;
	rectDisplay.bottom = rectMain.top;
	return rectDisplay;
}

void GUIManager::DisplayHurtFrame()
{
	switch (GetStageElement())
	{
	case StageElement::Fire:
		hurtGUI_Fire.alpha = hurtGUI_alpha;
		break;
	case StageElement::Water:
		hurtGUI_Water.alpha = hurtGUI_alpha;
		break;
	case StageElement::Elec:
		hurtGUI_Elec.alpha = hurtGUI_alpha;
		break;
	case StageElement::Dark:
		CheckOverflowAdd(hurtGUI_Dark.alpha, 0x50);
		break;
	default:
		assert(0);
		break;
	}
}




void GUIManager::HurtGUI::ReduceAlpha()
{
	if (alpha == 0x00)
	{
		return;
	}
	gui->SetAlpha(alpha--);
}
#include "stdafx.h"
#include "Loading.h"
#include "Framework.h"


#define LOADING_POKEMON_X 150
#define LOADING_POKEMON_Y 121

#define HALF_RECTWINDOW_X 175 
#define HALF_RECTWINDOW_Y 280

#define LOADING_BAR_X 182
#define LOADING_BAR_Y 397

#define ALPHA 220

#define LOADING_POKEMON_X 150
#define LOADING_POKEMON_Y 121
#define LOADING_POKEMON_MAX_X 2100
#define LOADING_POKEMON_MAX_Y 121

void Loading::AnimateLoadingBar()
{
	loadingBarCnt += DeltaTime() * 2;
	if (loadingBarCnt > 4)
	{
		loadingBarCnt = 4;
	}
}

void Loading::AnimatePokemon()
{
	loadingPokemonCnt += DeltaTime() * 12.f;
	if ((int)loadingPokemonCnt > crntPokemonCnt) {
		crntPokemonCnt = (int)loadingPokemonCnt;
		lodingPokemonRectImage.left += LOADING_POKEMON_X;

		if (lodingPokemonRectImage.left == LOADING_POKEMON_MAX_X)
		{
			lodingPokemonRectImage.left = 0;
		}
	}
}

Loading::Loading()
{
	lodingPokemonRectImage = { 0, 0, LOADING_POKEMON_X, LOADING_POKEMON_Y };
	lodingPokemonRectDraw = { HALF_RECTWINDOW_X, HALF_RECTWINDOW_Y, LOADING_POKEMON_X, LOADING_POKEMON_Y };

	background.Load(L"images\\loading\\Loading_Black_background.bmp");
	loadingPokemon.Load(L"images\\loading\\Loading_pokemon.png");
	loadingBar[0].Load(L"images\\loading\\Loading_bar1.bmp");
	loadingBar[1].Load(L"images\\loading\\Loading_bar2.bmp");
	loadingBar[2].Load(L"images\\loading\\Loading_bar3.bmp");
	loadingBar[3].Load(L"images\\loading\\Loading_bar4.bmp");
	loadingBar[4].Load(L"images\\loading\\Loading_bar5.bmp");
}

void Loading::Render(HDC hdc)
{
	background.AlphaBlend(hdc, rectWindow, rectWindow, ALPHA);
	loadingPokemon.Draw(hdc, HALF_RECTWINDOW_X, HALF_RECTWINDOW_Y - 19, LOADING_POKEMON_X, LOADING_POKEMON_Y,
		lodingPokemonRectImage.left, lodingPokemonRectImage.top, lodingPokemonRectImage.right, lodingPokemonRectImage.bottom);

	loadingBar[(int)loadingBarCnt].TransparentBlt(hdc, LOADING_BAR_X, LOADING_BAR_Y, 125, 20, 0, 0, 1731, 286, RGB(0, 0, 0));

	HFONT hFont = CreateFont(31, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("ChubbyChoo-SemiBold"));
	HFONT oldFont = (HFONT)SelectObject(hdc, hFont);

	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(255, 255, 255));
	TextOut(hdc, LOADING_BAR_X, LOADING_BAR_Y + 12, L"LOADING...", 10);

	SelectObject(hdc, oldFont);
	DeleteObject(hFont);
}
void Loading::Animate()
{
	AnimateLoadingBar();
	AnimatePokemon();
}
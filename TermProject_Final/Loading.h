#pragma once
class Loading {
private:
	const int maxLoadCnt = 4;

	CImage background;
	CImage loadingPokemon;
	CImage loadingBar[5];
	float loadingBarCnt = maxLoadCnt;

	void AnimateLoadingBar();
	void AnimatePokemon();
public:
	RECT lodingPokemonRectImage = { 0, };
	RECT lodingPokemonRectDraw = { 0, };
	float loadingPokemonCnt = 0;
	int crntPokemonCnt = 0;

	Loading();
	void Render(HDC hdc);
	void Animate();
	inline constexpr bool IsLoaded()
	{
		return (loadingBarCnt >= maxLoadCnt) ? true : false;
	}
	inline constexpr void ResetLoading()
	{
		loadingPokemonCnt = 0;
		loadingBarCnt = 0;
	}
};
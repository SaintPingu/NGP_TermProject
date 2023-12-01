#include "stdafx.h"
#include "Effect.h"

EffectManager::EffectManager()
{
	explode_fire.Load(_T("images\\battle\\explode_fire.png"), { 56, 56 }, 8);
	explode_fire.ScaleImage(0.7f, 0.7f);
	explode_elec.Load(_T("images\\battle\\explode_elec.png"), { 81, 73 }, 8);
	explode_elec.ScaleImage(0.6f, 0.6f);
	explode_water.Load(_T("images\\battle\\explode_water.png"), { 72, 64 }, 9);
	explode_water.ScaleImage(0.7f, 0.7f);
	explode_dark.Load(_T("images\\battle\\explode_dark.png"), { 40, 40 }, 12);

	cloud_fire.Load(_T("images\\battle\\sprite_cloud_fire.png"), { 56, 64 }, 11);
	cloud_fire.ScaleImage(0.8f, 0.8f);
	cloud_elec.Load(_T("images\\battle\\sprite_cloud_elec.png"), { 56, 64 }, 11);
	cloud_elec.ScaleImage(0.8f, 0.8f);
	cloud_water.Load(_T("images\\battle\\sprite_cloud_water.png"), { 48, 56 }, 15);
	cloud_water.ScaleImage(0.9f, 0.9f);
	cloud_dark.Load(_T("images\\battle\\sprite_cloud_dark.png"), { 56, 64 }, 11);
}
EffectManager::Effect::Effect(EffectImage& effectImage, const POINT& pos)
{
	this->effectImage = &effectImage;
	this->pos = pos;
}

void EffectManager::Render(HDC hdc)
{
	for (Effect& effect : effects) {
		effect.Render(hdc);
	}
}

void EffectManager::Animate()
{
	for (size_t i = 0; i < effects.size(); ++i)
	{
		if (effects.at(i).Animate() == false)
		{
			effects[i--] = effects.back();
			effects.pop_back();
		}
	}
}


void EffectManager::Effect::Render(HDC hdc)
{
	const RECT rectImage = ISprite::GetRectImage(*effectImage, frame);
	effectImage->Render(hdc, pos, &rectImage);
}

bool EffectManager::Effect::Animate()
{
	if (++frame >= effectImage->GetMaxFrame())
	{
		return false;
	}

	return true;
}
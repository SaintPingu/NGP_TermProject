#pragma once
#include "Image.h"

class EffectManager {
private:
	class Effect : public ISprite {
	private:
		EffectImage* effectImage;
		POINT pos = { 0, };
	public:
		Effect(EffectImage& effectImage, const POINT& pos);
		void Render(HDC hdc);
		bool Animate();
	};

	EffectImage explode_fire;
	EffectImage explode_elec;
	EffectImage explode_water;
	EffectImage explode_dark;

	EffectImage cloud_fire;
	EffectImage cloud_elec;
	EffectImage cloud_water;
	EffectImage cloud_dark;

	std::vector<Effect> effects;
public:
	EffectManager();
	void Render(HDC hdc);
	void Animate();
};
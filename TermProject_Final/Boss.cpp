#include "stdafx.h"
#include "boss.h"
#include "player.h"
//#include "bullet.h"
//#include "timer.h"
#include "effect.h"
//#include "interface.h"
//#include "skill.h"
#include "scene.h"
#include "sound.h"
#include "Framework.h"
#include "SceneStage.h"

Boss::Boss()
{
	ObjectImage imgBullet;
	image = new ObjectImage();

	switch (GetStageElement())
	{
	case StageElement::Elec:
		image->Load(_T("images\\battle\\sprite_boss_elec.png"), { 73,68 }, { 3,7 }, { 69,50 });
		image->ScaleImage(4, 4);
		imgBullet.Load(_T("images\\battle\\bullet_boss_elec.png"), { 400,400 });
		imgBullet.ScaleImage(0.05f, 0.05f);
		break;
	case StageElement::Water:
		image->Load(_T("images\\battle\\sprite_boss_water.png"), { 65,41 }, { 2,3 }, { 63,36 });
		image->ScaleImage(4, 4);
		imgBullet.Load(_T("images\\battle\\bullet_boss_water.png"), { 400,400 });
		imgBullet.ScaleImage(0.05f, 0.05f);
		break;
	case StageElement::Fire:
		image->Load(_T("images\\battle\\sprite_boss_fire.png"), { 54,44 }, { 6,12 }, { 44,29 });
		image->ScaleImage(6, 6);
		imgBullet.Load(_T("images\\battle\\bullet_boss_fire.png"), { 400,400 });
		imgBullet.ScaleImage(0.05f, 0.05f);
		break;
	case StageElement::Dark:
		image->Load(_T("images\\battle\\sprite_boss_dark.png"), { 110,110 }, { 20,30 }, { 67,50 });
		image->ScaleImage(5, 5);
		imgBullet.Load(_T("images\\battle\\bullet_boss_dark.png"), { 700,700 });
		imgBullet.ScaleImage(0.03f, 0.03f);
		break;
	default:
		assert(0);
		break;
	}

	GameObject::Init(*image, {-9999, -9999});
}
Boss::~Boss()
{
	if (skill != nullptr)
	{
		//delete skill;
	}
	delete image;
	//delete bullets;
}


void Boss::Render(HDC hdc)
{
	image->Render(GetRectBody(), hdc);
	/*const RECT rectImage = ISprite::GetRectImage(GetImage(), frame);
	GameObject::Render(hdc, &rectImage);*/
}

void Boss::Animate(const HWND& hWnd)
{
	constexpr int loadingFrame = -40;

	//if (bossData.isDeath == true)
	//{
	//	if (--deathFrame > 0)
	//	{
	//		//effects->CreateBossDeathEffect(*this);
	//	}
	//	else if (deathFrame == -1)
	//	{
	//		//soundManager->PlayEffectSound(EffectSound::Win);
	//		//soundManager->StopBGMSound();

	//		//effects->CreateBossExplosionEffect(*this);
	//	}
	//	else if (deathFrame == loadingFrame)
	//	{
	//		//phase.ClearPhase();
	//		//sceneManager->StartLoading(hWnd);
	//	}
	//	return;
	//}

	if (isRevFrame == true)
	{
		--frame;
	}
	else
	{
		++frame;
	}

	//switch (GetAction())
	//{
	//case Action::Idle:
	//	if (frame > bossData.frameNum_IdleMax)
	//	{
	//		if (bossData.type == Type::Dark)
	//		{
	//			frame = bossData.frameNum_Idle;
	//		}
	//		else
	//		{
	//			isRevFrame = true;
	//			//--frame;
	//		}
	//	}
	//	else if (frame < bossData.frameNum_Idle)
	//	{
	//		isRevFrame = false;
	//		++frame;
	//	}
	//	break;
	//case Action::Attack:
	//	if (frame > bossData.frameNum_AtkMax)
	//	{
	//		isRevFrame = true;
	//		--frame;
	//	}
	//	else if (isRevFrame == true && frame < bossData.frameNum_AtkRev)
	//	{
	//		isRevFrame = false;
	//		if (bossData.type != Type::Dark)
	//		{
	//			SetAction(Action::Idle, bossData.frameNum_Idle);
	//		}
	//	}
	//	break;
	//default:
	//	assert(0);
	//	break;
	//}
}
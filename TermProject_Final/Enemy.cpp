#include "stdafx.h"
#include "enemy.h"
#include "player.h"
#include "effect.h"
#include "sound.h"

Enemy::Enemy(ObjectImage& image, const Vector2& pos) : GameObject(image, pos)
{
}

void Enemy::Render(const HDC& hdc, int spriteRow)
{
	const RECT rectImage = ISprite::GetRectImage(GetImage(), frame, spriteRow);
	GameObject::Render(hdc, &rectImage);
}
void Melee::Render(const HDC& hdc)
{
	const int spriteRow = GetSpriteRow();
	Enemy::Render(hdc, spriteRow);
}
void Range::Render(const HDC& hdc)
{
	constexpr int spriteRow = 0;
	Enemy::Render(hdc, spriteRow);
}

int Enemy::GetSpriteRow()
{
	int spriteRow = 0;
	switch (dir)
	{
	case Dir::Empty:
	case Dir::Up:
		spriteRow = 0;
		break;
	case Dir::Down:
		spriteRow = 1;
		break;
	case Dir::Left:
		spriteRow = 2;
		break;
	case Dir::Right:
		spriteRow = 3;
		break;
	case Dir::LU:
		spriteRow = 4;
		break;
	case Dir::RU:
		spriteRow = 5;
		break;
	case Dir::LD:
		spriteRow = 6;
		break;
	case Dir::RD:
		spriteRow = 7;
		break;
	default:
		assert(0);
		break;
	}

	return spriteRow;
}

void Enemy::Animate()
{
	if (isRevFrame == true)
	{
		--frame;
	}
	else
	{
		++frame;
	}

	switch (GetAction())
	{
	case Action::Idle:
		if (frame > data.frameNum_IdleMax)
		{
			isRevFrame = true;
			--frame;
		}
		else if (frame < data.frameNum_Idle)
		{
			isRevFrame = false;
			++frame;
		}
		break;
	case Action::Attack:
		if (frame > data.frameNum_AtkMax)
		{
			isRevFrame = true;
			--frame;
		}
		else if (isRevFrame == true && frame < data.frameNum_AtkRev)
		{
			isRevFrame = false;
			SetAction(Action::Idle, data.frameNum_Idle);
		}
		break;
	default:
		assert(0);
		break;
	}
}
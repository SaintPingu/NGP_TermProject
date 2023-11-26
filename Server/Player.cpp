#include "Player.h"
#include "Player.h"
#include "Player.h"
#include "stdafx.h"
#include "player.h"
#include "scene.h"

Player::Player(Type type, Type subType)
{
	constexpr int damagePerSec = 10;

	static ObjectImage img_pokemon;
	ObjectImage bulletImage;
	ObjectImage subBulletImage;

	playerData.type = type;
	playerData.subType = subType;
	switch (playerData.type)
	{
	case Type::Elec:
		pokemon = Pokemon::Thunder;
		img_pokemon.Load({ 17,24 });
		bulletImage.Load({ 5,16 });

		playerData.maxhp = 40;
		playerData.maxmp = 80;
		playerData.mp = 0;
		playerData.speed = 4;
		playerData.damage = 1.0f;
		playerData.damage_Q = 10.5f / damagePerSec;
		playerData.bulletSpeed = 8;
		playerData.shotDelay = 90;
		break;
	case Type::Water:
		pokemon = Pokemon::Articuno;
		img_pokemon.Load({ 13,23 });
		img_pokemon.ScaleImage(1.2f, 1.2f);
		bulletImage.Load({ 7,15 });
		bulletImage.ScaleImage(0.9f, 0.9f);

		playerData.maxhp = 65;
		playerData.maxmp = 120;
		playerData.mp = 40;
		playerData.speed = 2.5f;
		playerData.damage = 1.25f;
		playerData.damage_Q = 4.25f / damagePerSec;
		playerData.bulletSpeed = 6;
		playerData.shotDelay = 110;
		break;
	case Type::Fire:
		pokemon = Pokemon::Moltres;
		img_pokemon.Load({ 15,28 });
		bulletImage.Load({ 11,16 });
		bulletImage.ScaleImage(0.9f, 0.9f);

		playerData.maxhp = 50;
		playerData.maxmp = 100;
		playerData.mp = 20;
		playerData.speed = 3;
		playerData.damage = 1.35f;
		playerData.damage_Q = 12.5f / damagePerSec;
		playerData.bulletSpeed = 7;
		playerData.shotDelay = 100;
		break;
	default:
		assert(0);
		break;
	}
	playerData.hp = playerData.maxhp;

	switch (playerData.subType)
	{
	case Type::Elec:
		subPokemon = SubPokemon::Pikachu;
		subBulletImage.Load({ 11,30 });
		subBulletImage.ScaleImage(0.7f, 0.7f);
		playerData.subDamage = 1.0f;
		break;
	case Type::Water:
		subPokemon = SubPokemon::Squirtle;
		subBulletImage.Load({ 8,24 });
		subBulletImage.ScaleImage(0.8f, 0.7f);
		playerData.subDamage = 1.1f;
		break;
	case Type::Fire:
		subPokemon = SubPokemon::Charmander;
		subBulletImage.Load({ 8,16 });
		subBulletImage.ScaleImage(0.7f, 0.7f);
		playerData.subDamage = 1.2f;
		break;
	default:
		assert(0);
		break;
	}

	GameObject::Init(img_pokemon, { 250,500 });
	//bullets = new PlayerBullet(bulletImage);
	//subBullets = new PlayerBullet(subBulletImage);
}
Player::~Player()
{
	//delete bullets;
	//delete subBullets;
	//delete skillManager;
}
void Player::Init()
{
	//skillManager = new SkillManager();
}

void Player::Death()
{
	GameObject::SetPos({ WINDOWSIZE_X / 2, 10000 });
	playerData.isDeath = true;
	playerData.hp = 0;
}
void Player::SetPosDest()
{
	const int movementAmount = playerData.speed * 2;
	switch (direction)
	{
	case Dir::Left:
		vectorMove.x = -movementAmount;
		break;
	case Dir::Right:
		vectorMove.x = movementAmount;
		break;
	case Dir::Up:
		vectorMove.y = -movementAmount;
		break;
	case Dir::Down:
		vectorMove.y = movementAmount;
		break;
	case Dir::LD:
		vectorMove.x = -movementAmount;
		vectorMove.y = movementAmount;
		break;
	case Dir::LU:
		vectorMove.x = -movementAmount;
		vectorMove.y = -movementAmount;
		break;
	case Dir::RD:
		vectorMove.x = movementAmount;
		vectorMove.y = movementAmount;
		break;
	case Dir::RU:
		vectorMove.x = movementAmount;
		vectorMove.y = -movementAmount;
		break;
	default:
		break;
	}

	posDest = Vector2::GetDest(GetPosCenter(), vectorMove);
}

void Player::SetDirection(Dir inputDir)
{
	if (direction == inputDir || direction == Dir::Empty)
	{
		direction = inputDir;
		return;
	}

	switch (direction)
	{
	case Dir::Left:
		if (inputDir == Dir::Up)		direction = Dir::LU;
		else if (inputDir == Dir::Down)	direction = Dir::LD;
		break;
	case Dir::Right:
		if (inputDir == Dir::Up)		direction = Dir::RU;
		else if (inputDir == Dir::Down)	direction = Dir::RD;
		break;
	case Dir::Up:
		if (inputDir == Dir::Left)		direction = Dir::LU;
		else if (inputDir == Dir::Right)direction = Dir::RU;
		break;
	case Dir::Down:
		if (inputDir == Dir::Left)		direction = Dir::LD;
		else if (inputDir == Dir::Right)direction = Dir::RD;
		break;
	default:
		return;
	}
	StopMove();
}

void Player::SetMove(const HWND& hWnd, int timerID, int elpase, const TIMERPROC& timerProc)
{
	if (playerData.isDeath == true)
	{
		return;
	}
	else if (IsMove() == true && alpha > 0)
	{
		return;
	}

	SetPosDest();

	if (IsMove() == false && alpha == 0)
	{
		SetTimer(hWnd, timerID, elpase, timerProc);
	}
	else if (alpha > 0.5f)
	{
		alpha = 0.5f;
	}
	StartMove();
}

void Player::Move(const HWND& hWnd, int timerID)
{
	Vector2 posCenter = GetPosCenter();
	Vector2 posNext = Vector2::Lerp(posCenter, posDest, alpha);

	CheckCollideWindow(posNext);
	SetPos(posNext);
	posCenter = GetPosCenter();
	vectorMove = posDest - posCenter;

	alpha += 0.1f;
	if (alpha > 0.5f)
	{
		if (direction != Dir::Empty)
		{
			SetPosDest();

			alpha = 0.5f;
		}
		else if (alpha > 1)
		{
			vectorMove = { 0.0f, };
			StopMove();
			alpha = 0;
			KillTimer(hWnd, timerID);
		}
	}
}

void Player::Stop(Dir inputDir)
{
	switch (direction)
	{
	case Dir::Left:
		if (inputDir != Dir::Left) return;
		break;
	case Dir::Right:
		if (inputDir != Dir::Right) return;
		break;
	case Dir::Up:
		if (inputDir != Dir::Up) return;
		break;
	case Dir::Down:
		if (inputDir != Dir::Down) return;
		break;
	case Dir::LU:
		if (inputDir != Dir::Left && inputDir != Dir::Up) return;
		break;
	case Dir::LD:
		if (inputDir != Dir::Left && inputDir != Dir::Down) return;
		break;
	case Dir::RU:
		if (inputDir != Dir::Right && inputDir != Dir::Up) return;
		break;
	case Dir::RD:
		if (inputDir != Dir::Right && inputDir != Dir::Down) return;
		break;
	default:
		break;
	}
	direction = direction - inputDir;
}

void Player::CheckCollideWindow(Vector2& pos) const
{
	//const RECT rectDisplay = rectWindow;
	const RECT rectDisplay{};
	const RECT rectBody = GetRectBody(pos);

	POINT corrValue = { 0, };
	if (rectBody.left < 0)
	{
		corrValue.x = -rectBody.left;
	}
	else if (rectBody.right > rectDisplay.right)
	{
		corrValue.x = rectDisplay.right - rectBody.right;
	}
	if (rectBody.top < 0)
	{
		corrValue.y = -rectBody.top;
	}
	else if (rectBody.bottom > rectDisplay.bottom)
	{
		corrValue.y = rectDisplay.bottom - rectBody.bottom;
	}

	pos.x += corrValue.x;
	pos.y += corrValue.y;
}

void Player::Shot()
{
	//const RECT rectBody = GetRectBody();
	//BulletData bulletData;
	//bulletData.bulletType = playerData.type;
	//bulletData.damage = playerData.damage;
	//bulletData.speed = playerData.bulletSpeed;

	//POINT bulletPos = { 0, };
	//bulletPos.y = rectBody.top;
	//bulletPos.x = rectBody.left - 10;
	//bullets->CreateBullet(bulletPos, bulletData, Dir::Up);
	//bulletPos.x = rectBody.right + 10;
	//bullets->CreateBullet(bulletPos, bulletData, Dir::Up);

	//bulletData.bulletType = playerData.subType;
	//bulletData.damage = playerData.subDamage;
	//bulletPos.x = rectBody.left + ((rectBody.right - rectBody.left) / 2);
	//subBullets->CreateBullet(bulletPos, bulletData, Dir::Up);

	//skillManager->UseSkill();
}
void Player::CheckShot()
{
	if (playerData.isDeath == true)
	{
		return;
	}

	//playerData.crntShotDelay -= ELAPSE_BATTLE_INVALIDATE;
	if (IsClearShotDelay() == true)
	{
		Shot();
		ResetShotDelay();
	}
}
//void Player::CreateSubBullet(const POINT& center, const BulletData& data, Vector2 unitVector, bool isRotateImg, bool isSkillBullet)
//{
//	//subBullets->CreateBullet(center, data, unitVector, isRotateImg, isSkillBullet);
//}

void Player::Hit(float damage, Type hitType, POINT effectPoint)
{
	//if (playerData.isDeath == true)
	//{
	//	return;
	//}
	//else if (effectPoint.x == -1)
	//{
	//	effectPoint = GetPosCenter();
	//	GetRandEffectPoint(effectPoint);
	//}
	//effects->CreateHitEffect(effectPoint, hitType);
	//gui->DisplayHurtFrame(hitType);

	//if (playerData.isInvincible == true)
	//{
	//	return;
	//}

	//damage = CalculateDamage(damage, playerData.type, hitType);
	//if ((playerData.hp -= damage) <= 0)
	//{
	//	effects->CreateExplosionEffect(GetPosCenter(), playerData.type);
	//	Player::Death();
	//}
}

void Player::ActiveSkill(Skill skill)
{
	if (playerData.isDeath == true)
	{
		return;
	}

	//skillManager->ActiveSkill(skill);
}
void Player::MoveBullets()
{
	//bullets->Move();
	//subBullets->Move();
}
bool Player::IsUsingSkill() const
{
//	return skillManager->IsUsingSkill();
	return true;
}
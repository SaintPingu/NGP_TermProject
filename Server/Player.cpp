#include "stdafx.h"
#include "player.h"
#include "scene.h"
#include "Skill.h"
#include "Bullet.h"

Player::Player(Type type, Type subType)
{
	constexpr int damagePerSec = 10;

	ObjectImage img_pokemon;
	ObjectImage bulletImage;
	ObjectImage subBulletImage;

	PlayerDataInit(playerData, type, subType);

	switch (playerData.type)
	{
	case Type::Elec:
		pokemon = Pokemon::Thunder;
		img_pokemon.Load({ 17,24 });
		bulletImage.Load({ 5,16 });
		break;
	case Type::Water:
		pokemon = Pokemon::Articuno;
		img_pokemon.Load({ 13,23 });
		img_pokemon.ScaleImage(1.2f, 1.2f);
		bulletImage.Load({ 7,15 });
		bulletImage.ScaleImage(0.9f, 0.9f);
		break;
	case Type::Fire:
		pokemon = Pokemon::Moltres;
		img_pokemon.Load({ 15,28 });
		bulletImage.Load({ 11,16 });
		bulletImage.ScaleImage(0.9f, 0.9f);
		break;
	default:
		assert(0);
		break;
	}

	switch (playerData.subType)
	{
	case Type::Elec:
		subPokemon = SubPokemon::Pikachu;
		subBulletImage.Load({ 11,30 });
		subBulletImage.ScaleImage(0.7f, 0.7f);
		break;
	case Type::Water:
		subPokemon = SubPokemon::Squirtle;
		subBulletImage.Load({ 8,24 });
		subBulletImage.ScaleImage(0.8f, 0.7f);
		break;
	case Type::Fire:
		subPokemon = SubPokemon::Charmander;
		subBulletImage.Load({ 8,16 });
		subBulletImage.ScaleImage(0.7f, 0.7f);
		break;
	default:
		assert(0);
		break;
	}

	GameObject::Init(img_pokemon, { 250,500 });
	mainBullets = std::make_shared<PlayerBullet>(bulletImage);
	subBullets = std::make_shared<PlayerBullet>(subBulletImage);
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

void Player::SetDirection(Dir inputDir)
{
	direction = inputDir;
	if (direction != Dir::Empty) {
		StartMove();
	}
	else {
		StopMove();
	}
}

void Player::Move()
{
	if (!IsMove()) {
		return;
	}

	constexpr float speed = 200.f;
	Vector2 posCenter = GetPosCenter();
	Vector2 pos{};
	int v{}, h{};

	switch (direction) {
	case Dir::Left:
		v = -1;
		break;
	case Dir::Right:
		v = 1;
		break;
	case Dir::Up:
		h = -1;
		break;
	case Dir::Down:
		h = 1;
		break;
	}

	if (v != 0) {
		pos.x += DeltaTime() * speed * v;
	}
	else if (h != 0) {
		pos.y += DeltaTime() * speed * h;
	}
	Vector2 posDest = posCenter + pos;
	SetPos(posDest);
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
	const RECT rectBody = GetRectBody();
	BulletData bulletData;
	bulletData.bulletType = playerData.type;
	bulletData.damage = playerData.damage;
	bulletData.speed = playerData.bulletSpeed;

	POINT bulletPos = { 0, };
	bulletPos.y = rectBody.top;
	bulletPos.x = rectBody.left - 10;
	mainBullets->CreateBullet(bulletPos, bulletData, Dir::Up);
	bulletPos.x = rectBody.right + 10;
	mainBullets->CreateBullet(bulletPos, bulletData, Dir::Up);

	bulletData.bulletType = playerData.subType;
	bulletData.damage = playerData.subDamage;
	bulletPos.x = rectBody.left + ((rectBody.right - rectBody.left) / 2);
	subBullets->CreateBullet(bulletPos, bulletData, Dir::Up);

	//skillManager->UseSkill();
}
void Player::CheckShot()
{
	if (playerData.isDeath == true)
	{
		return;
	}

	playerData.crntShotDelay -= DeltaTime();
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

	skillManager->ActiveSkill(skill);
}
void Player::MoveBullets()
{
	mainBullets->Move();
	subBullets->Move();
}
bool Player::IsUsingSkill() const
{
	return skillManager->IsUsingSkill();
	//return true;
}
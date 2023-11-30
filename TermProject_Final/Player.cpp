#include "stdafx.h"
#include "Player.h"

Player::Player(Type type, Type subType)
{
	this->type = type;
	this->subType = subType;

	static ObjectImage img_pokemon;
	ObjectImage bulletImage;
	ObjectImage subBulletImage;

	switch (type)
	{
	case Type::Elec:
		pokemon = Pokemon::Thunder;
		img_pokemon.Load(_T("images\\battle\\sprite_thunder.png"), { 53, 48 }, { 19, 10 }, { 17,24 });
		bulletImage.Load(_T("images\\battle\\bullet_elec_main.png"), { 5,16 });
		break;
	case Type::Water:
		pokemon = Pokemon::Articuno;
		img_pokemon.Load(_T("images\\battle\\sprite_articuno.png"), { 69, 69 }, { 29, 28 }, { 13,23 });
		img_pokemon.ScaleImage(1.2f, 1.2f);
		bulletImage.Load(_T("images\\battle\\bullet_ice.png"), { 7,15 });
		bulletImage.ScaleImage(0.9f, 0.9f);
		break;
	case Type::Fire:
		pokemon = Pokemon::Moltres;
		img_pokemon.Load(_T("images\\battle\\sprite_moltres.png"), { 83, 75 }, { 35, 25 }, { 15,28 });
		bulletImage.Load(_T("images\\battle\\bullet_fire.png"), { 11,16 });
		bulletImage.ScaleImage(0.9f, 0.9f);
		break;
	default:
		assert(0);
		break;
	}

	switch (subType)
	{
	case Type::Elec:
		subPokemon = SubPokemon::Pikachu;
		img_subPokemon.Load(L"images\\battle\\sub_pikachu.png", { 23,25 });
		subBulletImage.Load(_T("images\\battle\\bullet_elec.png"), { 11,30 });
		subBulletImage.ScaleImage(0.7f, 0.7f);
		break;
	case Type::Water:
		subPokemon = SubPokemon::Squirtle;
		img_subPokemon.Load(L"images\\battle\\sub_squirtle.png", { 17,24 });
		subBulletImage.Load(_T("images\\battle\\bullet_water.png"), { 8,24 });
		subBulletImage.ScaleImage(0.8f, 0.7f);
		break;
	case Type::Fire:
		subPokemon = SubPokemon::Charmander;
		img_subPokemon.Load(L"images\\battle\\sub_charmander.png", { 18,23 });
		subBulletImage.Load(_T("images\\battle\\bullet_flame.png"), { 8,16 });
		subBulletImage.ScaleImage(0.7f, 0.7f);
		break;
	default:
		assert(0);
		break;
	}

	GameObject::Init(img_pokemon, { 250,500 });
	/*bullets = new PlayerBullet(bulletImage);
	subBullets = new PlayerBullet(subBulletImage);*/
}

Player::~Player()
{
	/*delete bullets;
	delete subBullets;
	delete skillManager;*/
}

void Player::Render(HDC hdc)
{
	/*bullets->Render(hdc);
	subBullets->Render(hdc);*/

	if (isDeath == true)
	{
		return;
	}

	const RECT rectImage = ISprite::GetRectImage(GetImage(), frame);
	GameObject::Render(hdc, &rectImage);

	const ObjectImage& image = GetImage();
	float scaleX, scaleY;
	image.GetScale(scaleX, scaleY);

	RECT rectDest = GetRectBody();
	switch (pokemon)
	{
	case Pokemon::Moltres:
		rectDest.top += 7 * scaleY;
		rectDest.bottom = rectDest.top + (22 * scaleY);
		break;
	case Pokemon::Articuno:
		rectDest.top += 3 * scaleY;
		rectDest.bottom = rectDest.top + (22 * scaleY);
		break;
	case Pokemon::Thunder:
		rectDest.top += 5 * scaleY;
		rectDest.bottom = rectDest.top + (20 * scaleY);
		break;
	default:
		assert(0);
		break;
	}

	img_subPokemon.Render(rectDest, hdc);
}

void Player::RenderSkill(HDC hdc)
{

}

void Player::Animate(const HWND& hWnd)
{

}


void Player::ActiveSkill(Skill skill)
{
	if (isDeath == true)
	{
		return;
	}

	//skillManager->ActiveSkill(skill);
}
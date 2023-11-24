#pragma once
#include "Scene.h"


class SceneStage : public Scene {
public:
	SceneStage();
	~SceneStage();
private:
	class Target
	{
	public:
		RECT _rectDraw = { 0, };
		RECT _rectImage = { 0, };
		CImage _img;
		CImage _select_img;
		RECT _cam = { 0, };
		bool _select = false;
		StageElement _select_index;
	};

	StageElement _phase = StageElement::Water;

	RECT _rectDraw = { 0, };
	RECT _rectImage = { 0, };
	RECT rectStage[5] = { 0, };
	CImage _water;
	CImage _fire;
	CImage _electronic;
	CImage _dark;
	CImage _dialog_bar;
	CImage _glowing_black;
	Target* target;
	bool _dialogflag = false;
	bool _select_pokemon = false;
	bool _ready_Air_pokemon = false;
	bool _ready_Land_pokemon = false;
	bool _enter_select = false;

	CImage _select_Pikachu;
	CImage _select_Charmander;
	CImage _select_Squirtle;
	CImage _select_Zapados;
	CImage _select_Moltres;
	CImage _select_Articuno;

	CImage _ready_Pikachu[2];
	CImage _ready_Charmander[2];
	CImage _ready_Squirtle[2];
	CImage _ready_Zapados[2];
	CImage _ready_Moltres[2];
	CImage _ready_Articuno[2];

	POINT _fingerPos = { 0, };

	int _finger = 0;
	int _finger_twinkle_cnt = 0;
	int moveX = 300;
	int _play_Air_pokemon = 0;
	int _play_Land_pokemon = 0;

	Type airPokemon = Type::Empty;
	Type landPokemon = Type::Empty;

	bool _allHide = false;
	int _select_pokemon_move = 0;
public:
	virtual void Init();
	virtual void Render(HDC hdc);
	virtual void Animate();
	virtual void GetInput(CommandList* cmdList);
	virtual void ProcessCommand();
	virtual void WriteData(void* data);


	void FingerController();

	void SetStageElement(StageElement stageElement) {
		_phase = stageElement;
	}
};
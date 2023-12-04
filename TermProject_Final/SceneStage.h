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
		CImage _img;					// 일반 타겟 이미지
		CImage _select_img;				// 빨간 타겟 이미지
		RECT _cam = { 0, };				// 카메라 RECT
		bool _select = false;			// 선택가능한가?
		StageElement _select_index;		// 현재 타겟이 위치한 스테이지
	};

	struct OtherType {
		Type fly{};
		Type gnd{};
	};

	RECT _rectDraw = { 0, };
	RECT _rectImage = { 0, };
	RECT rectStage[5] = { 0, };
	CImage _water;
	CImage _fire;
	CImage _electronic;
	CImage _dark;
	CImage _dialog_bar;
	CImage _glowing_black;
	std::unique_ptr<Target> target;
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
	float _finger_twinkle = 0;
	int moveX = 300;
	int _play_Air_pokemon = 0;
	int _play_Land_pokemon = 0;

	Type airPokemon = Type::Empty;
	Type landPokemon = Type::Empty;

	bool _allHide = false;
	int _select_pokemon_move = 0;
	int _select_pokemon_move_frameControl = 0;

	bool inWaitingRoom{};

	void ExitStage(CommandList* cmdList);
	void EnterLobby(CommandList* cmdList);
	void RequestEnterBattle(CommandList* cmdList);
public:
	virtual void Init();
	virtual void Render(HDC hdc);
	virtual void Animate();
	virtual void GetInput(CommandList* cmdList);
	virtual bool ProcessCommand();
	virtual void WriteData(void* data);

	void FingerController();
};

void SetStageElement(StageElement stageElement);
StageElement GetStageElement();
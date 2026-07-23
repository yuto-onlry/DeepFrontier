#pragma once
#include "DxLib.h"
#include "InputManager.h"
#include "CharacterManager.h"
#include "UIManager.h"
#include "PlayerCamera.h"
#include "WaveManager.h"
#include "StageManager.h"
#include "TitleManager.h"

enum class GameState
{
    Title,
	Playing,        //ゲームプレイ中
	GameClear,      //ゲームクリア
	GameOver        //ゲームオーバー
};

class GameManager
{
private:
    InputManager inputManager;

    CharacterManager characterManager;
    UIManager uiManager;
    PlayerCamera playerCamera;
    WaveManager waveManager;
    StageManager stageManager;
    GameState gameState;
    TitleManager titleManager;

    bool isPlaying;
    

public:
    GameManager();
    ~GameManager();
public:
    void Init();
    void Update();
    void Draw();
    void Release();

private:
    void Title();
    void Playing();
    void DrawGround();

    void StartGame();
    void ReleasePlaying();
    void BackTitle();
};
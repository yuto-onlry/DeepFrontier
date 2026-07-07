#pragma once
#include "DxLib.h"
#include "InputManager.h"
#include "CharacterManager.h"
#include "UIManager.h"
#include "PlayerCamera.h"
#include "WaveManager.h"

enum class GameState
{
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

    GameState gameState;

public:
    GameManager();
    ~GameManager();
public:
    void Init();
    void Update();
    void Draw();
    void Release();

private:
    void UpdatePlaying();
    void DrawGround();
};
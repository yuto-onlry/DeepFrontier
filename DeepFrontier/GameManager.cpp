#include "GameManager.h"

GameManager::GameManager()
    : gameState(GameState::Playing)
{
}

GameManager::~GameManager()
{
    Release();
}

void GameManager::Init()
{
    characterManager.Init();

    uiManager.Init();

    playerCamera.Init();

    waveManager.Init();
    waveManager.Start();

    gameState = GameState::Playing;
}

void GameManager::Update()
{
    inputManager.Update();

    if (gameState == GameState::Playing)
    {
        UpdatePlaying();
    }
}

void GameManager::UpdatePlaying()
{
    VECTOR cameraForward = playerCamera.GetForward();
    VECTOR cameraRight = playerCamera.GetRight();

    characterManager.Update(
        inputManager,
        cameraForward,
        cameraRight
    );

    // プレイヤー死亡判定
    if (characterManager.GetPlayer() != nullptr)
    {
        Player* player = characterManager.GetPlayer();

        if (player->GetHp() <= 0)
        {
            gameState = GameState::GameOver;
            return;
        }
    }

    // Wave更新
    waveManager.Update(characterManager);

    if (waveManager.IsGameClear() == true)
    {
        gameState = GameState::GameClear;
        return;
    }

    // カメラ更新
    if (characterManager.GetPlayer() != nullptr)
    {
        VECTOR rightStick = inputManager.GetRightStick();

        playerCamera.Update(
            characterManager.GetPlayer()->GetPosition(),
            rightStick
        );
    }
}

void GameManager::Draw()
{
    DrawGround();

    characterManager.Draw();

    waveManager.Draw(characterManager);

    if (characterManager.GetPlayer() != nullptr)
    {
        Player* player = characterManager.GetPlayer();

        uiManager.DrawPlayerHp(
            player->GetHp(),
            player->GetMaxHp()
        );
    }

    if (gameState == GameState::GameOver)
    {
        DrawString(
            300,
            250,
            "GAME OVER",
            GetColor(255, 0, 0)
        );
    }

    if (gameState == GameState::GameClear)
    {
        DrawString(
            300,
            250,
            "GAME CLEAR",
            GetColor(255, 255, 0)
        );
    }
}

void GameManager::Release()
{
    characterManager.Release();
    uiManager.Release();
}

void GameManager::DrawGround()
{
    DrawCube3D(
        VGet(-2000.0f, -40.0f, -2000.0f),
        VGet(2000.0f, -35.0f, 2000.0f),
        GetColor(30, 30, 40),
        GetColor(30, 30, 40),
        TRUE
    );

    for (int i = -2000; i <= 2000; i += 100)
    {
        DrawLine3D(
            VGet((float)i, -34.0f, -2000.0f),
            VGet((float)i, -34.0f, 2000.0f),
            GetColor(80, 80, 100)
        );

        DrawLine3D(
            VGet(-2000.0f, -34.0f, (float)i),
            VGet(2000.0f, -34.0f, (float)i),
            GetColor(80, 80, 100)
        );
    }

    DrawLine3D(
        VGet(-2000.0f, -33.0f, 0.0f),
        VGet(2000.0f, -33.0f, 0.0f),
        GetColor(120, 80, 80)
    );

    DrawLine3D(
        VGet(0.0f, -33.0f, -2000.0f),
        VGet(0.0f, -33.0f, 2000.0f),
        GetColor(80, 120, 80)
    );
}
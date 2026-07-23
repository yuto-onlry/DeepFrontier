#include "GameManager.h"

GameManager::GameManager()
    : gameState(GameState::Title),
    isPlaying(false)
{
}

GameManager::~GameManager()
{
    Release();
}

void GameManager::Init()
{
    titleManager.Init();
    characterManager.Init();

    uiManager.Init();

    playerCamera.Init();

    waveManager.Init();
    waveManager.Start();
    
    stageManager.Init();
    
 

    gameState = GameState::Title;
}

void GameManager::Update()
{
    inputManager.Update();

    if (gameState == GameState::Title)
    {
        Title();
        return;
    }

    if (gameState == GameState::Playing)
    {
        stageManager.Update();
        Playing();
        return;
    }

    if (gameState == GameState::GameOver)
    {
        if (inputManager.IsButtonDown(InputManager::PadButton::A))
        {
            BackTitle();
        }

        return;
    }

    if (gameState == GameState::GameClear)
    {
        if (inputManager.IsButtonDown(InputManager::PadButton::A))
        {
            BackTitle();
        }

        return;
    }
}

void GameManager::Playing()
{
    VECTOR cameraForward = playerCamera.GetForward();
    VECTOR cameraRight = playerCamera.GetRight();

    characterManager.Update(
        inputManager,
        cameraForward,
        cameraRight,
        stageManager
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

        bool useFocusCamera = false;
        VECTOR lockOnTargetPos = VGet(0.0f, 0.0f, 0.0f);

        if (characterManager.IsFocusPhase() == true &&
            characterManager.GetLockOnTarget() != nullptr)
        {
                useFocusCamera = true;
                lockOnTargetPos = characterManager.GetLockOnTargetPosition();
        }

        playerCamera.Update(
            characterManager.GetPlayer()->GetPosition(),
            rightStick,
            useFocusCamera,
            lockOnTargetPos,
            stageManager
        );
    }
}

void GameManager::Title()
{
    titleManager.Update(inputManager);

    if (titleManager.IsStartRequest() == true)
    {
        StartGame();
    }
}

void GameManager::StartGame()
{
    ReleasePlaying();

    characterManager.Init();

    uiManager.Init();

    playerCamera.Init();

    waveManager.Init();
    waveManager.Start();

    stageManager.Init();

    isPlaying= true;

    gameState = GameState::Playing;
}

void GameManager::ReleasePlaying()
{
    if (isPlaying == false)
    {
        return;
    }

    stageManager.Release();
    characterManager.Release();
    uiManager.Release();

    isPlaying= false;
}

void GameManager::BackTitle()
{
    ReleasePlaying();

    titleManager.Init();

    gameState = GameState::Title;
}
void GameManager::Draw()
{
    if (gameState == GameState::Title)
    {
        titleManager.Draw();
        return;
    }

    if (gameState == GameState::Playing ||
        gameState == GameState::GameOver ||
        gameState == GameState::GameClear)
    {
        stageManager.Draw();

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
    }

    if (gameState == GameState::GameOver)
    {
        DrawString(
            300,
            250,
            "GAME OVER",
            GetColor(255, 0, 0)
        );

        DrawString(
            300,
            300,
            "A BUTTON : TITLE",
            GetColor(255, 255, 255)
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

        DrawString(
            300,
            300,
            "A BUTTON : TITLE",
            GetColor(255, 255, 255)
        );
    }
}

void GameManager::Release()
{
    ReleasePlaying();

    titleManager.Release();
}

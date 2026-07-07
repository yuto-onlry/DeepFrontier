#include "WaveManager.h"
#include "CharacterManager.h"
#include "DxLib.h"

WaveManager::WaveManager()
    : currentWave(1),
    maxWave(3),
    startWaitTimer(0),
    startWaitFrame(60),
    state(WaveState::Ready)
{
}

WaveManager::~WaveManager()
{
}

void WaveManager::Init()
{
    currentWave = 1;
    maxWave = 3;

    startWaitFrame = 60;
    startWaitTimer = startWaitFrame;

    state = WaveState::Ready;
}
/// <summary>
/// ウェーブ開始
/// </summary>
void WaveManager::Start()
{
    currentWave = 1;
    startWaitTimer = startWaitFrame;
    state = WaveState::Ready;
}

void WaveManager::Update(CharacterManager& characterManager)
{
    if (state == WaveState::Clear)
    {
        return;
    }

    // Wave開始前の待ち時間
    if (state == WaveState::Ready)
    {
        if (startWaitTimer > 0)
        {
            startWaitTimer--;
            return;
        }

        characterManager.SpawnWave(currentWave);
        state = WaveState::Playing;
        return;
    }

	//ウェーブ中の更新
    if (state == WaveState::Playing)
    {
        if (characterManager.IsAllEnemyDead() == true)
        {
            currentWave++;

            if (currentWave > maxWave)
            {
                state = WaveState::Clear;
            }
            else
            {
                startWaitTimer = startWaitFrame;
                state = WaveState::Ready;
            }
        }
    }
}

void WaveManager::Draw(CharacterManager& characterManager)
{
    DrawFormatString(
        20,
        20,
        GetColor(255, 255, 255),
        "WAVE %d / %d",
        currentWave,
        maxWave
    );

    DrawFormatString(
        20,
        45,
        GetColor(255, 255, 255),
        "ENEMY %d",
        characterManager.GetEnemyCount()
    );

    if (state == WaveState::Ready)
    {
        DrawFormatString(
            300,
            250,
            GetColor(255, 255, 255),
            "WAVE %d START",
            currentWave
        );
    }
}

bool WaveManager::IsGameClear() const
{
    return state == WaveState::Clear;
}

int WaveManager::GetCurrentWave() const
{
    return currentWave;
}

WaveState WaveManager::GetState() const
{
    return state;
}
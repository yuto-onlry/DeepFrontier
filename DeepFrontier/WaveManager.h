#pragma once

class CharacterManager;

enum class WaveState
{
    Ready,      //ウェーブ開始待ち
    Playing,    //ウェーブ中
    Clear       //全ウェーブクリア
};

class WaveManager
{
private:
    int currentWave;
    int maxWave;

    int startWaitTimer;
    int startWaitFrame;

    WaveState state;

public:
    WaveManager();
    ~WaveManager();

public:
    void Init();
    void Start();

    void Update(CharacterManager& characterManager);
    void Draw(CharacterManager& characterManager);

public:
    bool IsGameClear() const;
    int GetCurrentWave() const;
    WaveState GetState() const;
};
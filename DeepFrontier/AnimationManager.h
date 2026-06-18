#pragma once
#include "DxLib.h"

enum class AnimationType
{
	Idle,       // 待機
	Walk,       // 歩行
    Run,        // 走行
    Attack,     // 攻撃
	StrongAttack, // 強攻撃
    Damage,     // ダメージ
    Dead,       // 死亡   
    Max
};
class AnimationManager
{
private:
    int modelHandle;        // 操作するモデル
    int animSrcHandle;      // アニメーションのソースハンドル
    int currentAnimIndex;   // 現在のアニメーション番号
    int attachAnimIndex;    // Attachされたアニメーション番号

    float animTime;         // 現在の再生時間
    float animTotalTime;    // アニメーション全体の長さ
    float animSpeed;        // 再生速度

    int animNoTable[(int)AnimationType::Max];

public:
    AnimationManager();
    ~AnimationManager();
    void Init(int model, int animSrc);
    void Release();

    void SetAnimNo(AnimationType type, int animNo);
    void ChangeAnim(AnimationType type);
    void Update();

    void SetSpeed(float speed);
};
#pragma once
#include "DxLib.h"

/// <summary>
/// アニメーション管理
/// </summary>
enum class AnimationType
{
	Idle,       // 待機
	Walk,       // 歩行
    Run,        // 走行
    Attack,     // 攻撃
	StrongAttack, // 強攻撃
	AttackLockOn,  // 攻撃ロックオン
	JumpStart,       // ジャンプ
	JumpLoop,        // ジャンプ中
	JumpEnd,         // 着地
	Avoid,      // 回避       
    Damage,     // ダメージ
	GetUp,      // 起き上がり
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

    void Init(int model);
    void Update();
    void Release();
    void ChangeAnim(AnimationType type);
    bool LoadAnimModel(const char* filePath);


	// アニメーション番号を設定するセッター
    void SetAnimIndex(AnimationType type, int animIndex);
    void SetSpeed(float speed);
};
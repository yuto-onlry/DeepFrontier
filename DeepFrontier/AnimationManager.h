#pragma once
#include "DxLib.h"

/// <summary>
/// アニメーション管理
/// </summary>
enum class AnimationType
{
	Idle,       // 待機
	Walk,       // 歩行
	WalkBack,   // 後退
    Run,        // 走行
    MoveLeft,  // 左移動
    MoveRight, // 右移動
    Attack,     // 攻撃
	StrongAttack, // 強攻撃
	StrongAttack2, // 強攻撃2
	StrongAttack3, // 強攻撃3
	LockOnIdle,         // ロックオン待機
	LockOnWalk,         // ロックオン歩行
	LockOnLateralMove,     // ロックオン左右移動
	LockOnAttack,       // ロックオン攻撃
	Jump,       // ジャンプ
	Avoid,      // 回避       
    Damage,     // ダメージ
	KnockbackDown, // ノックバックダウン
	Down,       // ダウン
	GetUp,      // 起き上がり
    Dead,       // 死亡   
    Max
};
class AnimationManager
{
private:
    int modelHandle;        // 操作するモデル
    int animSrcHandle;      // アニメーションのソースハンドル
    //animSrcHandleTableはアニメーションタイプごとに別MV1を読み込む用
    int animSrcHandleTable[(int)AnimationType::Max];
    AnimationType currentAnimType;
    int currentAnimIndex;   // 現在のアニメーション番号
    int attachAnimIndex;    // アタッチされたアニメーション番号

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
    // アニメーションタイプごとに別MV1を読み込む用
    bool LoadAnimModelForType(AnimationType type, const char* filePath, int animIndex);
    void ChangeAnim(AnimationType type);
    bool LoadAnimModel(const char* filePath);


    // アニメーション番号を設定するセッター
    void SetAnimIndex(AnimationType type, int animIndex);
    void SetSpeed(float speed);
};
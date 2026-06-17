#include "Player.h"
#include "DxLib.h"
#include <cmath>

Player::Player()
{
}

Player::~Player()
{
}

void Player::Init()
{
    hp = 100;
    attack = 10;
    defense = 2;

    position = VGet(0.0f, 0.0f, 0.0f);
    velocity = VGet(0.0f, 0.0f, 0.0f);

	// モデルの読み込み
	modelHandle = MV1LoadModel("../3dModel/UAL2_Standard.mv1");

    if (modelHandle == -1)
    {
        printfDx("Playerモデルの読み込みに失敗しました\n");
    }
    else
    {
        // モデルの大きさ調整
        MV1SetScale(modelHandle, VGet(200.0f, 200.0f, 200.0f));
		// モデルの90度回転
        MV1SetRotationXYZ(modelHandle, VGet(DX_PI_F / 2.0f, 0.0f, 0.0f));
        // モデルの初期位置
        MV1SetPosition(modelHandle, position);
    }

    isDead = false;
}
void Player::Update()
{
    DINPUT_JOYSTATE input;

    velocity = VGet(0.0f, 0.0f, 0.0f);
	float speed = 5.0f;

    if(GetJoypadDirectInputState(DX_INPUT_PAD1, &input) == 0)
    {
		//スティック入力
        float x = input.X / 1000.0f;
        float y = input.Y / 1000.0f;

        // デッドゾーン
        if (fabs(x) < 0.2f) x = 0.0f;
        if (fabs(y) < 0.2f) y = 0.0f;

        //左右
        velocity.x = x * speed;
        //前後      
        velocity.z = -y * speed;
    }
	position = VAdd(position, velocity);
}

void Player::Draw()
{
    if (modelHandle != -1)
    {
        CharacterBase::Draw();
    }
    else
    {
        //球体
        DrawSphere3D(position, 30.0f, 16, GetColor(0, 255, 0), GetColor(0, 255, 0), TRUE);
    }
}
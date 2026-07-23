#pragma once

#include "DxLib.h"
#include "InputManager.h"

class TitleManager
{
private:
    enum class MenuItem
    {
        Start,  //スタート
        End,    //エンド
        Max 
    };
private:
    int selectIndex;
    bool startRequest;
    bool endRequest;

    int moveCoolTime;


public:
    TitleManager();
    ~TitleManager();
public:
    void Init();
    void Update(const InputManager& inputManager);
    void Draw();
    void Release();

    bool IsStartRequest() const;
    bool IsEndRequest() const;
};
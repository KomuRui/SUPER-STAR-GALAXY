#pragma once
#include "PlayerState.h"

class RotationningState : public PlayerState
{
public:

	//更新
	void Update() override;

	//入力によって状態変化する
	void HandleInput() override;
};


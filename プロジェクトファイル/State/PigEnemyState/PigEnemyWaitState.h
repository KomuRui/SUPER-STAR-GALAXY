#pragma once
#include "../EnemyState.h"

/// <summary>
/// 待機状態のクラス
/// </summary>
class PigEnemyWaitState : public EnemyState
{
	
	int operationTime_;     //次の状態に変わるまでの時間
	int stateCount_;        //その状態になってからどのくらいの秒数たったか

public:

	//2D用更新
	void Update2D(Enemy* enemy) override;

	//3D用更新
	void Update3D(Enemy* enemy) override;

	//入力によって状態変化する
	void HandleInput(Enemy* enemy) override;

	//状態変化したとき一回だけ呼ばれる関数
	void Enter(Enemy* enemy) override;

};

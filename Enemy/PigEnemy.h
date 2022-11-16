#pragma once
#include "Enemy.h"

class PigEnemy : public Enemy
{
	//ノックバックの方向と距離
	XMVECTOR knockBackDir_;

	//ノックバックしたかどうか
	bool knockBackFlag_;

public:

	//コンストラクタ
	PigEnemy(GameObject* parent, std::string modelPath, std::string name);

	//更新の前に一回呼ばれる関数
	void EnemyChildStartUpdate() override;

	//更新
	void EnemyChildUpdate() override;

	//ノックバックして死亡
	void KnockBackDie() override;

	//死亡
	void Die() override;

	//継承先用の指定した時間で呼ばれるメソッド
	void TimeMethod() override;

	//何かのオブジェクトに当たった時に呼ばれる関数
	void OnCollision(GameObject* pTarget) override;
};
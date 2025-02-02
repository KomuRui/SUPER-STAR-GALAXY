#pragma once
#include "Enemy.h"
#include "../Engine/Component/Gauge.h"
#include "../UI/TalkImage.h"
#include "../UI/WorldScene/World2/BossWarningImage.h"


/// <summary>
/// 最後のボス
/// </summary>
class BossEnemy : public Enemy
{
	//ボスの体力
	int hp_;

	//タイマー番号
	int hTime_;

	//ボスの音番号
	int hBossAudio_;

	//話す画像
	TalkImage* pTalkImage_;

	//警告画像
	BossWarningImage* pWarningImage_;

	//子供格納用
	std::vector<GameObject*> child_;

	///////////////ノックバック///////////////////

	//ノックバックの方向と距離
	XMVECTOR knockBackDir_;

	//空飛ぶときの元となる上ベクトル
	XMVECTOR vFly_;

	//元となるフライベクトルを保存しておく
	XMVECTOR keepFly_;

	//ノックバックしたかどうか
	bool isKnockBack_;


public:

	//コンストラクタ
	BossEnemy(GameObject* parent, std::string modelPath, std::string name);
	
	//デストラクタ
	~BossEnemy();

	//////////////////////////////オーバーライドする関数///////////////////////////////////

	//更新の前に一回呼ばれる関数
	void EnemyChildStartUpdate() override;

	//移動
	void Move() override;

	//回転
	void Rotation() override;

	//更新
	void EnemyChildUpdate() override;

	//Playerが視角内,指定距離内にいるかどうか調べる
	void PlayerNearWithIsCheck() override {};

	//ノックバック
	void KnockBackDie() override;

	//死亡
	void Die() override;

	//継承先用の指定した時間で呼ばれるメソッド
	void TimeMethod() override;

	//何かのオブジェクトに当たった時に呼ばれる関数
	void OnCollision(GameObject* pTarget) override;

	////////////////////////////////関数///////////////////////////////////

	/// <summary>
	/// 生成
	/// </summary>
	void Generation();

	/// <summary>
	/// 話す画像を描画するように
	/// </summary>
	void SetTalkImageDraw();

	/// <summary>
	/// ボスキルシーンへチェンジ
	/// </summary>
	void ChangeBossKill();
};


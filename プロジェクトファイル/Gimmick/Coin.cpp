#include "Coin.h"
#include "../Manager/CoinManager/CoinManager.h"

//定数
namespace
{
	static const float ADD_ROTATION_ANGLE = 0.08f;      //回転するときの加算する角度
	static const float BLOCK_ADD_ROTATION_ANGLE = 0.3f; //回転するときの加算する角度
	static const float COLLIDER_POS_Y = 1.0f;           //コライダーのY軸のポジション
	static const float COLLIDER_RADIUS = 1.0f;          //コライダーの半径
	static const float UP_SPEED = 0.1f;                 //コインが上に行くときの速度
	static const float CALL_TIME_METHOD = 0.5f;         //タイムメソッドを呼ぶ時間
}

//コンストラクタ
Coin::Coin(GameObject* parent, std::string modelPath, std::string name)
	: Mob(parent, modelPath, name),type_(RotationType)
{
}

//更新の前に一度だけ呼ばれる関数
void Coin::ChildStartUpdate()
{
	//コライダー追加
	SphereCollider* collision = new SphereCollider(XMFLOAT3(ZERO, XMVectorGetY(XMVector3Normalize(vNormal_)) * COLLIDER_POS_Y * transform_.scale_.y, ZERO), COLLIDER_RADIUS * transform_.scale_.y);
	AddCollider(collision);
}

void Coin::ChildUpdate()
{
	//型によって処理を分ける
	switch (type_)
	{
	//回転
	case RotationType:

		Rotation();
		break;

	//ブロックからコイン出る
	case BlockCoinType:

		Rotation();
		BlockCoinBehavior();
		break;

	//その他は処理しない
	default:
		break;
	}


}

//ブロックからコイン出た時の挙動
void Coin::BlockCoinBehavior()
{
	//上方向に行く
	transform_.position_ = Float3Add(transform_.position_, VectorToFloat3(XMVector3Normalize(vNormal_) * UP_SPEED));

	//時間メソッドをまだ使用していなかったら(0.5f秒後に自身削除)
	if(!GetTimeMethod())SetTimeMethod(CALL_TIME_METHOD);
}

//回転
void Coin::Rotation()
{
	//普通の回転とそれ以外の時の回転の速度を変える
	if(type_ == RotationType)
		angle_ += ADD_ROTATION_ANGLE;
	else
		angle_ += BLOCK_ADD_ROTATION_ANGLE;

	//Angleが360までいったら0に戻す
	if (angle_ > TWOPI_DEGREES)
		angle_ = ZEROPI_DEGREES;
}

//指定した時間で呼ばれるメソッド
void Coin::TimeMethod()
{
	//所有コインの量を増やす(コインの大きさによって増やす量変える)
	CoinManager::AddCoin(transform_.scale_.y);

	//自身の削除
	KillMe();
}

//当たり判定
void Coin::OnCollision(GameObject* pTarget)
{
	//Player以外なら何もしない
	if (pTarget->GetObjectName() != "Player")
		return;

	//所有コインの量を増やす(コインの大きさによって増やす量変える)
	CoinManager::AddCoin(transform_.scale_.y);

	//自身の削除
	KillMe();
}

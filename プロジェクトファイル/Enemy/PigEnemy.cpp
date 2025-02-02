#include "PigEnemy.h"
#include "../Engine/ResourceManager/Model.h"
#include "../Engine/GameObject/Camera.h"
#include "../Manager/EffectManager/EnemyEffectManager/EnemyEffectManager.h"
#include "../Manager/GameManager/GameManager.h"
#include "../Manager/AudioManager/PlayerAudioManager/PlayerAudioManager.h"

//定数
namespace
{
	//////////////////////アニメーション//////////////////////

	static const int ANIM_START_FRAME = 1; //開始フレーム
	static const int ANIM_END_FRAME = 60;  //終了フレーム
	static const int ANIM_DIE_FRAME = 70;  //死亡フレーム
	static const float ANIM_SPEED = 2.0f;  //アニメスピード

	//////////////////////キャラの必要な情報//////////////////////

	static const int FEED_BACK_ANGLE = 25;					  //反応角度
	static const int RAY_DISTANCE = 1;                        //レイの距離
	static const int KNOCKBACK_ASSUMPTION_DISTANCE = 10;	  //ノックバック想定距離
	static const float KNOCKBACK_DIFFERENCIAL_DISTANCE = 1.0f;//ノックバックの差分距離
	static const float INTERPOLATION_COEFFICIENT = 0.08f;     //補間係数
	static const float HIT_STOP_TIME = 0.15f;                 //ヒットストップ演出の時間
	static const float FLY_VECTOR_SIZE = 0.5f;				  //FLYベクトルの大きさ
	static const float FLY_VECTOR_DOWN = 0.015f;			  //FLYベクトルを小さくしていくときの値
	static const float COLLIDER_SIZE = 1.7f;                  //コライダーサイズ
	static const float DIE_TIME = 2.0f;                       //死ぬまでの時間

	//////////////////////カメラ//////////////////////

	static const float VIBRATION_INTENSITY = 0.2f; //振動の強さ
}

//コンストラクタ
PigEnemy::PigEnemy(GameObject* parent, std::string modelPath, std::string name)
	:Enemy(parent, modelPath, name), isKnockBack_(false),knockBackDir_(XMVectorSet(ZERO,ZERO,ZERO,ZERO))
{
}

//コンストラクタ
PigEnemy::PigEnemy(GameObject* parent, std::string name)
	: Enemy(parent, "Enemy/Model/PigEnemy.fbx", name), isKnockBack_(false), knockBackDir_(XMVectorSet(ZERO, ZERO, ZERO, ZERO))
{
}

//更新の前に一回呼ばれる関数
void PigEnemy::EnemyChildStartUpdate()
{

	///////////////当たり判定設定///////////////////

	//玉
	collision = new SphereCollider(XMFLOAT3(ZERO, XMVectorGetY(XMVector3Normalize(vNormal_)) * 1, ZERO), COLLIDER_SIZE);
	AddCollider(collision);

	///////////////アニメーション///////////////////

	//開始
	Model::SetAnimFrame(hModel_, ANIM_START_FRAME, ANIM_END_FRAME, ANIM_SPEED);


	//継承先用の更新の前に一度だけ呼ばれる関数
	ChildPigEnemyStartUpdate();
}

//更新
void PigEnemy::EnemyChildUpdate()
{
	//コライダーのポジション変更
	SetPosCollider(XMFLOAT3(ZERO, XMVectorGetY(XMVector3Normalize(vNormal_)), ZERO));

	//継承先用の更新
	ChildPigEnemyUpdate();
}

//Playerが自身の上にいるかどうか
bool PigEnemy::IsPlayerTop()
{
	//自身の上ベクトルとPlayerまでのベクトルの内積を求める
	float topAngle = acos(XMVectorGetX(XMVector3Dot(XMVector3Normalize(XMLoadFloat3(new XMFLOAT3(GameManager::GetpPlayer()->GetPosition())) - XMLoadFloat3(&transform_.position_)), XMVector3Normalize(vNormal_))));

	//視角内,指定距離内にいるなら
	return (topAngle < XMConvertToRadians(FEED_BACK_ANGLE) && topAngle > XMConvertToRadians(-FEED_BACK_ANGLE)) ? true : false;
}

//ノックバックして死亡
void PigEnemy::KnockBackDie()
{
	//ノックバックしていないのなら
	if (!isKnockBack_)
	{
		//ノックバックどこまでするか設定(単位ベクトルにして定数分倍にする)
		knockBackDir_ = (-XMVector3Normalize(XMLoadFloat3(new XMFLOAT3(GameManager::GetpPlayer()->GetPosition())) - XMLoadFloat3(&transform_.position_)) * KNOCKBACK_ASSUMPTION_DISTANCE) + XMLoadFloat3(&transform_.position_);

		//どのくらい空飛ぶか設定
		ARGUMENT_INITIALIZE(vFly_, vNormal_ * FLY_VECTOR_SIZE);

		//基となるFlyベクトルを保存しておく
		ARGUMENT_INITIALIZE(keepFly_, vFly_);

		//ノックバックした
		ARGUMENT_INITIALIZE(isKnockBack_, !isKnockBack_);
	}

	//ノックバック(指定の場所まで補間してゆっくり行くように)
	XMStoreFloat3(&transform_.position_, XMVectorLerp(XMLoadFloat3(&transform_.position_), knockBackDir_, INTERPOLATION_COEFFICIENT));

	//距離
	float dist = RangeCalculation(transform_.position_, VectorToFloat3(knockBackDir_));

	//壁に埋まらないようにするためにノックバック方向にレイを飛ばす
	RayCastData data;
	data.start = transform_.position_;    
	XMStoreFloat3(&data.dir, -XMVector3Normalize(XMLoadFloat3(new XMFLOAT3(GameManager::GetpPlayer()->GetPosition())) - XMLoadFloat3(&transform_.position_)));
	Model::RayCast(hGroundModel_, &data);  

	//埋まった分戻す
	if (data.dist <= RAY_DISTANCE)
	{
		//死亡状態に変更
		ChangeEnemyState(EnemyStateList::GetEnemyDieState());
	}

	//ノックバックしているなら
	if (isKnockBack_)
	{
		//基となるジャンプベクトルと符号が同じなら
		if (signbit(XMVectorGetY(vFly_)) == signbit(XMVectorGetY(keepFly_)))
		{
			//ベクトルの長さ調べる
			float len = sqrtf(XMVectorGetX(vFly_) * XMVectorGetX(vFly_) + XMVectorGetY(vFly_) * XMVectorGetY(vFly_) + XMVectorGetZ(vFly_) * XMVectorGetZ(vFly_));

			//フライベクトルをキャラの上軸に直す
			ARGUMENT_INITIALIZE(vFly_, vNormal_ * len);

			//空飛ばせる
			ARGUMENT_INITIALIZE(transform_.position_, Float3Add(transform_.position_, VectorToFloat3(vFly_ - (vNormal_ * FLY_VECTOR_DOWN))));

			//どんどんジャンプベクトルを小さくしていく
			ARGUMENT_INITIALIZE(vFly_, vFly_ - (vNormal_ * FLY_VECTOR_DOWN));
		}

		if (XMVectorGetX(up_) != XMVectorGetX(vNormal_) || XMVectorGetY(up_) != XMVectorGetY(vNormal_) || XMVectorGetZ(up_) != XMVectorGetZ(vNormal_))
		{
			//外積求める
			XMVECTOR cross = XMVector3Cross(up_, vNormal_);

			//転ばせる
			transform_.mmRotate_ *= XMMatrixRotationAxis(cross, 2);
		}
	}

	//ノックバックした距離がノックバックの想定距離と1以内の距離なら
	if (dist < KNOCKBACK_DIFFERENCIAL_DISTANCE)
	{
		//ノックバックしてない状態に
		ARGUMENT_INITIALIZE(isKnockBack_, !isKnockBack_);

		//死亡状態に変更
		ChangeEnemyState(EnemyStateList::GetEnemyDieState());
	}
}

//死亡
void PigEnemy::Die()
{
	//待機状態に変更
	ChangeEnemyState(EnemyStateList::GetEnemyWaitState());

	//死ぬエフェクト
	EnemyEffectManager::DieEffect(transform_.position_, up_);

	//削除
	KillMe();
}

//何かのオブジェクトに当たった時に呼ばれる関数
void PigEnemy::TimeMethod()
{
	Enter();
}

//何かのオブジェクトに当たった時に呼ばれる関数
void PigEnemy::OnCollision(GameObject* pTarget)
{
	//もしPlayerと当たったら
	if (pTarget->GetObjectName() == "Player")
	{
		
		//当たった時Playerが上にいるかつ自身が死んでいないなら
		if (IsPlayerTop() && pState_ != EnemyStateList::GetEnemyKnockBackState() && pState_ != EnemyStateList::GetEnemyDieState())
		{
			//死んでるアニメーションにする
			Model::SetAnimFrame(hModel_, ANIM_DIE_FRAME, ANIM_DIE_FRAME, ZERO);

			//死亡させる
			ChangeEnemyState(EnemyStateList::GetEnemyDieState());
		}

		//もしPlayerが回転していたらかつ自身が死んでいないなら
		if (GameManager::GetpPlayer()->IsRotation() && pState_ != EnemyStateList::GetEnemyKnockBackState() && pState_ != EnemyStateList::GetEnemyDieState())
		{
			//ヒットストップ演出(動きを止める)
			Leave();
			pTarget->Leave();

			//Playerも敵も0.15秒後に動き出す
			SetTimeMethod(HIT_STOP_TIME);
			pTarget->SetTimeMethod(HIT_STOP_TIME);

			//当たった位置を調べる
			XMFLOAT3 hitPos = VectorToFloat3(XMLoadFloat3(&transform_.position_) + (XMVector3Normalize(XMLoadFloat3(new XMFLOAT3(GameManager::GetpPlayer()->GetPosition())) - XMLoadFloat3(&transform_.position_)) * GetColliderRadius()));

			//エフェクト表示
			EnemyEffectManager::HitEffect(hitPos, transform_.position_);

			//音
			PlayerAudioManager::AttackHitAudio();

			//カメラ振動
			Camera::SetCameraVibration(VIBRATION_INTENSITY);

			//ノックバックして死亡させる
			ChangeEnemyState(EnemyStateList::GetEnemyKnockBackState());

			//終了
			return;

		}
	}
}

#include "UserPlanetBase.h"
#include "../Engine/Model.h"
#include "../Engine/Input.h"

//コンストラクタ
UserPlanetBase::UserPlanetBase(GameObject* parent, std::string modelPath, std::string name)
	:GameObject(parent, name), hModel_(-1), ModelNamePath_(modelPath), status_(Stop)
{
}

/////////////////////オーバーライドする関数//////////////////////

//初期化
void UserPlanetBase::Initialize()
{
	//モデルデータロード
	hModel_ = Model::Load(ModelNamePath_);
	assert(hModel_ >= ZERO);
}

//更新の前に一回呼ばれる関数
void UserPlanetBase::StartUpdate()
{
}

//更新
void UserPlanetBase::Update()
{
	//少し回転させる
	transform_.rotate_.y += RATATION_SPEED;

	//Move状態なら
	if (status_ == Move)
		NextPositionToMove();
}

//描画
void UserPlanetBase::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

//解放
void UserPlanetBase::Release()
{
}

/////////////////////関数//////////////////////

//次の位置へ移動
void UserPlanetBase::NextPositionToMove()
{
	//補間しながら目的のポジションまで変更していく
	transform_.position_ = VectorToFloat3(XMVectorLerp(XMLoadFloat3(&transform_.position_), XMLoadFloat3(&nextPos_), 0.05));
	
	//距離が0.05より小さいならStop状態に設定
	if (RangeCalculation(transform_.position_, nextPos_) < 0.05)
	{
		//自身のポジションを次の位置に設定
		ARGUMENT_INITIALIZE(transform_.position_, nextPos_);
		ARGUMENT_INITIALIZE(status_, Stop);
	}

}
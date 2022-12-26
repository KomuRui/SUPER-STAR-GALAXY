#include "EnemyRotationState.h"
#include "../../Engine/Input.h"
#include "../../Engine/GameManager.h"
#include "../../Enemy/Enemy.h"

//定数
namespace
{
    static const float ADD_ROTATION_ANGLE = 0.02f; //回転するときの加算する角度
}

//更新
void EnemyRotationState::Update2D(Enemy* enemy)
{
	//入力処理
	HandleInput(enemy);
}

//3D用更新
void EnemyRotationState::Update3D(Enemy* enemy)
{
	//回転
	enemy->Rotation();

    //上軸少し回転させてセット
    enemy->SetAngle(enemy->GetAngle() + (ADD_ROTATION_ANGLE * rotationSign_));

    //回転角度より回転総数が多くなったら
    if (rotationTotal_ > rotationAngle_)
    {
        //0に初期化
        ZERO_INITIALIZE(rotationTotal_);
        ZERO_INITIALIZE(rotationAngle_);

        //状態を待機に設定
        enemy->ChangeEnemyState(EnemyStateList::GetEnemyWaitState());
    }

	//入力処理
	HandleInput(enemy);
}

//入力によって状態変化する
void EnemyRotationState::HandleInput(Enemy* enemy)
{
}

//状態変化したとき一回だけ呼ばれる関数
void EnemyRotationState::Enter(Enemy* enemy)
{
    rotationSign_ = rand() % 2 == 1 ? 1 : -1;
    rotationAngle_ = XMConvertToRadians((rand() % 141) + 40);
}
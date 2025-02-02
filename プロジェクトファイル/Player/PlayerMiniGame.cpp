#include "PlayerMiniGame.h"
#include "../Engine/GameObject/Camera.h"
#include "../Engine/GameObject/Light.h"
#include "../Block/Block.h"
#include "../Engine/ResourceManager/Image.h"
#include "../Manager/EffectManager/PlayerEffectManager/PlayerEffectManager.h"
#include "../Manager/MiniGameManager/MiniGameManager.h"

//定数
namespace
{
    //Playerのパラメータを保存してあるパス
    LPCTSTR parameterPath = "Parameter/Player/PlayerParameter.ini";

    ///////////////キャラの必要な情報///////////////////

    static const float GRAVITY_ADDITION = 0.03f;                                                                  //重力の加算値
    static const float PLAYER_ANIM_SPEED = GetPrivateProfilefloat("PLAYER", "AnimSpeed", "1.0", parameterPath);   //アニメーションの再生速度

    ////////////////カメラ///////////////////

    static const float CAMERA_INTERPOLATION_FACTOR = GetPrivateProfilefloat("CAMERA", "CamFactor", "0.08", parameterPath);  //カメラの移動を補間するときの補間係数
    static const float CAM_SHOULDER_ADD_VALUE = 0.5f; //ショルダーでカメラ操作する時の加算する値
    static const float CAM_STICR_ADD_VALUE = 0.03f;   //スティックRでカメラを操作するときの加算する値
}

//コンストラクタ
PlayerMiniGame::PlayerMiniGame(GameObject* parent)
    :PlayerBase(parent), hPictWind_((int)ZERO), hPictWind2_((int)ZERO)
{
}

//初期化
void PlayerMiniGame::ChildPlayerInitialize()
{
    //補間係数を変更
    ARGUMENT_INITIALIZE(normalInterpolation_, 0.3f);

    //////風の画像をロード

    //1
    hPictWind_ = Image::Load("Image/MiniGame/kaze1.png");
    assert(hPictWind_ >= ZERO);

    //2
    hPictWind2_ = Image::Load("Image/MiniGame/kaze2.png");
    assert(hPictWind2_ >= ZERO);


    //ミニゲームの時のカメラの位置は少し違うので設定
    ARGUMENT_INITIALIZE(camVecTotal_[LONG],XMVectorSet(ZERO, 5, -55, ZERO));
    ARGUMENT_INITIALIZE(camVecTotal_[SHORT],XMVectorSet(ZERO, 5, -15, ZERO));
    ARGUMENT_INITIALIZE(vCam_, camVecTotal_[SHORT]);
}

//更新の前に一度だけ呼ばれる関数
void PlayerMiniGame::ChildPlayerStartUpdate()
{
}

//更新
void PlayerMiniGame::ChildPlayerUpdate()
{
    MovingOperation();       //Player操作
    RotationInStage();       //ステージに合わせて回転
    StageRayCast();          //ステージとの当たり判定
}

//描画
void PlayerMiniGame::ChildDraw()
{
    //もし走るモードなら
    if (MiniGameManager::IsRunMode() && IsCamLong())
    {
        Transform t;

        //乱数で表示する画像を変える
        if (rand() % 2 == 1)
        {
            Image::SetTransform(hPictWind_, t);
            Image::Draw(hPictWind_);
        }
        else
        {
            Image::SetTransform(hPictWind2_, t);
            Image::Draw(hPictWind2_);
        }
    }
}

//プレイヤー操作
void PlayerMiniGame::MovingOperation()
{
    //今の状態の動き
    pState_->UpdateMiniGame(this);
}

//ステージに合わせて回転
void PlayerMiniGame::RotationInStage()
{
    //Xのベクトルを抜き取る
    float dotX = ZERO;

    //外積の結果入れる用
    XMVECTOR cross;

    //自キャラまでのベクトルと自キャラの真上のベクトルが少しでも違うなら
    if (TwoVectorNotValue(up_, vNormal_))
    {
        //自キャラまでのベクトルと自キャラの真上のベクトルの内積を求める
        dotX = XMVectorGetX(XMVector3Dot(XMVector3Normalize(up_), XMVector3Normalize(vNormal_)));

        //外積を求める(この結果の軸を横軸にする)
        cross = XMVector3Cross(up_, vNormal_);
    }
    else
        cross = XMVectorSet(ZERO, ZERO, ZERO, ZERO);

    //エラーの範囲内ではなければ
    if (dotX != ZERO && dotX <= 1 && dotX >= -1 && !VectorNotZero(cross))
    {

        //Playerを回転させるために二つの軸で回転させる
        totalMx_ *= XMMatrixRotationAxis(cross, acos(dotX));
        transform_.mmRotate_ = totalMx_;
        transform_.mmRotate_ *= XMMatrixRotationAxis(vNormal_, angle_);

        //Playerが回転しているなら
        if (IsRotation()) mPreviousAngle_ = (totalMx_ * XMMatrixRotationAxis(cross, acos(dotX))) * XMMatrixRotationAxis(vNormal_, jampRotationPreviousAngle_);

        //カメラの行列用意
        ARGUMENT_INITIALIZE(camMat_, totalMx_);
    }
    else
    {
        //Playerを回転させるために軸で回転させる
        transform_.mmRotate_ = totalMx_;
        transform_.mmRotate_ *= XMMatrixRotationAxis(vNormal_, angle_);

        //Playerが回転しているなら
        if (IsRotation()) mPreviousAngle_ = (totalMx_ * XMMatrixRotationAxis(vNormal_, jampRotationPreviousAngle_));
    }

    //自身の上ベクトル更新
    ARGUMENT_INITIALIZE(up_, vNormal_);
}

//Playerのカメラの処理
void PlayerMiniGame::PlayerCameraBehavior(XMFLOAT3* pos, XMFLOAT3* tar)
{
    //走るモードなら
    if (MiniGameManager::IsRunMode()) RunModeCameraBehavior();


    XMFLOAT3 camPos;                                             //最終的なカメラの位置を入れる変数
    XMVECTOR vPos = XMLoadFloat3(&transform_.position_);         //transform_.position_のVector型
    XMVECTOR vCamDis_ = XMVector3TransformCoord(vCam_, camMat_); //vCamを回す

    vPos += vCamDis_;                //PlayerのPosにPlayerからカメラのベクトルをたす
    XMStoreFloat3(&camPos, vPos);    //camPosにvPosをXMFLOAT3に変えていれる

    *tar = Float3Add(transform_.position_, VectorToFloat3(vNormal_ * 3));

    //カメラのいろいろ設定
    Camera::SetUpDirection(vNormal_);
    Camera::SetPosition(camPos);
    Camera::SetTarget(*tar);

    //ライトの位置設定
    XMFLOAT3 lightPos;
    XMStoreFloat3(&lightPos, vNormal_ + XMLoadFloat3(&transform_.position_));
    Light::SetPlayerPosition(XMFLOAT4(lightPos.x, lightPos.y, lightPos.z, ZERO));
}

//レイで当たり判定
void PlayerMiniGame::StageRayCast()
{
    //各方向
    RayCastData straightData;
    RayCastData downData;

    //当たってるか確認
    HitTest(&straightData, STRAIGHT_VECTOR);
    HitTest(&downData, DOWN_VECTOR);

    ////////////////////////////////はみ出した分下げる//////////////////////////////////////

    //先にベクトル型のポジションを作っておく
    XMVECTOR pos = XMLoadFloat3(&transform_.position_);

    //前
    if (straightData.dist <= 1.0)
    {
        //障害物情報がnullptrじゃないのなら死亡
        if (straightData.obstacle != nullptr)
        {
            //当たった敵削除
            straightData.obstacle->KillMe();

            //死亡状態じゃないのなら
            if (PlayerStateManager::playerState_ != PlayerStateManager::playerDieing_)
            {
                pState_->ChangeState(PlayerStateManager::playerDieing_, this);
            }
        }

        XMVECTOR dis = { ZERO,ZERO,straightData.dist };
        dis = XMVector3TransformCoord(dis, transform_.mmRotate_);
        XMStoreFloat3(&transform_.position_, pos - (XMVector3TransformCoord(STRAIGHT_VECTOR, transform_.mmRotate_) - dis));
    }

    //下
    if (downData.dist >= 1.0)
    {
        transform_.position_ = Float3Add(transform_.position_, VectorToFloat3((down_ / 10) * acceleration_));
        acceleration_ += GRAVITY_ADDITION;
    }
    else
    {
        //回転じゃないかつ死亡していないのなら
        if (PlayerStateManager::playerState_ != PlayerStateManager::playerRotationning_ && PlayerStateManager::playerState_ != PlayerStateManager::playerDieing_)
        {
            //状態変更
            pState_->ChangeState(PlayerStateManager::playerStanding_, this);
        }

        ARGUMENT_INITIALIZE(acceleration_, 1);
    }
}

//当たり判定
void PlayerMiniGame::HitTest(RayCastData* data, const XMVECTOR& dir)
{
    data->start = transform_.position_;                                               //レイの発射位置  
    XMStoreFloat3(&data->dir, XMVector3TransformCoord(dir, transform_.mmRotate_));    //レイの方向
    Model::AllRayCast(hGroundModel_, data);                                           //レイを発射
}

//ランモードの時のカメラの処理
void PlayerMiniGame::RunModeCameraBehavior()
{
    //カメラが遠距離なら
    if (camStatus_ == LONG)
    {
        //カメラを補間で動かす
        ARGUMENT_INITIALIZE(vCam_, XMVectorLerp(vCam_, camVecTotal_[camStatus_], 0.1f));

        //距離が1以内なら
        if (RangeCalculation(vCam_, camVecTotal_[camStatus_]) < 1.0f)
        {
            ARGUMENT_INITIALIZE(camStatus_, SHORT);
            MiniGameManager::ResetRunSpeed();
            MiniGameManager::SetComboTextScale(1.0f);
        }
    }
    //通常にする
    else
    {
        //カメラを補間で動かす
        ARGUMENT_INITIALIZE(vCam_, XMVectorLerp(vCam_, camVecTotal_[camStatus_], 0.1f));

        //距離が1以内なら
        if (RangeCalculation(vCam_, camVecTotal_[camStatus_]) < 1.0f)
        {
            ARGUMENT_INITIALIZE(vCam_, camVecTotal_[camStatus_]);
            MiniGameManager::SetRunMode(false);
            MiniGameManager::ResetCombo();
        }
    }
}

//何かに当たった
void PlayerMiniGame::OnCollision(GameObject* pTarget)
{
    //豚の敵と当たった時回転していないのなら
    if (pTarget->GetObjectName() == "MiniGamePigEnemy" && !IsRotation())
    {
        //当たった敵削除
        pTarget->KillMe();

        //死亡状態じゃないのなら
        if (PlayerStateManager::playerState_ != PlayerStateManager::playerDieing_)
        {
            pState_->ChangeState(PlayerStateManager::playerDieing_,this);
        }
    }
}
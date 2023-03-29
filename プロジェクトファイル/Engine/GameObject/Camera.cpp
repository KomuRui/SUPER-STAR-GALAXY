#include "camera.h"
#include "../DirectX/Direct3D.h"

//////////カメラに必要な変数
XMFLOAT3 _position;
XMFLOAT3 _target;
XMMATRIX _view;
XMMATRIX _proj;
XMVECTOR _UpDirection;
XMMATRIX _billBoard;
int _field_angle;

//////////フレームワーク上でカメラを操作する時に必要な変数
XMFLOAT3 _fPosition;
XMFLOAT3 _fTarget;
XMVECTOR _fUpDirection;


//////////振動に必要な変数
float _vibrationQuantity;     //振動量
float _vibrationAttenuation;  //振動の減衰
bool  _vibrationFlag;         //カメラを振動させるかどうか
int   _sign;                  //符号

//初期化（プロジェクション行列作成）
void Camera::Initialize()
{
	ARGUMENT_INITIALIZE(_position,XMFLOAT3(ZERO, 50, -50));	            //カメラの位置
	ARGUMENT_INITIALIZE(_fPosition, _position);							//フレームワーク上でカメラを操作する時のカメラの位置
	ARGUMENT_INITIALIZE(_target,XMFLOAT3(ZERO, ZERO, ZERO));	        //カメラの焦点
	ARGUMENT_INITIALIZE(_fTarget, _target);								//フレームワーク上でカメラを操作する時のカメラの焦点
	ARGUMENT_INITIALIZE(_UpDirection,XMVectorSet(ZERO, 1, ZERO, ZERO)); //カメラの上方向のベクトル
	ARGUMENT_INITIALIZE(_fUpDirection, _UpDirection);					//フレームワーク上でカメラを操作する時のカメラの上方向のベクトル
	ARGUMENT_INITIALIZE(_field_angle,45);                               //カメラの画角
	ARGUMENT_INITIALIZE(_vibrationFlag,false);                          //カメラの振動Off
	ARGUMENT_INITIALIZE(_vibrationQuantity, ZERO);                      //振動量
	ARGUMENT_INITIALIZE(_vibrationAttenuation,0.01f);                   //振動の減衰
	ARGUMENT_INITIALIZE(_sign,1);                                       //符号

	//プロジェクション行列
	_proj = XMMatrixPerspectiveFovLH(XMConvertToRadians((float)_field_angle), (FLOAT)Direct3D::screenWidth_ / (FLOAT)Direct3D::screenHeight_, 0.1f, 1000.0f);
}

//更新（ビュー行列作成）
void Camera::Update()
{
	//カメラの振動 
	_target = Float3Add(_target,Vibration());

	//画面がゲーム状態なら
	if (Direct3D::GetScreenGameStatus())
	{
		//ビュー行列
		_view = XMMatrixLookAtLH(XMVectorSet(_position.x, _position.y, _position.z, ZERO),
			XMVectorSet(_target.x, _target.y, _target.z, ZERO), _UpDirection);

		//ビルボード行列
		//（常にカメラの方を向くように回転させる行列。パーティクルでしか使わない）
		_billBoard = XMMatrixLookAtLH(XMVectorSet(ZERO, ZERO, ZERO, ZERO), XMLoadFloat3(&_target) - XMLoadFloat3(&_position), _UpDirection);
		_billBoard = XMMatrixInverse(nullptr, _billBoard);
	}
	else
	{
		//ビュー行列
		_view = XMMatrixLookAtLH(XMVectorSet(_fPosition.x, _fPosition.y, _fPosition.z, ZERO),
			XMVectorSet(_fTarget.x, _fTarget.y, _fTarget.z, ZERO), _fUpDirection);

		//ビルボード行列
		//（常にカメラの方を向くように回転させる行列。パーティクルでしか使わない）
		_billBoard = XMMatrixLookAtLH(XMVectorSet(ZERO, ZERO, ZERO, ZERO), XMLoadFloat3(&_fTarget) - XMLoadFloat3(&_fPosition), _fUpDirection);
		_billBoard = XMMatrixInverse(nullptr, _billBoard);
	}

	
}

//カメラの振動
XMFLOAT3 Camera::Vibration()
{
	//振動量どんどん減らしておく
	if (abs(_vibrationQuantity) < 0.01)
		_vibrationQuantity = ZERO;
	else
		_vibrationQuantity = _sign * (abs(_vibrationQuantity) - _vibrationAttenuation);

	//符号反対に
	_sign *= -1;

	return XMFLOAT3(_vibrationQuantity, _vibrationQuantity, _vibrationQuantity);
}

//補間移動
void Camera::InterpolationMove(const XMFLOAT3& pos, const XMFLOAT3& tar, const float& factor)
{
	//カメラのポジションとターゲットセット(補間しながら変更)
	XMVECTOR vCamPos = XMVectorLerp(XMLoadFloat3(&_position), XMLoadFloat3(&pos), factor);
	XMVECTOR vCamTar = XMVectorLerp(XMLoadFloat3(&_target), XMLoadFloat3(&tar), factor);
	ARGUMENT_INITIALIZE(_position,VectorToFloat3(vCamPos));
	ARGUMENT_INITIALIZE(_target,VectorToFloat3(vCamTar));
}

//焦点を設定
void Camera::SetTarget(const XMFLOAT3& target) { _target = target;}

//位置を設定
void Camera::SetPosition(const XMFLOAT3& position) { _position = position; }

//上方向のベクトルを設定
void Camera::SetUpDirection(XMVECTOR direction) { _UpDirection = direction; }

//画角を設定
void Camera::SetFieldAngle(int angle) 
{
	//画角更新
	_field_angle = angle;
	//プロジェクション行列
	_proj = XMMatrixPerspectiveFovLH(XMConvertToRadians((float)_field_angle), (FLOAT)Direct3D::screenWidth_ / (FLOAT)Direct3D::screenHeight_, 0.1f, 1000.0f);
}

//フレームワーク上のカメラを初期化
void Camera::FrameCameraInitialize()
{
	ARGUMENT_INITIALIZE(_fPosition, _position);
	ARGUMENT_INITIALIZE(_fTarget, _target);
	ARGUMENT_INITIALIZE(_fUpDirection, _UpDirection);
}

//焦点を取得
XMFLOAT3 Camera::GetTarget() { return _target; }

//カメラ振動
//引数 : 振動の強さ
void Camera::SetCameraVibration(float strength,float attenuation)
{
	//強さ設定
	ARGUMENT_INITIALIZE(_vibrationQuantity, strength);

	//減衰設定
	ARGUMENT_INITIALIZE(_vibrationAttenuation, attenuation);

	//符号を1にしておく
	ARGUMENT_INITIALIZE(_sign,1);

}

//位置を取得
XMFLOAT3 Camera::GetPosition() { return _position; }

//ビュー行列を取得
XMMATRIX Camera::GetViewMatrix() { return _view; }

//プロジェクション行列を取得
XMMATRIX Camera::GetProjectionMatrix() { return _proj; }

//ビルボード用回転行列を取得
XMMATRIX Camera::GetBillboardMatrix() { return _billBoard; }
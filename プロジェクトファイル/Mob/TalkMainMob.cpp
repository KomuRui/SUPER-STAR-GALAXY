#include "TalkMainMob.h"
#include "../Engine/GUI/ImGuiSet.h"
#include "../Engine/ResourceManager/Audio.h"
#include "../Manager/GameManager/GameManager.h"
#include "../Engine/ResourceManager/CreateStage.h"
#include "../Scene/WorldScene/World2/WorldStage2.h"
#include "../Engine/ResourceManager/Fade.h"
#include "../Engine/ResourceManager/Audio.h"

//定数
namespace
{
	////////////////////////////アニメーション////////////////////////////

	static const int START_FRAME = 1;	  //開始フレーム
	static const int END_FRAME = 120;     //終了フレーム
	static const float ANIM_SPEED = 1.0f; //アニメーションの再生速度

	////////////////////////////その他////////////////////////////

	static const float PLAYER_LOOK_DISTANCE = 8; //Playerの方を向く時の距離
}

//コンストラクタ
TalkMainMob::TalkMainMob(GameObject* parent, std::string modelPath, std::string name)
	:Mob(parent, modelPath, name), isTalk_(false), toPlayer_(XMMatrixIdentity()), isLookPlayer_(false), hPict_(-1),
	pTalkImage_(nullptr), hAudio_(-1)
{}

//デストラクタ
TalkMainMob::~TalkMainMob()
{
	Audio::Stop(hAudio_);
}

//更新の前に一度だけ呼ばれる
void TalkMainMob::ChildStartUpdate()
{
	///////////////画像・音データのロード///////////////////

	hPict_ = Image::Load("Image/Text/Speak.png");
	assert(hPict_ >= ZERO);

	hAudio_ = Audio::Load("Audio/BGM/World2/World2.wav");
	assert(hAudio_ >= ZERO);

	//ボスステージなら
	if (GameManager::GetpSceneManager()->GetSceneId() == SCENE_ID_WORLD2) Audio::PlayLoop(hAudio_);

	////////////画像のトランスフォーム設定///////////////

	ARGUMENT_INITIALIZE(tPict_.position_.x, GetPrivateProfilefloat("POSITION", "TalkX", "1", "Image/Text/TextImagePosition.ini"));
	ARGUMENT_INITIALIZE(tPict_.position_.y, GetPrivateProfilefloat("POSITION", "TalkY", "1", "Image/Text/TextImagePosition.ini"));
	ARGUMENT_INITIALIZE(tPict_.position_.z, 1.0f);

	ARGUMENT_INITIALIZE(tPict_.scale_.x, GetPrivateProfilefloat("SCALE", "TalkX", "1", "Image/Text/TextImagePosition.ini"));
	ARGUMENT_INITIALIZE(tPict_.scale_.y, GetPrivateProfilefloat("SCALE", "TalkY", "1", "Image/Text/TextImagePosition.ini"));
	ARGUMENT_INITIALIZE(tPict_.scale_.z, 1.0f);
}

//更新
void TalkMainMob::ChildUpdate()
{
	//Playerの方を向く
	LookPlayer();

	//もし話している状態なら
	if (isTalk_)
	{
		//文字が最後まで描画されていてかつXボタンを押したのなら
		if (pTalkImage_->IsLastDraw() && Input::IsPadButtonDown(XINPUT_GAMEPAD_X))
		{
			//画像削除
			pTalkImage_->KillMe();

			//話していない状態に
			ARGUMENT_INITIALIZE(isTalk_, false);

			//カメラ
			GameManager::GetpPlayer()->SetCamLong();

			//Player動くように
			GameManager::GetpPlayer()->SetAnimFlag(true);
			GameManager::GetpPlayer()->Enter();

			//ボスのステージかラストシーンならフェードイン
			if (GameManager::GetpSceneManager()->GetSceneId() == SCENE_ID_WORLD2 || GameManager::GetpSceneManager()->GetSceneId() == SCENE_ID_LAST)
			{
				//フェードイン
				Fade::SetFadeStatus(FADE_NORMAL_IN,"Image/Fade/NormalFade.png");
			}
		}
		else
		{
			//カメラ
			GameManager::GetpPlayer()->SetCamShort();

			//Playerをこのオブジェクトの方向くようにする(球体上の時と平面の時で少し変化させる)
			if(GameManager::GetpStage()->GetCircleflag())
				GameManager::GetpPlayer()->LookObject(transform_.position_, GameManager::GetpPlayer()->GetUp());
			else
				GameManager::GetpPlayer()->LookObject(VectorToFloat3(XMLoadFloat3(&transform_.position_) - vNormal_ * 2), GameManager::GetpPlayer()->GetUp());

			//Player動かないようにする
			GameManager::GetpPlayer()->SetAnimFlag(false);
			GameManager::GetpPlayer()->Leave();
		}
	}


	//ボスのステージかつフェードが最後まで終了していたらステージを削除してムービーのシーン作成
	if (GameManager::GetpSceneManager()->GetSceneId() == SCENE_ID_WORLD2 && Fade::isNormalFadeNotTransparency())
	{
		//削除
		GameManager::GetpStage()->GetCreateStage()->AllCreateStageDelete();

		//ムービー作成
		((WorldStage2*)GetParent())->CreateMovie();

		//フェードアウト
		Fade::SetFadeStatus(FADE_NORMAL_OUT);
	}

	//ラストシーンかつフェードが最後まで終了していたらステージを削除してムービーのシーン作成
	if (GameManager::GetpSceneManager()->GetSceneId() == SCENE_ID_LAST && Fade::isNormalFadeNotTransparency())
	{
		//ロード画面を描画しない
		GameManager::GetpSceneManager()->SetLoadDrawFlag(false);

		//エンドロール流す
		GameManager::GetpSceneManager()->ChangeScene(SCENE_ID_ENDROLES);
	}
}

//描画
void TalkMainMob::ChildDraw()
{
	//Playerとの距離が指定距離以内かつ話していない状態なら画像表示
	if (IsInSpecifiedDistance() && !isTalk_)
	{
		Image::SetTransform(hPict_, tPict_);
		Image::Draw(hPict_);
	}
}

//Playerの方を向く
void TalkMainMob::LookPlayer()
{
	//もし一回でもPlayerの方向いていたら
	if (isLookPlayer_)
		transform_.mmRotate_ = XMMatrixInverse(nullptr, toPlayer_);

	//Playerとの距離が指定距離以内ならPlayerの方を向く
	if (IsInSpecifiedDistance())
	{
		//もし話していない状態でBボタンを押したのなら
		if (!isTalk_ && Input::IsPadButtonDown(XINPUT_GAMEPAD_X))
		{
			//話している時の画像・テキスト表示
			ARGUMENT_INITIALIZE(pTalkImage_,Instantiate<TalkImage>(GetParent()->GetParent()->GetParent()));

			//話している状態に
			ARGUMENT_INITIALIZE(isTalk_, true);
		}

		//Playerの方を向く
		toPlayer_ = XMMatrixLookAtLH(XMVectorSet(ZERO, ZERO, ZERO, ZERO), XMLoadFloat3(new XMFLOAT3(GameManager::GetpPlayer()->GetPosition())) - XMLoadFloat3(&transform_.position_), up_);
		transform_.mmRotate_ = XMMatrixInverse(nullptr, toPlayer_);

		//向いたらtrueにする
		ARGUMENT_INITIALIZE(isLookPlayer_, true);
	}
}

//指定距離内に入っているかどうか
bool TalkMainMob::IsInSpecifiedDistance()
{
	return (RangeCalculation(transform_.position_, GameManager::GetpPlayer()->GetPosition()) < (PLAYER_LOOK_DISTANCE * transform_.scale_.z));
}
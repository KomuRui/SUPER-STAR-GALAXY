#include "TalkImage.h"
#include "../Engine/ResourceManager/Image.h"
#include "../Engine/ResourceManager/Audio.h"
#include "../Engine/DirectX/Input.h"
#include "../Engine/GUI/ImGuiSet.h"
#include "../Engine/ResourceManager/CsvReader.h"
#include "../Manager/TextManager/TextManager.h"
#include "../Manager/GameManager/GameManager.h"
#include "../Manager/AudioManager/OtherAudioManager/OtherAudioManager.h"

//定数
namespace
{
	//文字
	static const float TEXT_SCALE = 0.3f;

	//描画スピード
	static const float NORMAL_DRAW_SPEED = 0.1f; //普通
	static const float FAST_DRAW_SPEED = 0.05f;  //速い
}

//コンストラクタ
TalkImage::TalkImage(GameObject* parent)
	: GameObject(parent, "TalkImage"), hBasePict_(-1),hCharaPict_(-1), hNextPict_(-1), drawTextNum_((int)ZERO),
	isLastDraw_(false), pText_(new Text), isButtonPushTextNext_(true), hAudio_(-1)
{
}

//デストラクタ
TalkImage::~TalkImage()
{
	//音止める
	Audio::Stop(hAudio_);
}

//初期化
void TalkImage::Initialize()
{
	///////////////文字のロード///////////////////

	//初期化
	pText_->Initialize(NORMAL_DRAW_SPEED);

	//文字を外部から取得
	pCsv_ = new CsvReader(TextManager::GetText(GameManager::GetpSceneManager()->GetSceneId()));

	///////////////画像・音データのロード///////////////////

	hBasePict_ = Image::Load("Image/Text/Talk.png");
	assert(hBasePict_ >= ZERO);

	//ボスのシーンなら
	if (GameManager::GetpSceneManager()->GetSceneId() == SCENE_ID_WORLD2)
	{
		hCharaPict_ = Image::Load("Image/MainMob/MainBossImage.png");
		assert(hCharaPict_ >= ZERO);
	}
	//ラストシーンなら
	else if (GameManager::GetpSceneManager()->GetSceneId() == SCENE_ID_LAST)
	{
		hCharaPict_ = Image::Load("Image/Princess/PrincessImage.png");
		assert(hCharaPict_ >= ZERO);
	}
	//それ以外なら
	else
	{
		hCharaPict_ = Image::Load("Image/MainMob/MainMobImage.png");
		assert(hCharaPict_ >= ZERO);
	}

	hNextPict_ = Image::Load("Image/Text/Next.png");
	assert(hNextPict_ >= ZERO);

	hAudio_ = Audio::Load("Audio/SE/Mob/Talk.wav");
	assert(hAudio_ >= ZERO);

	Audio::PlayLoop(hAudio_);

	/////////////////////////各Transform/////////////////////////

	//ベース
	ARGUMENT_INITIALIZE(tBase_.position_.x, GetPrivateProfilefloat("POSITION", "BaseX", "1", "Image/Text/TextImagePosition.ini"));
	ARGUMENT_INITIALIZE(tBase_.position_.y, GetPrivateProfilefloat("POSITION", "BaseY", "1", "Image/Text/TextImagePosition.ini"));
	ARGUMENT_INITIALIZE(tBase_.position_.z,1.0f);

	ARGUMENT_INITIALIZE(tBase_.scale_.x, GetPrivateProfilefloat("SCALE", "BaseX", "2.8", "Image/Text/TextImagePosition.ini"));
	ARGUMENT_INITIALIZE(tBase_.scale_.y, GetPrivateProfilefloat("SCALE", "BaseY", "0.8", "Image/Text/TextImagePosition.ini"));
	ARGUMENT_INITIALIZE(tBase_.scale_.z, 1.0f);

	//キャラ
	ARGUMENT_INITIALIZE(tChara_.position_.x, GetPrivateProfilefloat("POSITION", "CharaX", "1", "Image/Text/TextImagePosition.ini"));
	ARGUMENT_INITIALIZE(tChara_.position_.y, GetPrivateProfilefloat("POSITION", "CharaY", "1", "Image/Text/TextImagePosition.ini"));
	ARGUMENT_INITIALIZE(tChara_.position_.z, 1.0f);

	ARGUMENT_INITIALIZE(tChara_.scale_.x, GetPrivateProfilefloat("SCALE", "CharaX", "1", "Image/Text/TextImagePosition.ini"));
	ARGUMENT_INITIALIZE(tChara_.scale_.y, GetPrivateProfilefloat("SCALE", "CharaY", "1", "Image/Text/TextImagePosition.ini"));
	ARGUMENT_INITIALIZE(tChara_.scale_.z, 1.0f);

	//Next
	ARGUMENT_INITIALIZE(tNext_.position_.x, GetPrivateProfilefloat("POSITION", "NextX", "1", "Image/Text/TextImagePosition.ini"));
	ARGUMENT_INITIALIZE(tNext_.position_.y, GetPrivateProfilefloat("POSITION", "NextY", "1", "Image/Text/TextImagePosition.ini"));
	ARGUMENT_INITIALIZE(tNext_.position_.z, 1.0f);

	ARGUMENT_INITIALIZE(tNext_.scale_.x, GetPrivateProfilefloat("SCALE", "NextX", "1", "Image/Text/TextImagePosition.ini"));
	ARGUMENT_INITIALIZE(tNext_.scale_.y, GetPrivateProfilefloat("SCALE", "NextY", "1", "Image/Text/TextImagePosition.ini"));
	ARGUMENT_INITIALIZE(tNext_.scale_.z, 1.0f);
}

//更新の前に一度だけ呼ばれる
void TalkImage::StartUpdate()
{
}

//更新
void TalkImage::Update()
{
	//Bが押されているのなら
	if (Input::IsPadButton(XINPUT_GAMEPAD_X))
		pText_->SetDrawSpeed(FAST_DRAW_SPEED);
	else
		pText_->SetDrawSpeed(NORMAL_DRAW_SPEED);
}

//描画
void TalkImage::Draw()
{
	/////////////////////////////画像描画///////////////////////////

	Image::SetTransform(hBasePict_, tBase_);
	Image::Draw(hBasePict_);

	Image::SetTransform(hCharaPict_, tChara_);
	Image::SetUi(hCharaPict_); //手前に表示するためSetUiを使う

	/////////////////////////////文字をワイド文字列に変換///////////////////////////

    wchar_t wtext[FILENAME_MAX];
	std::string text = pCsv_->GetString(drawTextNum_, (int)ZERO);
	size_t ret;
	setlocale(LC_ALL, ".932");
	mbstowcs_s(&ret, wtext, text.c_str(), strlen(text.c_str()));

	///////////////////文字描画:もし文字が最後まで描画していたら///////////////////

	if (pText_->SlowlyDraw(1050, 800, wtext, TEXT_SCALE))
	{
		//最後の文字列を描画し終わっているのなら
		if (drawTextNum_ >= pCsv_->GetLines() - 1)
			ARGUMENT_INITIALIZE(isLastDraw_, true);

		//もしボタンを押して文字列を次へ更新するのならば
		if (isButtonPushTextNext_)
		{
			//Next画像を表示
			Image::SetTransform(hNextPict_, tNext_);
			Image::SetUi(hNextPict_);

			//音止める
			Audio::Stop(hAudio_);

			//文字列を次へ更新
			ButtonPushDrawTextNext();
		}
		else
			Audio::Stop(hAudio_);

	}
}

//解放
void TalkImage::Release()
{
}


//ボタンを押して描画する文字列を次の文字列に更新する
void TalkImage::ButtonPushDrawTextNext()
{
	//もしXボタンを押したなら次の文字列へ
	if (Input::IsPadButtonDown(XINPUT_GAMEPAD_X))
	{
		OtherAudioManager::ClickAudio(); //クリック音
		Audio::PlayLoop(hAudio_);        //会話音ループ
		NextText();
	}
}

//次の文字列へ
void TalkImage::NextText()
{
	//描画する文字列変更
	drawTextNum_++;

	//描画できる文字総数を初期化
	pText_->SetTotalDrawNum((int)ZERO);

	//最大文字列以上かつループするなら初期化
	if (drawTextNum_ >= pCsv_->GetLines())
		ARGUMENT_INITIALIZE(drawTextNum_, (int)ZERO);
}


//新しくCsv読み込みをする
void TalkImage::NewCsvFile(std::string fileNamePath)
{
	//読み込み
	ARGUMENT_INITIALIZE(pCsv_,new CsvReader(fileNamePath));

	//初期化
	ARGUMENT_INITIALIZE(isLastDraw_, false);
}

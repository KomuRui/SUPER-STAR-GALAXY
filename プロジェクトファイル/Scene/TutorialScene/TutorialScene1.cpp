#include "TutorialScene1.h"
#include "../../Manager/SceneManager/SceneManager.h"
#include "../../Player/Player2D.h"
#include "../../Scene/TutorialScene/TutorialStage1.h"
#include "../../Manager/GameManager/GameManager.h"
#include "../../Engine/DirectX/Input.h"
#include "../../Engine/ResourceManager/Audio.h"
#include "../../Engine/ResourceManager/Fade.h"
#include "../../Engine/ResourceManager/Image.h"

//コンストラクタ
TutorialScene1::TutorialScene1(GameObject* parent)
	: GameObject(parent, "TutorialScene1"),hAudio_(-1)
{

}

//デストラクタ
TutorialScene1::~TutorialScene1()
{
	Audio::Stop(hAudio_);
}

//初期化
void TutorialScene1::Initialize()
{
	//ステージの表示(これから使うステージのポインタをGameManagerにセット)
	GameManager::SetpStage(Instantiate<TutorialStage1>(this));

	//Player表示(これから使うステージのポインタをGameManagerにセット)
	GameManager::SetpPlayer(Instantiate<Player2D>(this));

	//フェードイン
	Fade::SetFadeStatus(FADE_CIRCLE_IN);

	//音ロード
	hAudio_ = Audio::Load("Audio/BGM/Tutorial/TutorialScene.wav");
	assert(hAudio_ >= ZERO);

	//音
	Audio::PlayLoop(hAudio_);
}

//更新の前に一度だけ呼ばれる関数
void TutorialScene1::StartUpdate()
{
}

//更新
void TutorialScene1::Update()
{
}

//描画
void TutorialScene1::Draw()
{
}

//開放
void TutorialScene1::Release()
{
}


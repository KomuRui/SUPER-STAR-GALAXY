#include "sceneManager.h"
#include "../../Engine/ResourceManager/Model.h"
#include "../../Engine/ResourceManager/Image.h"
#include "../../Engine/ResourceManager/Audio.h"
#include "../../Engine/GameObject/Light.h"
#include "../../Engine/ResourceManager/Time.h"
#include "../../Engine/ResourceManager/Fade.h"
#include "../../Scene/TitleScene/TitleScene.h"
#include "../../Scene/TutorialScene/TutorialScene1.h"
#include "../../Scene/TutorialScene/TutorialScene2.h"
#include "../../Scene/HomeScene/HomeScene.h"
#include "../../Scene/WorldScene/World1/WorldScene1.h"
#include "../../Scene/WorldScene/World2/WorldScene2.h"
#include "../../Manager/GameManager/GameManager.h"
#include "../../Scene/UserSelectScene/UserSelectScene.h"
#include "../../Scene/LastScene/LastScene.h"
#include "../../Scene/EndRolesScene/EndRolesScene.h"
#include "../../Scene/StageSelectScene/StageSelectScene.h"
#include "../../Manager/CoinManager/CoinManager.h"
#include "../../Manager/LifeManager/LifeManager.h"
#include "../../Manager/EffectManager/PlayerEffectManager/PlayerEffectManager.h"
#include "../../Manager/EffectManager/EnemyEffectManager/EnemyEffectManager.h"
#include "../../Manager/EffectManager/CoinEffectManager/CoinEffectManager.h"
#include "../../Scene/MiniGameScene/MiniGameScene.h"
#include "../../Manager/MiniGameManager/MiniGameManager.h"
#include "../../Scene/MiniGameLevelSelectScene/MiniGameLevelSelectScene.h"
#include "../../Manager/ButtonManager/ButtonManager.h"
#include "../../Engine/ResourceManager/VFX.h"


//コンストラクタ
SceneManager::SceneManager(GameObject * parent)
	: GameObject(parent, "SceneManager"), loadDrawFlag_(true)
{
}

//初期化
void SceneManager::Initialize()
{
	//自身を追加
	GameManager::SetpSceneManager(this);

	//最初のシーンを準備
	currentSceneID_ = SCENE_ID_TITLE;
	nextSceneID_ = currentSceneID_;
	Instantiate<TitleScene>(this);
}

//更新
void SceneManager::Update()
{
	//次のシーンが現在のシーンと違う　＝　シーンを切り替えなければならない
	if (currentSceneID_ != nextSceneID_)
	{
		//ロード中に描画するなら
		if (loadDrawFlag_)
		{
			//ロードしているとき用の画像表示(すぐ表示したいので描画終了を呼ぶ)
			Fade::SetFadeStatus(DRAW);
			Direct3D::EndDraw();
		}

		//そのシーンのオブジェクトを全削除
		KillAllChildren();

		//ロードしたデータを全削除
		ButtonManager::Reset();
		VFX::Release();
		Audio::AllRelease();
		Model::AllRelease();
		Image::AllRelease();
		Time::AllRelease();

		//シーン遷移時の初期化
		GameManager::SceneTransitionInitialize();

		//次のシーンを作成
		switch (nextSceneID_)
		{
		case SCENE_ID_TITLE:				 Instantiate<TitleScene>(this); break;
		case SCENE_ID_USER_SELECT:			 Instantiate<UserSelectScene>(this); break;
		case SCENE_ID_TUTORIAL1:		     Instantiate<TutorialScene1>(this); break;
		case SCENE_ID_TUTORIAL2:		     Instantiate<TutorialScene2>(this); break;
		case SCENE_ID_STAGE_SELECT:		     Instantiate<StageSelectScene>(this); break;
		case SCENE_ID_HOME:     		     Instantiate<HomeScene>(this); break;
		case SCENE_ID_MINIGAME:              Instantiate<MiniGameScene>(this); break;
		case SCENE_ID_MINIGAME_LEVEL_SELECT: Instantiate<MiniGameLevelSelectScene>(this); break;
		case SCENE_ID_WORLD1:                Instantiate<WorldScene1>(this); break;
		case SCENE_ID_WORLD2:                Instantiate<WorldScene2>(this); break;
		case SCENE_ID_LAST:					 Instantiate<LastScene>(this); break;
		case SCENE_ID_ENDROLES:              Instantiate<EndRolesScene>(this); break;
		}
		currentSceneID_ = nextSceneID_;

		//ロード中に描画するに初期化しておく
		ARGUMENT_INITIALIZE(loadDrawFlag_, true);
	}

}

//描画
void SceneManager::Draw()
{
}

//開放
void SceneManager::Release()
{
}

void SceneManager::StartUpdate()
{
}

//シーン切り替え（実際に切り替わるのはこの次のフレーム）
void SceneManager::ChangeScene(SCENE_ID next)
{
	nextSceneID_ = next;

}

//現在のシーンIDゲット
SCENE_ID SceneManager::GetSceneId()
{
	return nextSceneID_;
}

//ロードをしてる時に画像表示するかセットする
void SceneManager::SetLoadDrawFlag(bool flag)
{
	ARGUMENT_INITIALIZE(loadDrawFlag_, flag);
}
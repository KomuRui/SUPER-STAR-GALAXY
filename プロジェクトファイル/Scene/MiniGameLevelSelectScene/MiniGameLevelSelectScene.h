#pragma once
#include "../../Engine/GameObject/GameObject.h"

/// <summary>
/// ミニゲームのレベルを選択するシーン
/// </summary>
class MiniGameLevelSelectScene : public GameObject
{

	int hAudio_;    //音番号

public:

	//コンストラクタ
	MiniGameLevelSelectScene(GameObject* parent);

	//デストラクタ
	~MiniGameLevelSelectScene();

	//初期化
	void Initialize() override;

	//更新の前に一度だけ呼ばれる
	void StartUpdate() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;

};


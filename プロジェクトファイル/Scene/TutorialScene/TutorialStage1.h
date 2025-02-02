#pragma once
#include "../../Stage.h"

//ステージクラス
class TutorialStage1 : public Stage
{

public:

	//コンストラクタ
	//引数：parent  親オブジェクト（SceneManager）
	TutorialStage1(GameObject* parent);

	//初期化
	void Initialize() override;

	//更新の前に一回呼ばれる関数
	void StartUpdate() override {};

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;

};


#pragma once
#include "../Engine/GameObject/GameObject.h"

//海のモデル
class Water : public GameObject
{
	int hModel_;   			//モデル番号
	std::string filePath_;  //ファイルのパス

	float scroll;           //UVスクロール

public:

	//コンストラクタ
	Water(GameObject* parent, std::string modelFilePath_, std::string name);

	/////////////////////オーバーライドする関数//////////////////////

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


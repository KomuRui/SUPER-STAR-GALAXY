#pragma once
#include "../Engine/GameObject.h"

/// <summary>
/// ユーザー情報選択シーンのUI(画像とか文字)
/// </summary>
class UserSelectSceneUI : public GameObject
{
public:

	//コンストラクタ
	UserSelectSceneUI(GameObject* parent, std::string modelPath, std::string name);

	/////////////////////オーバーライドする関数//////////////////////

	//初期化
	void Initialize() override;

	//更新の前に一回呼ばれる関数
	void StartUpdate() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;
};


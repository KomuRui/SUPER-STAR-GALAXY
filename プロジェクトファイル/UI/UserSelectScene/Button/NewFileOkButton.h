#pragma once
#include "../../Button/ButtonBase.h"

/// <summary>
/// 新しいファイル作るときのOKボタン
/// </summary>
class NewFileOkButton : public ButtonBase
{

	int hSelectPict_;    //選択されているときの画像番号
	int hNotSelectPict_; //選択されていないときの画像番号

public:

	//コンストラクタ
	NewFileOkButton(GameObject* parent, std::string modelPath, std::string name);

	////////////////////オーバーライドする関数/////////////////////////

	/// <summary>
	/// 初期化
	/// </summary>
	void ChildInitialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void ChildButtonUpdate() override;

	/// <summary>
	/// ボタンが押されたら何するか
	/// </summary>
	/// <param name="p">実行したい関数ポインタ</param>
	void IsButtonPush() override;

	/// <summary>
	/// ボタンが選択された瞬間に何をするか
	/// </summary>
	/// <param name="p">実行したい関数ポインタ</param>
	void IsButtonSelect() override;

	/// <summary>
	/// ボタンが選択解除された瞬間に何をするか
	/// </summary>
	/// <param name="p">実行したい関数ポインタ</param>
	void IsButtonSelectRelease() override;
};


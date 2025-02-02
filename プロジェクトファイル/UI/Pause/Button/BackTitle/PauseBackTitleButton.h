#pragma once
#include "../../../Button/ButtonBase.h"


/// <summary>
/// タイトルに戻るボタン
/// </summary>
class PauseBackTitleButton : public ButtonBase
{

	int hSelectPict_;    //選択されているときの画像番号
	int hSelectPict2_;   //選択されているときに表示する2つ目の画像
	int hNotSelectPict_; //選択されていないときの画像番号

public:

	//コンストラクタ
	PauseBackTitleButton(GameObject* parent, std::string modelPath, std::string name);

	////////////////////オーバーライドする関数/////////////////////////

	/// <summary>
	/// 初期化
	/// </summary>
	void ChildInitialize() override;

	/// <summary>
	/// 描画
	/// </summary>
	void ChildDraw() override;

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
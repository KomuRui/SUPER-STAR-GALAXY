#pragma once
#include "../ImageBase.h"

/// <summary>
/// ボタンクラス
/// </summary>
class ButtonBase : public ImageBase
{
protected:

	//ボタンが選択されているとき何をするかの関数ポインタ
	void(*ButtonSelectingFunc)(void);

	//押したときの関数ポインタ
	void(*OnPushFunc)(void);

	//ボタン選択された瞬間に呼ぶ関数ポインタ
	void(*ButtonSelectFunc)(void);

	//ボタンが選択解除された瞬間に呼ぶ関数ポインタ
	void (*ButtonSelectReleaseFunc)(void);

	//選択されているかどうか
	bool isSelect_;

public:

	//コンストラクタ
	ButtonBase(GameObject* parent, std::string modelPath, std::string name);
	
	////////////////////オーバーライドする関数/////////////////////////

	//更新
	void ChildUpdate() override;

	/////////////////////////////関数////////////////////////////////

	/// <summary>
	/// ボタンが選択されているとき何をするか
	/// </summary>
	/// <param name="p">実行したい関数ポインタ</param>
	void IsButtonSelecting(void (*p)()) { p(); }

	/// <summary>
	/// ボタンが押されたら何するか
	/// </summary>
	/// <param name="p">実行したい関数ポインタ</param>
    void IsButtonPush(void (*p)()) { p(); }

	/// <summary>
	/// ボタンが選択された瞬間に何をするか
	/// </summary>
	/// <param name="p">実行したい関数ポインタ</param>
	void IsButtonSelect(void (*p)()) { p(); }

	/// <summary>
	/// ボタンが選択解除された瞬間に何をするか
	/// </summary>
	/// <param name="p">実行したい関数ポインタ</param>
	void IsButtonSelectRelease(void (*p)()) { p(); }

	//////////////////////////セットゲット関数/////////////////////////////

	/// <summary>
	/// 選択されているかをセット 
	/// </summary>
	/// <param name="flag">trueなら選択されてる</param>
	void SetSelect(bool flag);

	/// <summary>
	/// 選択されているかを取得
	/// </summary>
	/// <returns>trueなら選択されている</returns>
	bool IsSelect() { return isSelect_; }
};


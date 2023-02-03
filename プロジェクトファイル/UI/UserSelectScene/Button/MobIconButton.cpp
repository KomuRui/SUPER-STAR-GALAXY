#include "MobIconButton.h"
#include "../../../Engine/Global.h"
#include "../../../Engine/Image.h"
#include "../NewFileUI.h"

//定数
namespace
{
	static const wchar_t DRAW_TEXT[] = L"Mr.D";   //描画する文字
	static const int DRAW_X = 750;                //描画位置X
	static const int DRAW_Y = 850;                //描画位置Y
}

//コンストラクタ
MobIconButton::MobIconButton(GameObject* parent, std::string modelPath, std::string name)
	:ButtonBase(parent, modelPath, name), hNotSelectPict_(-1), hSelectPict_(-1), NameText(new Text)
{}

//初期化
void MobIconButton::ChildInitialize()
{
	//初期化
	NameText->Initialize();

	//セレクト画像の番号取得
	ARGUMENT_INITIALIZE(hNotSelectPict_, hPict_);

	//ロード
	ARGUMENT_INITIALIZE(hSelectPict_, Image::Load("Image/UserSelect/IconSelect/MobIcon_Select.png"));
}

//描画
void MobIconButton::ChildDraw()
{
	//最後まで描画されたら最初から描画し直す
	if (isSelect_)NameText->Draw(DRAW_X, DRAW_Y, DRAW_TEXT, 0.8f);
}

//ボタンが押されたら何するか
void MobIconButton::IsButtonPush()
{
	//親に押されたことを報告
	((NewFileUI*)GetParent())->ChangeEasingMove();
}

//ボタンが選択された瞬間に何をするか
void MobIconButton::IsButtonSelect() { ARGUMENT_INITIALIZE(hPict_, hSelectPict_); }

//ボタンが選択解除された瞬間に何をするか
void MobIconButton::IsButtonSelectRelease() { ARGUMENT_INITIALIZE(hPict_, hNotSelectPict_); }
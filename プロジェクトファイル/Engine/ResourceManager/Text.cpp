#include <stdlib.h>
#include "../DirectX/Direct3D.h"
#include "Text.h"
#include "Global.h"
#include "../GameObject/Camera.h"
#include "../../Manager/TextManager/TextManager.h"
#include "../../Manager/GameManager/GameManager.h"
#include "../../Gimmick/Warp.h"


Text::Text() : hPict_(-1), width_(128), height_(256), fileName_("Text/MainFont.png"), rowLength_(16), speed_(1.0f), fpsCount_(0), totalDrawNum_(1)
{
}

Text::~Text()
{
}

//初期化（デフォルト）
HRESULT Text::Initialize(float speed, float textInterval)
{
	//画像のロード
	hPict_ = Image::Load(fileName_);
	assert(hPict_ >= 0);

	//60FPSと仮定する
	speed_ = speed * 60; 
	textInterval_ = textInterval;

	return S_OK;
}

//初期化（オリジナルの画像）
HRESULT Text::Initialize(const char* fileName, const unsigned int charWidth, const unsigned int charHeight, const unsigned int rowLength, float speed, float textInterval)
{
	strcpy_s(fileName_, fileName);
	width_ = charWidth;
	height_ = charHeight;
	rowLength_ = rowLength;
	speed_ = speed * 60; //60FPSと仮定する
	return Initialize();
}


//描画（文字列:数字）
void Text::NumberDraw(int x, int y, const char* str, float ratio, float textInterval)
{
	//表示位置（左上）を計算
	//Spriteクラスは中心が(0,0)、右上が(1,1)という座標だが、ここの引数は左上を(0,0)、ドット単位で指定している
	float px, py;

	//引数は左上原点だが、スプライトは画面中央が原点なので、画面サイズの半分ずらす
	px = (float)(x - Direct3D::screenWidth_ / 2);
	py = (float)(-y + Direct3D::screenHeight_ / 2);	//Y軸は+-反転

	//スプライトはPositionを1ずらすと画面サイズの半分ずれるので、ピクセル単位に変換
	px /= (float)(Direct3D::screenWidth_ / 2.0f);
	py /= (float)(Direct3D::screenHeight_ / 2.0f);


	//１文字ずつ表示する
	for (int i = 0; str[i] != '\0'; i++)	//文字列の末尾まで来たら終わり
	{
		//表示したい文字が、画像の何番目に書いてあるかを求める
		int id = str[i] - '0';

		//表示したい文字が、画像のどこにあるかを求める
		int x = id % rowLength_;	//左から何番目
		int y = id / rowLength_;	//上から何番目

		//表示する位置
		Transform transform;
		transform.position_.x = px;
		transform.position_.y = py;

		//大きさ
		transform.scale_.x *= ratio;
		transform.scale_.y *= ratio;

		Image::SetTransform(hPict_, transform);

		//表示する範囲
		Image::SetRect(hPict_, width_ * x , height_ * y, width_, height_);

		//表示
		Image::Draw(hPict_);

		//次の位置にずらす
		px += (width_ / (float)(Direct3D::screenWidth_ / 2.0f) * transform.scale_.x) - textInterval;
	}
}

//描画（文字列）一文字ごとに徐々に表示する
bool Text::SlowlyDraw(int x, int y, const wchar_t* str, float ratio, float textInterval)
{
	//表示位置（左上）を計算
	//Spriteクラスは中心が(0,0)、右上が(1,1)という座標だが、ここの引数は左上を(0,0)、ドット単位で指定している
	float px, py;

	//引数は左上原点だが、スプライトは画面中央が原点なので、画面サイズの半分ずらす
	px = (float)(x - Direct3D::screenWidth_ / 2);
	py = (float)(-y + Direct3D::screenHeight_ / 2);	//Y軸は+-反転

	//スプライトはPositionを1ずらすと画面サイズの半分ずれるので、ピクセル単位に変換
	px /= (float)(Direct3D::screenWidth_ / 2.0f);
	py /= (float)(Direct3D::screenHeight_ / 2.0f);

	//１文字ずつ表示する
	for (int i = 0; str[i] != '\0'; i++)	//文字列の末尾まで来たら終わり
	{
		//もし表示していい数より小さいなら
		if (i < totalDrawNum_)
		{
			//文字によって処理変える
			switch (str[i])
			{
				//改行
				case  '|':
				{
					//表示するXを初期化
					px = (float)(x - Direct3D::screenWidth_ / 2.0f);
					px /= (float)(Direct3D::screenWidth_ / 2.0f);

					//Yを少しずらす
					py -= 0.1f;

					break;
				}

				//空白ならずらす
				case ' ':
				{
					//次の位置にずらす
					px += (width_ / (float)(Direct3D::screenWidth_ / 2.0f)) + textInterval;

					break;
				}

				//カメラ移動
				case '{':
				{
					//Playerのカメラ動作させないように
					GameManager::GetpPlayer()->SetCamFlag(true);

					//カメラ設定
					Camera::SetUpDirection(UP_VECTOR);
					Camera::InterpolationMove({15,35,0}, { 0,25,0 }, 0.03f);
					break;
				}

				//カメラ元に戻す
				case '}':
				{
					//Playerのカメラ動作させるように
					GameManager::GetpPlayer()->SetCamFlag(false);
					break;
				}

				//通常
				default:
				{
					//表示したい文字が、画像の何番目に書いてあるかを求める
					int id = TextManager::GetNumber(str[i]);

					//表示したい文字が、画像のどこにあるかを求める
					int x = id % rowLength_;	//左から何番目
					int y = id / rowLength_;	//上から何番目

					//表示する位置
					Transform transform;
					transform.position_.x = px;
					transform.position_.y = py;

					//大きさ
					transform.scale_.x *= ratio;
					transform.scale_.y *= ratio;

					Image::SetTransform(hPict_, transform);

					//表示する範囲
					Image::SetRect(hPict_, width_ * x, height_ * y, width_, height_);

					//表示
					Image::Draw(hPict_);

					//次の位置にずらす
					px += (width_ / (float)(Direct3D::screenWidth_ / 2.0f) * transform.scale_.x) + textInterval;

					break;
				}
			}

		}
		else
			break;
	}

	//1ずつ増やす
	fpsCount_++;

	//もしfpsCount_がスピードより大きくなったら
	if (fpsCount_ > speed_)
	{
		ARGUMENT_INITIALIZE(fpsCount_, (int)ZERO);
		totalDrawNum_++;
	}

	//もし最後まで描画できているのなら
	if (totalDrawNum_ > wcslen(str))
		return true;

	return false;
}

//描画（文字列）を表示する
void Text::Draw(int x, int y, const wchar_t* str, float ratio, float textInterval)
{
	//表示位置（左上）を計算
	//Spriteクラスは中心が(0,0)、右上が(1,1)という座標だが、ここの引数は左上を(0,0)、ドット単位で指定している
	float px, py;

	//引数は左上原点だが、スプライトは画面中央が原点なので、画面サイズの半分ずらす
	px = (float)(x - Direct3D::screenWidth_ / 2);
	py = (float)(-y + Direct3D::screenHeight_ / 2);	//Y軸は+-反転

	//スプライトはPositionを1ずらすと画面サイズの半分ずれるので、ピクセル単位に変換
	px /= (float)(Direct3D::screenWidth_ / 2.0f);
	py /= (float)(Direct3D::screenHeight_ / 2.0f);

	//１文字ずつ表示する
	for (int i = 0; str[i] != '\0'; i++)	//文字列の末尾まで来たら終わり
	{
		//もし@なら改行
		if (str[i] == '|')
		{
			//表示するXを初期化
			px = (float)(x - Direct3D::screenWidth_ / 2.0f);
			px /= (float)(Direct3D::screenWidth_ / 2.0f);

			//Yを少しずらす
			py -= 0.1f;
		}
		//空白ならずらす
		else if (str[i] == ' ')
		{
			//次の位置にずらす
			px += (width_ / (float)(Direct3D::screenWidth_ / 2.0f)) + textInterval;
		}
		else
		{
			//表示したい文字が、画像の何番目に書いてあるかを求める
			int id = TextManager::GetNumber(str[i]);

			//表示したい文字が、画像のどこにあるかを求める
			int x = id % rowLength_;	//左から何番目
			int y = id / rowLength_;	//上から何番目

			//表示する位置
			Transform transform;
			transform.position_.x = px;
			transform.position_.y = py;

			//大きさ
			transform.scale_.x *= ratio;
			transform.scale_.y *= ratio;

			Image::SetTransform(hPict_, transform);

			//表示する範囲
			Image::SetRect(hPict_, width_ * x, height_ * y, width_, height_);

			//表示
			Image::Draw(hPict_);

			//次の位置にずらす
			px += (width_ / (float)(Direct3D::screenWidth_ / 2.0f) * transform.scale_.x) + textInterval;
		}
	}
}

//描画（整数値）
void Text::NumberDraw(int x, int y, int value, float ratio, float textInterval)
{
	//文字列に変換
	char str[256];
	sprintf_s(str, "%d", value);

	NumberDraw(x, y, str, ratio, textInterval);
}

//解放
void Text::Release()
{
	Image::Release(hPict_);
}

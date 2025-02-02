#include "CoinManager.h"
#include "../../Engine/GameObject/Transform.h"
#include "../../Engine/ResourceManager/Text.h"
#include "../../Engine/ResourceManager/Global.h"
#include "../../Engine/DirectX/Sprite.h"
#include "../../Manager/GameManager/GameManager.h"

/// <summary>
/// Playerの持ってるコイン管理
/// </summary>
namespace CoinManager
{
	///////////////////////////////変数//////////////////////////////////

	//コインの総数
	int coinTotalCount;

	//コインの総数を保存しておく用(ボスシーンで使われるため)
	int keepCoinTotal;

	//画像
	Sprite* pCoinImage;             //コインの画像
	Sprite* pCrossImage;            //×UIの画像
	Transform coinImageTransform_;  //コイン画像の位置・拡大率
	Transform crossImageTransform_; //×画像の位置・拡大率

	//テキスト
	Text* pCoinText_;           //コインの数表示するテキスト
	XMFLOAT2 textPositiom_;     //テキストのポジション

	///////////////////////////////関数//////////////////////////////////

	//初期化
	void CoinManager::Initialize()
	{
		//コインの総数初期化
		ARGUMENT_INITIALIZE(coinTotalCount, (int)ZERO);

		//画像のロード
		ARGUMENT_INITIALIZE(pCoinImage, new Sprite);
		ARGUMENT_INITIALIZE(pCrossImage, new Sprite);
		pCoinImage->Load("Image/Coin/Coin.png");
		pCrossImage->Load("Image/Coin/Cross.png");

		//テキストの初期化
		ARGUMENT_INITIALIZE(pCoinText_, new Text);
		pCoinText_->Initialize("Text/NumberFont.png", 128, 256, 10);

		//画像のポジション設定
		ARGUMENT_INITIALIZE(coinImageTransform_.position_.x, GetPrivateProfilefloat("POSITION", "coinImageX", "1", "Image/Coin/CoinPosition.ini"));
		ARGUMENT_INITIALIZE(coinImageTransform_.position_.y, GetPrivateProfilefloat("POSITION", "coinImageY", "1", "Image/Coin/CoinPosition.ini"));
		ARGUMENT_INITIALIZE(crossImageTransform_.position_.x, GetPrivateProfilefloat("POSITION", "crossImageX", "1", "Image/Coin/CoinPosition.ini"));
		ARGUMENT_INITIALIZE(crossImageTransform_.position_.y, GetPrivateProfilefloat("POSITION", "crossImageY", "1", "Image/Coin/CoinPosition.ini"));

		//テキストのポジション設定
		ARGUMENT_INITIALIZE(textPositiom_.x, GetPrivateProfilefloat("POSITION", "coinTextX", "1", "Image/Coin/CoinPosition.ini"))
		ARGUMENT_INITIALIZE(textPositiom_.y, GetPrivateProfilefloat("POSITION", "coinTextY", "1", "Image/Coin/CoinPosition.ini"))
	}

	//シーン遷移の時の初期化
	void CoinManager::SceneTransitionInitialize()
	{
		//テキストの初期化
		ARGUMENT_INITIALIZE(pCoinText_, new Text);
		pCoinText_->Initialize("Text/NumberFont.png", 128, 256, 10);
	}

	//コイン何枚持っているか描画
	void CoinManager::Draw()
	{
		//シーンがエンドロールなら描画しない
		if (GameManager::GetpSceneManager()->GetSceneId() == SCENE_ID_ENDROLES) return;

		//画像
		{
			//テクスチャのサイズ取得
			XMFLOAT3 size = pCoinImage->GetTextureSize();

			//切り抜き範囲をリセット（画像全体を表示する）
			RECT rect;
			rect.left = (LONG)ZERO;
			rect.top = (LONG)ZERO;
			rect.right = (long)size.x;
			rect.bottom = (long)size.y;

			//描画
			pCoinImage->Draw(coinImageTransform_, rect);
		}

		//テキスト
		{
			pCoinText_->NumberDraw((int)textPositiom_.x, (int)textPositiom_.y, coinTotalCount, coinImageTransform_.scale_.x,0.02f);
		}
	}


	//コインの追加
	void AddCoin(int num){ coinTotalCount += num; }

	//コインの減少
	void DecrementCoin(int num) { coinTotalCount -= num; }

	//コインの数取得
	int GetCoinNum() { return coinTotalCount; }

	//コインの数をセット
	void SetCoinNum(int num) { coinTotalCount = num; };

	//保存しているコイン数を既存コイン数にセット
	void SetCoinKeep() { coinTotalCount = keepCoinTotal; }

	//保存しているコイン数を更新
	void KeepCoinUpdate() { keepCoinTotal = coinTotalCount; }
}
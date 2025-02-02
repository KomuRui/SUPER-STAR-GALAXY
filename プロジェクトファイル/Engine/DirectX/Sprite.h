#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>

#include "string"
#include "Texture.h"
#include "../GameObject/Transform.h"

using namespace DirectX;


//-----------------------------------------------------------
//2D画像を表示するためのクラス
//とは言っても実態はポリゴンを平面的に表示しているだけ
//-----------------------------------------------------------
class Sprite
{
	// 一つの頂点情報を格納する構造体
	struct VERTEX
	{
		XMFLOAT3 position;
		XMFLOAT3 uv;
	};


	//【コンスタントバッファー】
	// GPU(シェーダ側)へ送る数値をまとめた構造体
	//Simple2D.hlslのグローバル変数と対応させる
	struct CONSTANT_BUFFER
	{
		XMMATRIX	world;		// 頂点座標変換行列
		XMMATRIX	uvTrans;	// テクスチャ座標変換行列
		XMFLOAT4	color;		// テクスチャとの合成色
		FLOAT       distance;   // 距離
		FLOAT    screenWidth;   // スクリーンの横
		FLOAT    screenHeight;  // スクリーンの縦
	};

	//【頂点バッファ】
	//各頂点の情報（位置とか色とか）を格納するところ
	//頂点数分の配列にして使う
	ID3D11Buffer *pVertexBuffer_;

	//【インデックスバッファ】
	//「どの頂点」と「どの頂点」と「どの頂点」で３角形ポリゴンになるかの情報を格納するところ
	ID3D11Buffer *pIndexBuffer_;

	//【テクスチャ】
	Texture*	pTexture_;

	//【定数バッファ】
	//シェーダー（Simple2D.hlsl）のグローバル変数に値を渡すためのもの
	ID3D11Buffer *pConstantBuffer_;


	//////////////////////////private関数（Load関数から呼ばれる）/////////////////////////////
	void InitVertex();			//頂点バッファ準備
	void InitIndex();			//インデックスバッファ準備
	void InitConstantBuffer();	//コンスタントバッファ（シェーダーに情報を送るやつ）準備


public:
	Sprite();
	~Sprite();

	//ロード
	HRESULT Load(std::string fileName);

	//初期化
	virtual HRESULT Initialize(ID3D11Texture2D* pTexture);

	//描画
	void Draw(Transform& transform, RECT rect, float alpha = 1.0f);
	void Draw(Transform& transform, float dis, RECT rect);
	void Draw(Transform& transform, float alpha);
	void Draw(Transform& transform);

	/// <summary>
	/// 色を反転する(透明色は指定した色にその他の色は透明に)
	/// </summary>
	/// <param name="transform">表示位置</param>
	/// <param name="rect">切り抜き範囲</param>
	/// <param name="color">色</param>
	void ReversalColorDraw(Transform& transform, RECT rect, XMFLOAT4 color);

	//画像サイズの取得
	//戻値：画像サイズ
	XMFLOAT3 GetTextureSize() {	return pTexture_->GetSize();}

};
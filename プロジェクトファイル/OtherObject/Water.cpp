#include "Water.h"
#include "../Engine/ResourceManager/Model.h"
#include "../Engine/ResourceManager/VFX.h"

//コンストラクタ
Water::Water(GameObject* parent, std::string modelFilePath_, std::string name)
	:GameObject(parent, name), hModel_(-1),filePath_(modelFilePath_)
{
}

//初期化
void Water::Initialize()
{
	//モデルデータのロード
	hModel_ = Model::Load(filePath_);
	ARGUMENT_INITIALIZE(pathName_, filePath_);
	assert(hModel_ >= ZERO);

	//ノーマルマップを使う
	Model::SetShederType(hModel_,Direct3D::SHADER_NORMALMAP);
}

//更新
void Water::Update()
{
	
	scroll += 0.002f;

	Model::SetUvScroll(hModel_, scroll);
}

//描画
void Water::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

//解放
void Water::Release()
{
}

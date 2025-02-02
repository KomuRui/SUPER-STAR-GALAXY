#include "TutorialStage2.h"
#include "../../Engine/ResourceManager/Model.h"
#include "../../Engine/GameObject/Light.h"
#include "../../Engine/GameObject/Camera.h"
#include "../../Gimmick/Warp.h"
#include "../../Engine/ResourceManager/CreateStage.h"

//コンストラクタ
TutorialStage2::TutorialStage2(GameObject* parent)
	:Stage(parent, "TutorialStage2")
{
	//画角
	ARGUMENT_INITIALIZE(fieldAngle_,100);

	//ライトの強さ
	ARGUMENT_INITIALIZE(lightIntensity_,3);
}

//初期化
void TutorialStage2::Initialize()
{

	/////////////////ファイル読み込んでステージの各オブジェクト設置///////////////////

	//////ステージ作成
	pCreateStage_ = new CreateStage;
	pCreateStage_->LoadFileCreateStage(this, "Stage/Tutorial/StageInformation/TutorialStage2.txt");

	//ワープのシーン遷移先を決めておく
	Warp* pWarp = (Warp*)FindObject("Warp");
	pWarp->SetSceneId(SCENE_ID_HOME);
	pWarp->SetStageRelease(2); //このステージをクリアしたら解放ステージ数が2になるようにする

	/////////////////////モデルデータのロード///////////////////////

	hModel_[Base] = Model::Load("Stage/Tutorial/first_Stage1.fbx");
	hModel_[Space] = Model::Load("Stage/SpaceModel/Space.fbx");
	hModel_[PolyModel] = Model::Load("Stage/Tutorial/first_Stage_Polygon.fbx");

	///////////////////////////各種設定/////////////////////////////

	//レイの判定にBaseをいれたいのでtrueにしておく
	Model::SetRayFlag(hModel_[Base], true);

	//Spaceモデルの明るさ設定
	Model::SetBrightness(hModel_[Space], 0.5f);

	//PolyModelを透明に設定(軸を確認するためだけに使うため)
	Model::SetAlpha(hModel_[PolyModel], ZERO);

	//画角
	Camera::SetFieldAngle(fieldAngle_);

	//カメラの上ベクトル
	Camera::SetUpDirection(UP_VECTOR);

	//ライトの強さ
	Light::SetPlayerIntensity((float)lightIntensity_);

	//Warpの移動先
	ARGUMENT_INITIALIZE(warpPos_,XMFLOAT3(16.7f,10.7f,-0.3f));

	//Playerの初期位置
	ARGUMENT_INITIALIZE(pos_,XMFLOAT3(20,300,ZERO));

	//3Dなのでtrueにする
	ARGUMENT_INITIALIZE(threeDflag_,true);

	//球体使用しているのでtrue
	ARGUMENT_INITIALIZE(circleFlag_,true);

}

//更新
void TutorialStage2::Update()
{
	tSpace_.rotate_.y += 0.1f;
}

//描画
void TutorialStage2::Draw()
{
	Model::SetTransform(hModel_[Base], transform_);
	Model::Draw(hModel_[Base]);

	Model::SetTransform(hModel_[Space], tSpace_);
	Model::Draw(hModel_[Space]);
}

//解放
void TutorialStage2::Release()
{
}

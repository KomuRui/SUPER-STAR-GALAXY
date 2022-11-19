#pragma once
#include "Block.h"

//透明ブロッククラス
class TransparentBlock : public Block
{

public:

	//コンストラクタ
	TransparentBlock(GameObject* parent, std::string modelPath, std::string name) :Block(parent, modelPath, name) {}

	//更新の前に一度だけ呼ばれる関数
	void BlockChildStartUpdate() override;
};
#include "PlayerState.h"

//状態変更
void PlayerState::ChangeState(PlayerState* nowState, PlayerState* state)
{
	//状態変更
	ARGUMENT_INITIALIZE(nowState, state);

	//状態変化したとき一回だけ呼ばれる関数を呼ぶ
	//nowState->Enter();
}

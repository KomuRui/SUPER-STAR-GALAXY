#include "State.h"

//状態変更
void State::ChangeState(State* nowState, State* state)
{
	//状態変更
	ARGUMENT_INITIALIZE(nowState, state);

	//状態変化したとき一回だけ呼ばれる関数を呼ぶ
	nowState->Enter();
}

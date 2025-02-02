#pragma once
#include "../../../Engine/GameObject/GameObject.h"

/// <summary>
/// Coinの音を管理する
/// </summary>
namespace CoinAudioManager
{

	/// <summary>
	/// 初期化
	///</summary>
	void Initialize();

	/// <summary>
	/// シーン遷移の時の初期化
	/// </summary>
	void SceneTransitionInitialize();

	/// <summary>
	/// 当たった時の音
	/// </summary>
	void HitAudio();

}
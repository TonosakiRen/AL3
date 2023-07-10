#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "memory"

#include "DebugCamera.h"
#include "Player.h"
#include "Skydome.h"
#include "Ground.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;
	//3Dモデル
	std::unique_ptr<Model> model_ = nullptr;
	//ヴュープロジェクション
	ViewProjection viewProjection_;

	// デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;
	bool isDebugCameraActive_;


	//自キャラ
	std::unique_ptr<Player> player_ = nullptr;

	//天空
	std::unique_ptr<Skydome> skydome_ = nullptr;
	//3DModel
	std::unique_ptr<Model> modelSkydome_ = nullptr;

	// 地
	std::unique_ptr<Ground> ground_ = nullptr;
	// 3DModel
	std::unique_ptr<Model> modelGround_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};

#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include "DebugCamera.h"
#include "Player.h"
#include "Enemy.h"
#include "CollisionManager.h"
#include "Skydome.h"
#include "RailCamera.h"
#include "EnemyBullet.h"

#include <sstream>

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
	/// 衝突判定と応答
	/// </summary>
	void CheckAllCollision();

	/// <summary>
	/// 敵弾を追加
	/// </summary>
	/// <param name="enemyBullet"></param>
	void AddEnemybullet(std::unique_ptr<EnemyBullet> enemyBullet);

	/// <summary>
	/// 敵発生データの読み込み
	/// </summary>
	void LoadEenemyPopData();

	/// <summary>
	/// 敵発生コマンドの更新
	/// </summary>
	void UpdateEnemyPopCommands();

	/// <summary>
	/// 敵発生関数
	/// </summary>
	void EnemySpawn(Vector3 position);

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
	Model* model_ = nullptr;
	//ヴュープロジェクション
	ViewProjection viewProjection_;

	//自キャラ
	std::unique_ptr<Player> player_ = nullptr;

	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;
	bool isDebugCameraActive_;

	//Enemy
	std::list<std::unique_ptr<Enemy>> enemies_;

	//CollsiionManager
	std::unique_ptr<CollisionManager> collisionManager_;

	//Skydome
	std::unique_ptr<Skydome> skydome_;
	//3Dモデル
	Model* modelSkydome_ = nullptr;
	//RailCamera
	std::unique_ptr<RailCamera> railCamera_;
	//enemyBullet
	std::list<std::unique_ptr<EnemyBullet>> enemyBullets_;
	//　敵発生コマンド
	std::stringstream enemyPopCommands;
	//待機フラグ
	bool waitFlag = false;
	//待機タイマー
	int32_t waitTimer = 0;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};

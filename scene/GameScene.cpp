#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "Mymath.h"

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete modelSkydome_;
	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("neko.jpg");
	// 3Dモデル生成
	model_ = Model::Create();
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
	//自キャラの設定
	player_ = std::make_unique<Player>();
	player_->Initialize(model_,textureHandle_);

	debugCamera_ =
	    new DebugCamera(dxCommon_->GetBackBufferWidth(), dxCommon_->GetBackBufferHeight());
	isDebugCameraActive_ = false;

	//軸方向表示の表示を有効にする	
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロダクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	// 敵を生成、初期化
	std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
	// 敵キャラに自キャラのアドレスを渡す
	newEnemy->SetPlayer(player_.get());
	newEnemy->Initialize(model_, {0, 5, 50});
	// 敵を登録する
	enemies_.push_back(std::move(newEnemy));

	//CollisionManagerを生成する
	collisionManager_ = std::make_unique<CollisionManager>();

	//skydomeを生成する
	skydome_ = std::make_unique<Skydome>();
	//3Dモデルの生成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	skydome_->Initialize(modelSkydome_);
}

void GameScene::Update() {
	//自キャラ更新
	player_->Update();
	//敵更新
	for (const std::unique_ptr<Enemy>& enemy : enemies_) {
		enemy->Update();
	}
	#ifdef _DEBUG
	if (input_->TriggerKey(DIK_0) && isDebugCameraActive_ == false) {
		isDebugCameraActive_ = true;
	} else
	if (input_->TriggerKey(DIK_0) && isDebugCameraActive_ == true) {
		isDebugCameraActive_ = false;
	}
	//当たり判定
	CheckAllCollision();
	#endif
	//カメラの処理
	if (isDebugCameraActive_) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	} else {
		viewProjection_.UpdateMatrix();
	}

	
}

void GameScene::CheckAllCollision() {

	//collisionManagerのcolliderをクリア
	collisionManager_->ClearColliders();

	//コライダー
	std::vector<Collider*> colliders_;
	//コライダーをリストに登録
	colliders_.push_back(player_.get());
	for (auto& enemy : enemies_) {
		colliders_.push_back(enemy.get());	
	}
	// 自弾すべてについて
	for (auto& playerBullet : player_->GetBullets()) {
		colliders_.push_back(playerBullet.get());	
	}
	for (auto& enemy : enemies_) {
		// 敵弾すべてについて
		for (auto& enemyBullet : enemy->GetBullets()) {
			colliders_.push_back(enemyBullet.get());
		}
	}

	collisionManager_->SetColliders(colliders_);
	collisionManager_->CheckAllCollision();

}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	// 3Dモデル描画

	skydome_->Draw(viewProjection_);

	player_->Draw(viewProjection_);
	// 敵描画
	for (const std::unique_ptr<Enemy>& enemy : enemies_) {
		enemy->Draw(viewProjection_);
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

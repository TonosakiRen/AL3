#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "Mymath.h"

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete debugCamera_;
	delete player_;
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
	player_ = new Player();
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
	newEnemy->SetPlayer(player_);
	newEnemy->Initialize(model_, {0, 5, 50});
	// 敵を登録する
	enemies_.push_back(std::move(newEnemy));
	
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
	//判定対象AとBの座標
	Vector3 posA, posB;

	//自弾リストの取得
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullets();
	// 自キャラの座標
	posA = player_->GetWorldPosition();
#pragma region 自キャラと敵弾の当たり判定
	for (auto& enemy : enemies_) {
		// 敵弾リストの取得
		const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy->GetBullets();
		//自キャラと敵弾すべての当たり判定
		for (auto& enemyBullet : enemyBullets) {
		  //敵弾の座標
		  posB = enemyBullet->GetWorldPosition();
		  float distance = Length(posA - posB);
		  if (distance <= enemyBullet->GetRadius() + player_->GetRadius()) {
			//自キャラの衝突時コールバックを呼び出す
			  player_->OnCollision();
			 //敵弾の衝突時コールバックを呼び出す	
			  enemyBullet->OnCollision();
		  }
		}	
	}
#pragma endregion
	//自キャラの弾取得
	for (auto& playerBullet : playerBullets) {
		//敵キャラ取得
		for (auto& enemy : enemies_) {
		  posA = enemy->GetWorldPosition();
		  posB = playerBullet->GetWorldPosition();
		  float distance = Length(posA - posB);
		  //自弾と敵の当たり判定
		  if (distance <= enemy->GetRadius() + playerBullet->GetRadius()) {
			//自弾のコールバックを呼び出す	
			  enemy->OnCollision();
 			  playerBullet->OnCollision();
		  }
		}
	}

	//自キャラの弾取得
	for (auto& playerBullet : playerBullets) {
		for (auto& enemy : enemies_) {
		  // 敵弾リストの取得
		  const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy->GetBullets();
		  for (auto& enemyBullet : enemyBullets) {
			  posA = enemyBullet->GetWorldPosition();
			  posB = playerBullet->GetWorldPosition();
			  float distance = Length(posA - posB);
			  //自弾と敵弾の当たり判定
			  if (distance <= enemyBullet->GetRadius() + playerBullet->GetRadius()) {
				  // 自弾のコールバックを呼び出す
				  enemyBullet->OnCollision();
				  playerBullet->OnCollision();
			  }
		  }
		}
	}

	
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

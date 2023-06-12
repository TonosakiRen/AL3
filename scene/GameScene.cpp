#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "Mymath.h"

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
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
	//コライダー
	std::list<Collider*> colliders_;
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

	//リスト内のペアを総当たり
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {
		Collider* colliderA = *itrA;

		//イテレータBはイテレータAの次の要素から回す(重複判定を回避)
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;
		
		for (; itrB != colliders_.end(); ++itrB) {
			Collider* colliderB = *itrB;
			// 衝突フィルタリング
			if ((colliderA->GetCollsionAttribute() & colliderB->GetCollisionMask()) == 0 || 
				(colliderB->GetCollsionAttribute() & colliderA->GetCollisionMask()) == 0) {
				continue;
			}
			//ペアの当たり判定
			CheckCollisionPair(colliderA, colliderB);
		}
	}

}

void GameScene::CheckCollisionPair(Collider* colliderA, Collider* colliderB) {
	Vector3 posA = colliderA->GetWorldPosition();
	Vector3 posB = colliderB->GetWorldPosition();
	float distance = Length(posA - posB);
	//球と球の交差判定
	if (distance <= colliderA->GetRadius() + colliderB->GetRadius()) {
		//コライダーAの衝突時コールバックを呼び出す
		colliderA->OnCollision();
		//コライダーBの衝突時コールバックを呼び出す
		colliderB->OnCollision();
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

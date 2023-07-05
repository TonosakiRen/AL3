#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "Mymath.h"
#include "PrimitiveDrawer.h"
#include "ImGuiManager.h"
#include <fstream>

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
	// れてぃくるのテクスチャ
	TextureManager::Load("reticle.png");
	// 3Dモデル生成
	model_ = Model::Create();
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	debugCamera_ =
	    new DebugCamera(dxCommon_->GetBackBufferWidth(), dxCommon_->GetBackBufferHeight());
	isDebugCameraActive_ = false;

	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロダクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	//ライン描画が参照するビュープロジェクションを指定する
	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection_);
	// railCamera
	railCamera_ = std::make_unique<RailCamera>();
	railCamera_->Initialize(viewProjection_.translation_, viewProjection_.rotation_);
	//自キャラの設定
	player_ = std::make_unique<Player>();
	Vector3 playerPosition(0.0f, 0.0f, 30.0f);
	player_->Initialize(model_, textureHandle_, playerPosition);
	//自キャラとレールカメラの親子関係を結ぶ
	player_->SetParent(&railCamera_->GetWorldTransform());
	//敵生成csvをロード
	LoadEenemyPopData();
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
	player_->Update(viewProjection_);

	//敵生成
	UpdateEnemyPopCommands();
	
	for (const std::unique_ptr<Enemy>& enemy : enemies_) {

		enemy->SetGameScene(this);
	}
	//敵更新
	for (const std::unique_ptr<Enemy>& enemy : enemies_) {
		enemy->Update();
	}
	// デスフラグの立った敵を削除
	enemies_.remove_if([](std::unique_ptr<Enemy>& enemy) {
		if (enemy->IsDead()) {
			return true;
		}
		return false;
	});
	// デスフラグの立った敵弾を削除
	enemyBullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		if (bullet->IsDead()) {
			return true;
		}
		return false;
	});
	// 敵弾更新
	for (const std::unique_ptr<EnemyBullet>& enemyBullet : enemyBullets_) {
		enemyBullet->Update();
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
		railCamera_->Update();
		viewProjection_.matView = railCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
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
	// 敵弾すべてについて
	for (auto& enemyBullet : enemyBullets_) {
		colliders_.push_back(enemyBullet.get());
	}
	

	collisionManager_->SetColliders(colliders_);
	collisionManager_->CheckAllCollision();

}

void GameScene::AddEnemybullet(std::unique_ptr<EnemyBullet> enemyBullet) {
	// リストに追加する
	enemyBullets_.emplace_back(std::move(enemyBullet));
}

void GameScene::LoadEenemyPopData() {

	//fileを開く
	std::ifstream file;
	file.open("./Resources/enemyPop.csv");
	assert(file.is_open());

	//ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	//ファイルを閉じる
	file.close();
}

void GameScene::UpdateEnemyPopCommands() {
	//待機処理
	if (waitFlag == true) {

		waitTimer--;
	
		if (waitTimer <= 0) {
			//待機官僚	
			waitFlag = false;
		}
		return;
	}


	//１行分の文字列を入れる変数
	std::string line;

	//コマンド実行ループ
	while (getline(enemyPopCommands, line)) {
	// １行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);
		std::string word;
		//,区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');
		//"//"から始まる行はコメント
		if (word.find("//") == 0) {
			continue;
		}

		//POPコマンド
		if (word.find("POP") == 0) {
			//x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			//y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			// y座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			//敵を発生させる
			EnemySpawn(Vector3(x,y,z));
		} else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			//待ち時間
			int32_t waitTime = atoi(word.c_str());
			//待機開始
			waitFlag = true;
			waitTimer = waitTime;

			//コマンドループを抜ける
			break;
		}
	}
}

void GameScene::EnemySpawn(Vector3 position) {
	// 敵を生成、初期化
	std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
	// 敵キャラに自キャラのアドレスを渡す
	newEnemy->SetPlayer(player_.get());
	newEnemy->SetGameScene(this);
	newEnemy->Initialize(model_, position);
	// 敵を登録する
	enemies_.push_back(std::move(newEnemy));
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
	//敵弾描画
	for (const std::unique_ptr<EnemyBullet>& enemyBullet : enemyBullets_) {
		enemyBullet->Draw(viewProjection_);
	}
	railCamera_->Draw();

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	player_->DrawUI();

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("neko.jpg");
	// 3Dモデル生成
	model_.reset(Model::Create());
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
	//デバッグカメラ
	debugCamera_ = new DebugCamera(dxCommon_->GetBackBufferWidth(), dxCommon_->GetBackBufferHeight());
	isDebugCameraActive_ = false;
	// 追従カメラ
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();
	//自キャラの設定
	player_ = std::make_unique<Player>();

	modelPlayer_.resize(static_cast<int>(Player::Parts::PartsNum));

	modelPlayer_[static_cast<int>(Player::Parts::Body)].reset(Model::CreateFromOBJ("player_body", true));
	modelPlayer_[static_cast<int>(Player::Parts::Head)].reset(Model::CreateFromOBJ("player_head", true));
	modelPlayer_[static_cast<int>(Player::Parts::L_arm)].reset(Model::CreateFromOBJ("player_left_hand", true));
	modelPlayer_[static_cast<int>(Player::Parts::R_arm)].reset(Model::CreateFromOBJ("player_right_hand", true));
	modelPlayer_[static_cast<int>(Player::Parts::Hammer)].reset( Model::CreateFromOBJ("hammer", true));
	player_->Initialize(modelPlayer_);
	player_->SetViewProjection(&followCamera_->GetViewProjection());
	// 自キャラのワールドトランスフォームを追従カメラにセット
	followCamera_->SetTarget(&player_->GetWorldTransform());
	//skydomeを生成
	skydome_ = std::make_unique<Skydome>();
	//3Dモデルの生成
	modelSkydome_.reset(Model::CreateFromOBJ("skydome", true));
	skydome_->Initialize(modelSkydome_.get());

	// groundを生成
	ground_ = std::make_unique<Ground>();
	// 3Dモデルの生成
	modelGround_.reset(Model::CreateFromOBJ("ground", true));
	ground_->Initialize(modelGround_.get());

	//敵の生成
	enemy_ = std::make_unique<Enemy>();
	modelEnemy_.resize(static_cast<int>(Enemy::Parts::PartsNum));
	modelEnemy_[static_cast<int>(Enemy::Parts::Body)].reset(Model::CreateFromOBJ("enemy", true));
	enemy_->Initialize(modelEnemy_);
	
}

void GameScene::Update() {
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_0) && isDebugCameraActive_ == false) {
		isDebugCameraActive_ = true;
	} else if (input_->TriggerKey(DIK_0) && isDebugCameraActive_ == true) {
		isDebugCameraActive_ = false;
	}
#endif
	
	//自キャラ更新
	player_->Update();
	//敵キャラ更新
	enemy_->Update();
	// カメラの処理
	if (isDebugCameraActive_) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	} else {
		// 追従カメラの更新
		followCamera_->Update();
		viewProjection_.matView = followCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = followCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
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


	skydome_->Draw(viewProjection_);
	ground_->Draw(viewProjection_);
	player_->Draw(viewProjection_);
	enemy_->Draw(viewProjection_);

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

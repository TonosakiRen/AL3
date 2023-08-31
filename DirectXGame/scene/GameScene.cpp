#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "ImGuiManager.h"

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
	//自キャラの設定
	player_ = std::make_unique<Player>();

	modelPlayer_.resize(static_cast<int>(Player::Parts::PartsNum));

	modelPlayer_[static_cast<int>(Player::Parts::Body)].reset(Model::CreateFromOBJ("player", true));
	player_->Initialize(modelPlayer_);
	// 追従カメラ
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize(player_.get());
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
	modelEnemy_[static_cast<int>(Enemy::Parts::Right)].reset(Model::CreateFromOBJ("enemy", true));
	modelEnemy_[static_cast<int>(Enemy::Parts::Left)].reset(Model::CreateFromOBJ("enemy", true));
	enemy_->Initialize(modelEnemy_);
	enemy_->SetPlayer(player_.get());

	//title
	uint32_t textureTitle = TextureManager::Load("BoxBattle.png");
	title_.reset(Sprite::Create(
	    textureTitle, {WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f},
	    {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));
	//title_->SetSize({64.0f, 64.0f});

	// push
	uint32_t texturePush = TextureManager::Load("push.png");
	push_.reset(Sprite::Create(
	    texturePush, {WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f}, {1.0f, 1.0f, 1.0f, 1.0f},
	    {0.5f, 0.5f}));
	// push
	uint32_t textureClear = TextureManager::Load("GameClear.png");
	gameClear_.reset(Sprite::Create(
	    textureClear, {WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f},
	    {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));
	// push
	uint32_t textureOver = TextureManager::Load("GameOver.png");
	gameOver_.reset(Sprite::Create(
	    textureOver, {WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f},
	    {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));
	//push_->SetSize({64.0f, 64.0f});
	uint32_t textureHpBar = TextureManager::Load("hpBar.png");
	hpBar_.reset(Sprite::Create(
	    textureHpBar, {WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f},
	    {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));
	
	uint32_t textureFrame = TextureManager::Load("frame.png");
	frame_.reset(Sprite::Create(
	    textureFrame, {WinApp::kWindowWidth / 2.0f, WinApp::kWindowHeight / 2.0f},
	    {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));

	uint32_t textureHeart = TextureManager::Load("heart.png");

	for (int i = 0; i < 6; i++) {
		heart_[i].reset(Sprite::Create(
		    textureHeart, {20.0f + (64.0f * i) + (5.0f * i), WinApp::kWindowHeight - 64.0f - 20.0f },
		    {1.0f, 1.0f, 1.0f, 1.0f}));
	}
}

void GameScene::Update() {
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_0) && isDebugCameraActive_ == false) {
		isDebugCameraActive_ = true;
	} else if (input_->TriggerKey(DIK_0) && isDebugCameraActive_ == true) {
		isDebugCameraActive_ = false;
	}
#endif
	ACooltime--;
	// ゲームパッドの状態
	XINPUT_STATE joyState;
	Input::GetInstance()->GetJoystickState(0, joyState);
	switch (scene) {
	case GameScene::title:
		if ((joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) && 0 > ACooltime) {
				ACooltime = 15;
				player_->Initialize2();
				enemy_->Initialize2();
				scene = inGame;
			}
		break;
	case GameScene::inGame:
		// 自キャラ更新
		player_->Update();
		// 敵キャラ更新
		enemy_->Update();

		player_->SetFocus(enemy_->GetWorldTransform());
		Collision();

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

		if (player_->GetisDead()) {
			if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A && 0 > ACooltime) {
				ACooltime = 15;
				scene = title;
				break;
			}
		}

		if (enemy_->GetisDead()) {
			if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A && 0 > ACooltime) {
				ACooltime = 15;
				scene = title;
				break;
			}
		}

		break;
	default:
		break;
	}
	
}

void GameScene::Collision() { 
	player_->GetWorldTransform();
	enemy_->GetBullets()[0].worldTransform;
	enemy_->GetWorldTransform();
	bool playerHit = false;
	bool enemyHit = false;
	//プレイヤー当たり判定
	if (playerHit == false) {
		playerHit = IsOBBCollision(
		    player_->GetWorldTransform(),
		    enemy_->GetModelWorldTransform()[static_cast<int>(Enemy::Parts::Body)]);
		if (playerHit == true) {
			player_->SetIsHit(
			    playerHit, enemy_->GetModelWorldTransform()[static_cast<int>(Enemy::Parts::Body)].translation_);
		}
	}
	if (playerHit == false) {
		playerHit = IsOBBCollision(
		    player_->GetWorldTransform(),
		    enemy_->GetModelWorldTransform()[static_cast<int>(Enemy::Parts::Right)]);
		if (playerHit == true) {
			player_->SetIsHit(
			    playerHit, enemy_->GetModelWorldTransform()[static_cast<int>(Enemy::Parts::Right)].translation_);
		}
	}
	//敵弾当たり判定
	if (playerHit == false) {
		for (int i = 0; i < enemy_->bulletNum; i++) {
			if (enemy_->GetBullets()[i].isActive == true) {
				playerHit = IsOBBCollision(
				    player_->GetWorldTransform(), enemy_->GetBullets()[i].worldTransform);
				if (playerHit == true) {
					player_->SetIsHit(
					    playerHit, enemy_->GetBullets()[i].worldTransform.translation_);
					break;
				}
			}
		}
	}
	if (enemyHit == false) {
		for (int i = 0; i < player_->bulletNum; i++) {
			if (player_->GetBullets()[i].isActive == true) {
				enemyHit = IsOBBCollision(
				    enemy_->GetModelWorldTransform()[static_cast<int>(Enemy::Parts::Body)],
				    player_->GetBullets()[i].worldTransform);
				if (enemyHit == true) {
					enemy_->SetIsHit(
					    enemyHit);
					player_->GetBullets()[i].isActive = false;
					break;
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
	switch (scene) {
	case GameScene::title:
		break;
	case GameScene::inGame:
		break;
	default:
		break;
	}
	

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

	switch (scene) {
	case GameScene::title:
		break;
	case GameScene::inGame:
		skydome_->Draw(viewProjection_);
		ground_->Draw(viewProjection_);
		player_->Draw(viewProjection_);
		enemy_->Draw(viewProjection_);
		break;
	default:
		break;
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
	switch (scene) {
	case GameScene::title:
		title_->Draw();
		push_->Draw();
		enemy_->Initialize2();
		player_->Initialize2();
		break;
	case GameScene::inGame:
		player_->DrawUI();
		for (int i = 0; i < player_->GetHp(); i++) {
			heart_[i]->Draw();
		}
		hpBar_->SetSize({1280.0f * (enemy_->GetHp() / 300.0f),720.0f});
		hpBar_->Draw();
		frame_->Draw();
		

		if (player_->GetisDead()) {
			gameOver_->Draw();
			push_->Draw();
		}

		if (enemy_->GetisDead()) {
			gameClear_->Draw();
			push_->Draw();
		}
		break;
	default:
		break;
	}

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

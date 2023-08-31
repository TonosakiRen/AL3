#pragma once
#include "BaseCharacter.h"
#include "Model.h"
#include "WorldTransform.h"
#include <cstdint>
#include "ViewProjection.h"
#include "Input.h"
#include <vector>
#include <optional>
#include "Sprite.h"
#include "Audio.h"
/// <summary>
/// 自キャラ
/// </summary>
class Player : public BaseCharacter {
public:

	////調整項目の適用
	//void ApplyGlobalVariables();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	void Initialize(const std::vector<std::unique_ptr<Model>>& models) override;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	void Initialize2() ;

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	
	

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
	void Draw(const ViewProjection& viewProjection) override;

	void DrawUI();

	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}

	enum class Parts { Body,PartsNum };

	Vector3 GetDirection() { return playerDirection; }

	WorldTransform* GetFocus() { return focus_; }
	void SetFocus(WorldTransform* focus) { focus_ = focus; }
	bool GetIsFocus() { return isFocus; }
	Vector3 GetMove() { return move; }

	struct Bullet {
		bool isActive;
		WorldTransform worldTransform;
		Vector3 direction;
		Vector3 startPosition;
	};

	Bullet* GetBullets() { return bullet; }

	void SetIsHit(bool ishit, Vector3 hittranslation) {
		isHit = ishit;
		if (isHit == true && hitCoolTime <= 0) {
			audio->PlayWave(hitSoundHandle_,false, 0.2f);
			hitTranslation = hittranslation;
			hitCoolTime = 60;
			hp--;
		}
	}

	bool GetisDead() { return isDead; }
	int GetHp() { return hp; }
	static const int bulletNum = 200;

private:

	
	Audio* audio;
	// ワールド変換データ
	WorldTransform modelWorldTransform_[static_cast<int>(Player::Parts::PartsNum)];

	//カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;
	
	Vector3 playerDirection = {0.0f, 0.0f, 0.0f};

	Vector3 move = {0.0f, 0.0f, 0.0f};


	enum class Behavior {
		kRoot,
		kAttack
	};
	Behavior behavior_ = Behavior::kRoot;

	//次の振る舞いリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	//焦点相手
	WorldTransform* focus_ = nullptr; 

	bool isFocus = false;
	bool preRightBotton = false;
	
	Bullet bullet[bulletNum];
	int bulletCooltime = 0;
	const float bulletSpeed = 1.5f;
	float bulletRange = 30.0f;

	int hitCoolTime = 0;
	bool isHit = false;
	Vector3 hitTranslation = {0.0f, 0.0f, 0.0f};
	int hp = 5;
	bool isDead = false;

	uint32_t hitSoundHandle_ = 0;
	uint32_t shotSoundHandle_ = 0;


 private:
	std::unique_ptr<Sprite> sprite2DReticle_ = nullptr;
	
};
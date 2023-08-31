#pragma once
#include "BaseCharacter.h"
#include "Model.h"
#include "WorldTransform.h"
#include <cstdint>
#include "ViewProjection.h"
#include <vector>
#include "Mymath.h"
#include "Audio.h"

class Player;
class Enemy : public BaseCharacter {
public:
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

	enum class Parts { Body, Right, Left, PartsNum };

	WorldTransform* GetWorldTransform() { return &worldTransform_; }

	WorldTransform* GetModelWorldTransform() { return modelWorldTransform_; }

	void SetPlayer(Player* player) { player_ = player; }

	struct Bullet {
		bool isActive;
		WorldTransform worldTransform;
		Vector3 direction;
		int lifeSpan = 0;
	};

	Bullet* GetBullets() { return bullet; }

	void SetIsHit(bool ishit) { isHit = ishit; }

	bool GetisDead() { return isDead; }

	int GetHp() { return hp; }

	static const int bulletNum = 200;

private:
	// ワールド変換データ
	WorldTransform modelWorldTransform_[static_cast<int>(Enemy::Parts::PartsNum)];

	// カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;
	// 浮遊ギミックの媒介変数
	float moveRotate_ = 0.0f;
	// プレイヤー
	Player* player_ = nullptr;
	// enemy
	enum Stats {
		normal,
		attack,
		assault,
		bulletAttack,
	};
	Stats stats_ = normal;
	Stats preStats_ = normal;

	//行動
	bool isRandam = false;
	int randam = 0;

	// moveCoolTime
	bool isRotated = false;
	bool isRotate = false; 
	Vector3 enemyToPlayer;
	Vector3 goalRotation; 
	bool isDecidePositive = false;
	bool isPositive = false;
	int moveTime = 0;

	// Attack

	float attackT = 0.0f;
	bool isSavePos = false;
	Vector3 savePos = {0.0f, 0.0f, 0.0f};
	const float attackSpeed = 0.02f;
	Vector3 goalPos = {0.0f,0.0f,0.0f};
	int attackCooltime = 60;

	//assault
	float degree = 0.0f;
	float degreeSpeed = 0.0f;
	bool isAssault = false;
	int assaultTime = 0;
	bool isEndAssult = false;

	Bullet bullet[bulletNum];
	int bulletCooltime = 0;
	const float bulletSpeed = 0.5f;
	int bulletShotNum = 0;
	int bulletShotAllNum = 0;
	int bulletLastNum = 0;

	//
	bool isHit = false;
	int hp = 300;
	bool isDead = false;

	float length;

	Audio* audio;
	uint32_t hitSoundHandle_ = 0;
};

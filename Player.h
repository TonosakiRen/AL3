#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <cstdint>
#include "ViewProjection.h"
#include <Input.h>
#include "PlayerBullet.h"
#include <list>
#include "Collider.h"
#include "Sprite.h"
/// <summary>
/// 自キャラ
/// </summary>
class Player : public Collider{
public:


	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Initialize(Model* model, uint32_t textureHandle, Vector3 playerPosition);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(const ViewProjection& viewProjection);

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

	//衝突したら呼び出されるコールバック関数
	void OnCollision() override;

	//弾リストを取得
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
	void Draw(ViewProjection& viewProjection);

	/// <summary>
	/// UI描画
	/// </summary>
	void DrawUI();

	//ワールド座標を取得
	Vector3 GetWorldPosition() override{
		return {
		    worldTransform_.matWorld_.m[3][0], worldTransform_.matWorld_.m[3][1],
		    worldTransform_.matWorld_.m[3][2]};
    };
	

	void SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; }

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//キーボード入力
	Input* input_ = nullptr;
	// 弾
	std::list < std::unique_ptr<PlayerBullet>> bullets_;
	//当たり判定
	const float radius_ = 1.0f;
	//3Dレティクル用ワールドトランスフォーム
	WorldTransform worldTransform3DReticle_;
	//2Dレティクル用スプライト
	Sprite* sprite2DReticle_ = nullptr; 

};
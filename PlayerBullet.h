#pragma once
#include"Model.h"
class PlayerBullet {
public:
	PlayerBullet();
	~PlayerBullet();
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="position">�������W</param>
	void Initialize(Model* model, const Vector3& position);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	void Draw(const ViewProjection&viewProjection);

private:
};


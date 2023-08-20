#pragma once
#include <vector>
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
class BaseCharacter {
protected:
	//���f���f�[�^�z��
	std::vector<Model*> models_;
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;

public:
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="models"></param>
	virtual void Initialize(const std::vector<Model*>&models);
	/// <summary>
	/// �X�V����
	/// </summary>
	virtual void Update();
	/// <summary>
	/// �`�揈��
	/// </summary>
	/// <param name="viewProjection"></param>
	virtual void Draw(const ViewProjection& viewProjection);
	/// <summary>
	/// ���[���h�ϊ��f�[�^���擾
	/// </summary>
	/// <returns></returns>
	const WorldTransform& GetWorldTransform() { return worldTransform_; }

private:

};

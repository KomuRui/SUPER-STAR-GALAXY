#include "Transform.h"



Transform::Transform(): pParent_(nullptr)
{
	position_ = XMFLOAT3(0, 0, 0);
	rotate_ = XMFLOAT3(0, 0, 0);
	scale_ = XMFLOAT3(1, 1, 1);
	matTranslate_ = XMMatrixIdentity();
	matRotate_ = XMMatrixIdentity();
	matScale_ = XMMatrixIdentity();
	mRotate_  = XMMatrixIdentity();
	mmRotate_ = XMMatrixIdentity();
	q1_       = quaternion();
	q2_       = quaternion();
	mFlag_ = false;
}


Transform::~Transform()
{
}

void Transform::Calclation()
{

	if(!mFlag_)
	{
		//�ړ��s��
		matTranslate_ = XMMatrixTranslation(position_.x, position_.y, position_.z);

		//��]�s��
		XMMATRIX rotateX, rotateY, rotateZ;
		rotateX = XMMatrixRotationX(XMConvertToRadians(rotate_.x));
		rotateY = XMMatrixRotationY(XMConvertToRadians(rotate_.y));
		rotateZ = XMMatrixRotationZ(XMConvertToRadians(rotate_.z));
		matRotate_ = rotateZ * rotateX * rotateY;

		//�g��k��
		matScale_ = XMMatrixScaling(scale_.x, scale_.y, scale_.z);
	}
	else
	{
		//�ړ��s��
		matTranslate_ = XMMatrixTranslation(position_.x, position_.y, position_.z);

		//�g��k��
		matScale_ = XMMatrixScaling(scale_.x, scale_.y, scale_.z);

		//q1_ = concatenate(q1_, q2_);
	}

	
}

XMMATRIX Transform::GetWorldMatrix() 
{
	if (!mFlag_)
	{
		Calclation();
		if (pParent_)
		{
			return  matScale_ * matRotate_ * matTranslate_ * pParent_->GetWorldMatrix();
		}

		return  matScale_ * matRotate_ * matTranslate_;
	}
	else
	{

		Calclation();

		XMMATRIX m = QuaternionToMattrix(q1_);

		XMMATRIX m2 = QuaternionToMattrix(q2_);

		if (pParent_)
		{
			return  matScale_ /* * mRotate_ */* mmRotate_ * matTranslate_ * pParent_->GetWorldMatrix();
		}
	
		return matScale_ * mRotate_ * mmRotate_ * matTranslate_;
	}
		
}

XMMATRIX Transform::QuaternionToMattrix(quaternion q)
{
	XMMATRIX m = {
		1 - (2 * float(pow((q.y),2.0))) - (2 * float(pow((q.z),2.0))),(2 * q.x * q.y) + (2 * q.w * q.z),(2 * q.x * q.z) - (2 * q.w * q.y),0,
		(2 * q.x * q.y) - (2 * q.w * q.z),1 - (2 * float(pow((q.x),2.0))) - (2 * float(pow((q.z),2.0))),(2 * q.y * q.z) + (2 * q.w * q.x),0,
		(2 * q.x * q.z) + (2 * q.w * q.y),(2 * q.y * q.z) - (2 * q.w * q.x),1 - (2 * float(pow((q.x),2.0))) - (2 * float(pow((q.y),2.0))),0,
		0,0,0,1
	};

	return m;
}


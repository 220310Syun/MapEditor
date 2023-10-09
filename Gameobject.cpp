#include "GameObject.h"

GameObject::GameObject()
	:pParent_(nullptr)
{
}

GameObject::GameObject(GameObject* parent, const std::string& name)
	:pParent_(parent), objectName_(name), isDead_(false)
{
	if (parent)
		this->transform_.pParent_ = &(parent)->transform_;
}

GameObject::~GameObject()
{
}

void GameObject::DrawSub()
{
	Draw();
	for (auto itr = childList_.begin(); itr != childList_.end(); itr++)
	{
		(*itr)->DrawSub();
	}
}

void GameObject::UpdateSub()
{
	Update();
	for (auto itr = childList_.begin(); itr != childList_.end(); itr++)
	{
		(*itr)->UpdateSub();
	}

	for (auto itr = childList_.begin(); itr != childList_.end();)
	{
		if ((*itr)->isDead_) {
			ReleaseSub();
			delete* itr;
			itr = childList_.erase(itr);
		}
		else
		{
			itr++;
		}
	}
}

void GameObject::ReleaseSub()
{
	for (auto itr = childList_.begin(); itr != childList_.end(); itr++)
	{
		(*itr)->ReleaseSub();
	}
	Release();
}

void GameObject::KillMe()
{
	isDead_ = true;
}

void GameObject::SetTransform(Transform _transform)
{
	this->transform_ = _transform;
}

void GameObject::SetPosition(XMFLOAT3 _position)
{
	this->transform_.position_ = _position;
}

void GameObject::SetPosition(float _x, float _y, float _z)
{
	SetPosition(XMFLOAT3(_x, _y, _z));
}

void GameObject::SetRotate(XMFLOAT3 _rotate)
{
	this->transform_.rotate_ = _rotate;
}

void GameObject::SetRotate(float _x, float _y, float _z)
{
	SetRotate(XMFLOAT3(_x, _y, _z));
}

void GameObject::SetScale(XMFLOAT3 _scale)
{
	this->transform_.scale_ = _scale;
}

void GameObject::SetScale(float _x, float _y, float _z)
{
	SetScale(XMFLOAT3(_x, _y, _z));
}

void GameObject::MirrorPosition(GameObject* _object)
{
	SetPosition(-_object->GetPosition().x, -_object->GetPosition().y, -_object->GetPosition().z);
}

GameObject* GameObject::FindChildObject(string _objName)
{
	if (_objName == this->objectName_)
	{
		return (this);
	}
	else
	{
		//for (auto itr=childList_.begin();itr != childList_.end(); itr++)
		for (auto itr : childList_)
		{
			GameObject* obj = itr->FindChildObject(_objName);
			if (obj != nullptr)
				return obj;
		}
	}
	return nullptr;
}

GameObject* GameObject::GetRootJob()
{
	if (pParent_ == nullptr)
		return this;

	return pParent_->GetRootJob();
}

GameObject* GameObject::FindObject(string _objName)
{
	return GetRootJob()->FindChildObject(_objName);
}

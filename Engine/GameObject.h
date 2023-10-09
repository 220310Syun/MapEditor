#pragma once

#include <list>
#include <string>
#include "Transform.h"

using std::string;
using std::list;
class GameObject
{
	bool isDead_;
protected:
	list<GameObject *>	childList_;
	Transform			transform_;
	GameObject*			pParent_;
	string				objectName_;

public:
	GameObject();
	GameObject(GameObject* parent, const std::string& name);
	~GameObject();

	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Release() = 0;
	
	void DrawSub();
	void UpdateSub();
	void ReleaseSub();

	void KillMe();	

	//位置・角度・大きさを設定する
	void SetTransform(Transform _transform);
	void SetPosition(XMFLOAT3 _position);
	void SetPosition(float _x, float _y, float _z);
	void SetRotate(XMFLOAT3 _rotate);
	void SetRotate(float _x,float _y,float _z);
	void SetScale(XMFLOAT3 _scale);
	void SetScale(float _x, float _y, float _z);

	void MirrorPosition(GameObject* _object);

	GameObject* FindChildObject(string _objName);
	GameObject* GetRootJob();
	GameObject* FindObject(string _objName);

	//位置・角度・大きさを取得する
	Transform GetTransform() { return this->transform_; }
	XMFLOAT3  GetPosition() { return this->transform_.position_; }
	XMFLOAT3  GetRotate() { return this->transform_.rotate_; }
	XMFLOAT3  GetScale() { return this->transform_.scale_; }

	//コライダー用の関数
	//void AddCollider(SphereCollider* _pCollider);

	template <class T>
	T* Instantiate(GameObject* parent)
	{
		T* pObject;
		pObject = new T(parent);
		pObject->Initialize();
		childList_.push_back(pObject);
		return pObject;
	}
};
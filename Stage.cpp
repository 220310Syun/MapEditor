#include "Stage.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Engine/Camera.h"
#include "Engine/Fbx.h"
#include "resource.h"
HWND hDlg;

void Stage::SetBlock(int _x, int _z, BLOCKTYPE _type)
{
	table_[_x][_z].type = _type;
}

void Stage::SetBlockHeight(int _x, int _z, int _height)
{
	table_[_x][_z].height = _height;
}

//コンストラクタ
Stage::Stage(GameObject* parent)
	:GameObject(parent, "Stage")
{
	for (int i = 0; i < MODEL_NUM; i++) {
		hModel_[i] = -1;
	}
	for (int z = 0; z < ZSIZE; z++) {
		for (int x = 0; x < XSIZE; x++) {
			SetBlock(x, z, DEFAULT);
		}
	}
}

//デストラクタ
Stage::~Stage()
{
}

//初期化
void Stage::Initialize()
{
	string modelname[] = {
		"BoxDefault.fbx",
		"BoxBrick.fbx",
		"BoxGrass.fbx",
		"BoxSand.fbx",
		"BoxWater.fbx"
	};
	string fname_base = "assets/";
	//モデルデータのロード
	for (int i = 0; i < MODEL_NUM; i++) {
		hModel_[i] = Model::Load(fname_base + modelname[i]);
		assert(hModel_[i] >= 0);
	}
	//tableにブロックのタイプをセットしてやろう！
	for (int z = 0; z < ZSIZE; z++) {
		for (int x = 0; x < XSIZE; x++) {
			SetBlock(x, z, (BLOCKTYPE)(0));
			SetBlockHeight(x, z, 0);
		}
	}

}


//更新
void Stage::Update()
{
	
	if (!Input::IsMouseButtonDown(0)) {
		return;
	}
	float w = (float)(Direct3D::scrWidth / 2.0f);
	float h = (float)(Direct3D::scrHeight / 2.0f);
	//Offsetx,y は0
	//minZ =0 maxZ = 1

	XMMATRIX vp =
	{
		 w,  0,  0, 0,
		 0, -h,  0, 0,
		 0,  0,  1, 0,
		 w,  h,  0, 1
	};

	//ビューポート
	XMMATRIX invVP = XMMatrixInverse(nullptr, vp);
	//プロジェクション変換
	XMMATRIX invProj = XMMatrixInverse(nullptr, Camera::GetProjectionMatrix());
	//ビュー変換
	XMMATRIX invView = XMMatrixInverse(nullptr, Camera::GetViewMatrix());
	XMFLOAT3 mousePosFront = Input::GetMousePosition();
	mousePosFront.z = 0.0f;
	XMFLOAT3 mousePosBack = Input::GetMousePosition();
	mousePosBack.z = 1.0f;
	//�@　mousePosFrontをベクトルに変換
	XMVECTOR vMouseFront = XMLoadFloat3(&mousePosFront);
	//�A　�@にinvVP、invPrj、invViewをかける
	vMouseFront = XMVector3TransformCoord(vMouseFront, invVP * invProj * invView);
	//�B　mousePosBackをベクトルに変換
	XMVECTOR vMouseBack = XMLoadFloat3(&mousePosBack);
	//�C　�BにinvVP、invPrj、invViewをかける
	vMouseBack = XMVector3TransformCoord(vMouseBack, invVP * invProj * invView);

	

	
		



	for (int x = 0; x < 15; x++)
	{
		for (int z = 0; z < 15; z++)
		{
			for (int y = 0; y < table_[x][z].height + 1; y++)
			{
				//�D　�Aから�Cに向かってレイをうつ（とりあえずモデル番号はhModel_[0]）
				RayCastData data;
				XMStoreFloat4(&data.start, vMouseFront);
				XMStoreFloat4(&data.dir, vMouseBack - vMouseFront);
				Transform trans;
				trans.position_.x = x;
				trans.position_.y = y;
				trans.position_.z = z;
				Model::SetTransform(hModel_[0], trans);

				Model::RayCast(hModel_[0], data);

				//�E　レイが当たったらブレークポイントで止める
				if (data.hit)
				{
					if (mode_ == 0)
					{
						table_[x][z].height++;
						break;
					}
					else if (mode_ == 1)
					{
						table_[x][z].height--;
						break;
					}
				}

			}

		}
	}
}

//描画
void Stage::Draw()
{
	//Model::SetTransform(hModel_, transform_);
	//Model::Draw(hModel_);

	for (int x = 0; x < 15; x++)
	{
		for (int z = 0; z < 15; z++)
		{
			for (int y = 0; y < table_[x][z].height + 1; y++)
			{
				//table[x][z]からオブジェクトのタイプを取り出して書く！
				int type = table_[x][z].type;
				Transform trans;
				trans.position_.x = x;
				trans.position_.y = y;
				trans.position_.z = z;
				Model::SetTransform(hModel_[type], trans);
				Model::Draw(hModel_[type]);
			}
		}
	}
}

//開放
void Stage::Release()
{
}

BOOL Stage::DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_INITDIALOG:
		SendMessage(GetDlgItem(hDlg, IDC_RADIO_UP), BM_SETCHECK, BST_CHECKED, 0);

		SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_ADDSTRING,0,(LPARAM)"デフォルト");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_ADDSTRING, 0, (LPARAM)"レンガ");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_ADDSTRING, 0, (LPARAM)"草原");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_ADDSTRING, 0, (LPARAM)"砂地");
		SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_ADDSTRING, 0, (LPARAM)"水");

		SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_SETCURSEL, 0,0);

	}

	if (SendMessage(GetDlgItem(hDlg, IDC_RADIO_UP), BM_GETCHECK, 0, 0) == BST_CHECKED)
	{
		mode_ = 0;
	}
	else if (SendMessage(GetDlgItem(hDlg, IDC_RADIO_DOWN), BM_GETCHECK, 0, 0) == BST_CHECKED)
	{
		mode_ = 1;
	}

	return FALSE;
	}
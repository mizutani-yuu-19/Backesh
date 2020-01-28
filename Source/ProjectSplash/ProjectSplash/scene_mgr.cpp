///////////////////////////////////////////////////////////
//
// シーン管理
//
//////////////////////////////////////////////////////////

//-----------インクルード------------
#include "scene_mgr.h"
#include "title.h"
#include "select.h"
#include "game.h"
#include "fade.h"

//*************ベースシーンクラス(継承させて管理しやすく)****************
// コンストラクタ
CBaseScene::CBaseScene()
{
}

// デストラクタ
CBaseScene::~CBaseScene()
{
}

//*****************シーン管理クラス**********************
// コンストラクタ
CScene::CScene()
{
	m_scene = new CTitle();
	m_nextScene = SCENE_NONE;
}
// デストラクタ
CScene::~CScene()
{
}

void CScene::Init()
{
	m_scene->Init();
}
void CScene::Uninit()
{
	m_scene->Uninit();
}
void CScene::Update()
{
	if (m_nextScene != SCENE_NONE)
	{    //次のシーンがセットされていたら

		m_scene->Uninit();//現在のシーンの終了処理を実行
		m_scene->~CBaseScene();
		delete m_scene;
		switch (m_nextScene)
		{       //シーンによって処理を分岐
		case SCENE_TITLE:				 //次の画面がメニューなら
			m_scene = new CTitle();   //メニュー画面のインスタンスを生成する
			break;
		case SCENE_SELECT:
			m_scene = new CSelect();
			break;
		case SCENE_GAME:
			m_scene = new CGame();
			break;
		}

		m_scene->Init();    //シーンを初期化

		m_nextScene = SCENE_NONE;    //次のシーン情報をクリア
	}
	m_scene->Update(); //シーンの更新
}
void CScene::Draw()
{
	m_scene->Draw();
}

void CScene::ChangeScene(int nextScene)
{
	m_nextScene = nextScene;
}
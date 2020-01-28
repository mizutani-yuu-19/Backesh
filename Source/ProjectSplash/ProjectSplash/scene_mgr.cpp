///////////////////////////////////////////////////////////
//
// �V�[���Ǘ�
//
//////////////////////////////////////////////////////////

//-----------�C���N���[�h------------
#include "scene_mgr.h"
#include "title.h"
#include "select.h"
#include "game.h"
#include "fade.h"

//*************�x�[�X�V�[���N���X(�p�������ĊǗ����₷��)****************
// �R���X�g���N�^
CBaseScene::CBaseScene()
{
}

// �f�X�g���N�^
CBaseScene::~CBaseScene()
{
}

//*****************�V�[���Ǘ��N���X**********************
// �R���X�g���N�^
CScene::CScene()
{
	m_scene = new CTitle();
	m_nextScene = SCENE_NONE;
}
// �f�X�g���N�^
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
	{    //���̃V�[�����Z�b�g����Ă�����

		m_scene->Uninit();//���݂̃V�[���̏I�����������s
		m_scene->~CBaseScene();
		delete m_scene;
		switch (m_nextScene)
		{       //�V�[���ɂ���ď����𕪊�
		case SCENE_TITLE:				 //���̉�ʂ����j���[�Ȃ�
			m_scene = new CTitle();   //���j���[��ʂ̃C���X�^���X�𐶐�����
			break;
		case SCENE_SELECT:
			m_scene = new CSelect();
			break;
		case SCENE_GAME:
			m_scene = new CGame();
			break;
		}

		m_scene->Init();    //�V�[����������

		m_nextScene = SCENE_NONE;    //���̃V�[�������N���A
	}
	m_scene->Update(); //�V�[���̍X�V
}
void CScene::Draw()
{
	m_scene->Draw();
}

void CScene::ChangeScene(int nextScene)
{
	m_nextScene = nextScene;
}
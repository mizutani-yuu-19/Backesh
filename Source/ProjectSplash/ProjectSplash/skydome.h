///////////////////////////////////////////////////
//
// �X�J�C�h�[��
//
//////////////////////////////////////////////////
#ifndef _SKYDOME_H_
#define _SKYDOME_H_
//----------------�C���N���[�h-------------------
#include "main.h"
#include "game.h"
#include "model.h"
//---------------�萔�}�N��------------

// ���f���t�@�C����
#define	MODEL_SKYDOME1			"data/MODEL/classroom.x"		// �X�e�[�W1
#define	MODEL_SKYDOME2			"data/MODEL/skydome.x"		// �X�e�[�W2

//-------------------�񋓑̒�`----------------

//-------------�N���X----------------
class CSkydome
{
public:
	CSkydome(int characterType);
	~CSkydome();

	void Init(D3DXVECTOR3 pos);
	void Uninit();
	void Update();
	void Draw(int camNo);

public:	// �ÓI
	static void CreateSkydome(int type);
	static CSkydome *m_skydome;				// ����
	static void DeleteSkydome();
private:

	MODEL					m_model;

	D3DXMATRIX				m_mtxWorld;					// ���[���h�}�g���N�X

};

#endif // !_SKYDOME_H_
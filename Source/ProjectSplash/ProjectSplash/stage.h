///////////////////////////////////////////////////
//
// �X�e�[�W
//
//////////////////////////////////////////////////
#ifndef _STAGE_H_
#define _STAGE_H_
//----------------�C���N���[�h-------------------
#include "main.h"
#include "game.h"
#include "model.h"
//---------------�萔�}�N��------------

// ���f���t�@�C����
#define	MODEL_STAGE1			"data/MODEL/showstage.x"		// �X�e�[�W1
#define	MODEL_STAGE2			"data/MODEL/showstage2.x"		// �X�e�[�W2
#define	MODEL_COL_STAGE1		"data/MODEL/colstage.x"		// �����蔻��p�X�e�[�W1
#define	MODEL_COL_STAGE2		"data/MODEL/colstage2.x"		// �����蔻��p�X�e�[�W2
#define MAX_MISSING_POINT		(8)							//�G�T�[�`�~�X���̃����_�����W�p�^�[����

//-------------------�񋓑̒�`----------------


//-------------�N���X----------------
class CStage
{
public:
	CStage(int characterType);
	~CStage();

	void Init(D3DXVECTOR3 pos);
	void Uninit();
	void Update();
	void Draw(int camNo);

	static D3DXVECTOR3 m_missingPoint[MAX_MISSING_POINT];
public:	// �ÓI
	static void CreateStage(int type);
	static CStage *m_stage;				// ����
	void CStage::GetStageBound(D3DXVECTOR3 *max, D3DXVECTOR3 *min);	// �X�e�[�W�̃T�C�Y
	D3DXVECTOR3 GetStageSize();

	LPD3DXMESH				m_pColStageMeshModel;			// ���b�V�����ւ̃|�C���^
private:
	// �Փ˔���p���f��
	LPD3DXBUFFER			m_pColStageBuffMatModel;		// �}�e���A�����ւ̃|�C���^
	DWORD					m_nColStageNumMatModel;			// �}�e���A�����̐�
															// �����ڃ��f��
	MODEL					m_showStageModel;			// ���b�V�����ւ̃|�C���^

	D3DXVECTOR3				m_size;						// �傫��

	D3DXMATRIX				m_mtxWorld;					// ���[���h�}�g���N�X

	D3DXVECTOR3			m_max;
	D3DXVECTOR3			m_min;
};

#endif // !_STAGE_H_
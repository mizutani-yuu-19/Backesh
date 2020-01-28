///////////////////////////////////////////////////
//
// キャラクター
//
//////////////////////////////////////////////////
#ifndef _PLAYER_H_
#define _PLAYER_H_
//----------------インクルード-------------------
#include "main.h"
#include "water_atk.h"
#include "game.h"
#include "scene_mgr.h"
#include "water_puddle.h"
#include "character.h"
//---------------定数マクロ------------


//-------------クラス----------------
class CPlayer : public CCharacter
{
public:
	CPlayer(int characterType, int teamNo, int bucketType);
	~CPlayer();

	void Init();
	void Uninit();
	void Update();
	void Draw(int camNo);

	void				SkillUpdate();		// 必殺技更新
private:
	void				Operate();			// 操作全般

	void				LockOn();			// ロックオン			
	void				LockOnUpdate();		// ロックオンアップデート			
	void				LockOnDraw();		// ロックオン描画
	D3DXVECTOR3			m_lockOnPos;		// ロックオン中の敵の位置
	int					m_lockOnNo;			// ロックしているキャラ	１０の位にチームナンバー、１の位にメンバーナンバー

	int					m_skillTime;		// スキル使用中のカウント
};

#endif // !_PLAYER_H_
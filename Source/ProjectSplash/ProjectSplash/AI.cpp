///////////////////////////////////////////////////
//
// キャラクター
//
//////////////////////////////////////////////////
//----------------インクルード--------------
#include "ai.h"
#include "game.h"
#include "game_pad.h"
#include "camera.h"
#include "collision.h"
#include "input.h"
#include "water_puddle.h"
#include "debugproc.h"
#include "AI.h"
#include "player.h"
#include "stage.h"

//-------------定数マクロ--------------
#define VALUE_MOVE_AI		(2.0f)					//AI移動速度
#define MAX_ROTCOUNT		(6)						//回転を与えるまでの最大フレーム数
#define MAX_RESETCOUNT		(30)					//回転をリセットするカウント
#define MAX_ROTPATCOUNT		(240)					//回転方向を変えるまでの最大フレーム数
#define SENSOR_DIST			(60.0f)					//壁との衝突を感知する距離
#define SEARCH_RANGE		(400.0f)				//AIの敵サーチ範囲
#define MAX_SEARCHCOUNT		(0)						//最短距離にいる敵の探索を更新するカウント数
#define ATTACK_RANGE		(80.0f)				//攻撃を行う範囲
#define JUMP_RANGE			(50.0f)					//ジャンプを行うか判断する距離


//----------グローバル-----------
// コンストラクタ
CAi::CAi(int aiType, int teamNo, int bucketType) :CCharacter(aiType, teamNo, bucketType)
{
	//攻撃パターンの初期化
	m_atkType = 0;

	//賢さを与える
	//AIの賢さを０～４の５段階にランダムに割り振る
	m_intelligence = rand() % 5;

	//壁を認識した時方向を変えるカウント初期化
	m_wallTime = 0;

	//上の地点を目指すためのフラグ初期化
	m_markUP = false;

	//AIと敵の最短距離にステージ幅をいれて初期化しておく
	m_pDistVector = D3DXVECTOR3(CStage::m_stage->GetStageSize().x, CStage::m_stage->GetStageSize().y, CStage::m_stage->GetStageSize().z);

	//敵探索カウント初期化
	m_searchCount = MAX_SEARCHCOUNT;

	//ランダムサーチ初期化
	m_randSearch = false;

	//モード初期化
	m_modeRotWall = false;

	//上昇中フラグ初期化
	m_upStairs = false;

}
void CAi::ChangeOccasion() {
	//壁を認識した時方向を変えるカウント初期化
	m_wallTime = 0;

	//上の地点を目指すためのフラグ初期化
	m_markUP = false;

	//AIと敵の最短距離にステージ幅をいれて初期化しておく
	m_pDistVector = D3DXVECTOR3(CStage::m_stage->GetStageSize().x, CStage::m_stage->GetStageSize().y, CStage::m_stage->GetStageSize().z);

	//敵探索カウント初期化
	m_searchCount = MAX_SEARCHCOUNT;

	//壁を認識した時方向を変えるカウント初期化
	m_wallTime = 0;

	//ジャンプ可能な壁との最短距離初期化
	m_wallDistJump = 0.0f;

	//回転リセットカウントの初期化
	m_resetCount = 0;

	//AIと敵の角度初期化
	m_pRotEnemy = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//各種初期化
	m_markUpPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_markUpDistVector = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_upStairsPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_upStairsVector = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//ターゲット番号関係初期化
	m_targetNo = 0;
	m_prevTargetNo = 0;

	//避難関係初期化
	m_refugePos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_refugeVector = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//難易度
	difficulty = rand() % 4;

	//モード初期化
	m_modeRotWall = false;

	//ランダムサーチ初期化
	m_randSearch = false;

	//上昇中フラグ初期化
	m_upStairs = false;
}

// デストラクタ
CAi::~CAi() {

}

// 位置とか初期化
void CAi::Init() {
	CCharacter::Init();
	m_markUP = false;
}

void CAi::Uninit() {

}

void CAi::Update() {
	//各キャラクターの動きの設定
	if (CGame::GetStep() == STEP_GAME) {
		if (m_surviveFlag&&m_pickUpTime <= 0) {
			Operate();
		}
	}
	// キャラクターのアップデート
	CCharacter::Update();
}

void CAi::Draw(int camNo) {
	CCharacter::Draw(camNo);
}


// 操作
void CAi::Operate()
{
	D3DXMATRIX *y = new D3DXMATRIX;

	//メインキャラクター以外

	//２パターンで抽選を行う
	m_atkType = rand() % 2;



	//AIの水がなかったら
	if (m_waterVolume < 1.0f&&m_pickUpHandle == -1)
	{
		//０番目の水たまりの情報を初期値として扱う
		//if (m_puddleDistMin==NULL) {
		m_puddlePos = CWaterPuddle::GetWaterPuddlePos(0);
		m_puddleDistvector = m_puddlePos - m_beforePos;
		m_puddleDistvectorMin = m_puddleDistvector;
		m_puddleDistMin = CWaterPuddle::m_waterPuddle[0];
		//}
		//全水たまりの数だけ調査
		for (int i = 0; i < CWaterPuddle::m_waterPuddle.size(); i++) {
			m_puddlePos = CWaterPuddle::GetWaterPuddlePos(i);
			m_puddleDistvector = m_puddlePos - m_beforePos;
			//水たまりまでの最短距離が見つかり次第更新
			if (D3DXVec3Length(&m_puddleDistvector) < D3DXVec3Length(&m_puddleDistvectorMin))		// 比較をD3DXVec3Lengthに変えたら最短を狙うようになった
			{
				m_puddleDistvectorMin = m_puddleDistvector;
				m_puddleDistMin = CWaterPuddle::m_waterPuddle[i];
			}
		}

		//AIから水たまりまでの角度を求める
		//オブジェクトを見つけてよけるために向きの更新を行っているときは
		//水たまりとの向き更新を行わない
		if (!m_modeRotWall && !m_markUP)
		{
			m_rot.y = atan2f(m_puddleDistvectorMin.z, m_puddleDistvectorMin.x);
		}

		//移動量更新
		//向きの更新と同様に移動量の更新も制御
		if (!m_modeRotWall && !m_markUP)
		{
			m_move.x = cosf(m_rot.y) * VALUE_MOVE_AI;
			m_move.z = sinf(m_rot.y) * VALUE_MOVE_AI;
		}

		//水たまりの上にプレイヤーがいたら
		if (m_puddleDistMin) {
			if (CollisionAABB(m_beforePos + D3DXVECTOR3(0, m_size.y*0.5f, 0), m_size, m_puddleDistMin->m_pos, m_puddleDistMin->m_size))
			{
				PickUpWater();
			}
		}
	}

	//AIが水を所持していたら
	else
	{
		m_puddleDistMin = NULL;

		//敵座標取得
		//敵の座標を取得、格納
		for (int i = 0; i < m_teamNum; i++)
		{
			// 自分のチームメンバーだったらスルー
			if (m_teamNo == i)	continue;
			for (int j = 0; j < MAX_TEAMMEMBER; j++)
			{
				if (!CCharacter::GetCharacterSurvive(i, j))	continue;
				//一時的に敵の座標を格納
				m_pPosTmp = CCharacter::GetCharacterPos(i, j);

				//敵とAIとの距離(ベクトル)を求める
				//一時的に格納、最短距離比較用
				m_pDistVectorTmp = m_pPosTmp - m_beforePos;

				//最短距離にいる敵を探す
				if (D3DXVec3Length(&m_pDistVector) > D3DXVec3Length(&m_pDistVectorTmp))
				{
					m_pDistVector = m_pDistVectorTmp;
					//最短距離にいる敵の座標を取得
					m_pPos = m_pPosTmp;
					m_targetNo = i * 10 + j;
					m_shiftPos = true;
				}
			}
		}

		//ターゲット番号に変更があった時、上に行くのをやめる
		if (m_targetNo != m_prevTargetNo)
		{
			m_markUP = false;
			m_prevTargetNo = m_targetNo;
		}


		//AIの賢さに応じて操作プレイヤーの座標を取得した値をずらす
		//賢さ（m_intelligence）の値が大きいほど大きなずれが生じる
		/*if (!m_randSearch && !m_modeRotWall && !m_markUP && m_shiftPos)
		{
		switch (difficulty)
		{
		case 0:
		m_pPos.x += rand() % 20 - 10;
		m_pPos.z += rand() % 20 - 10;

		case 1:
		m_pPos.x += rand() % 40 - 20;
		m_pPos.z += rand() % 40 - 20;

		case 2:
		m_pPos.x += rand() % 60 - 30;
		m_pPos.z += rand() % 60 - 30;

		case 3:
		m_pPos.x += rand() % 100 - 50;
		m_pPos.z += rand() % 100 - 50;

		case 4:
		m_pPos.x += rand() % 200 - 100;
		m_pPos.z += rand() % 200 - 100;
		}

		m_shiftPos = false;
		}
		*/

		//敵とAIとの距離(ベクトル)を求める
		m_pDistVector = m_pPos - m_beforePos;


		D3DXVECTOR3 vec1;
		D3DXVECTOR3 vec2;

		//正規化
		D3DXVec3Normalize(&vec1, &m_move);
		D3DXVec3Normalize(&vec2, &m_pDistVector);

		//AIから敵の角度を求める
		//AIと敵の角度を常に持ってもらう
		m_pRotEnemy.y = acosf(D3DXVec3Dot(&vec1, &vec2));

		//オブジェクトを見つけてよけるために向きの更新及び
		//上に向かうための移動を行っているときは
		//敵との向き更新を行わない
		if (!m_modeRotWall && !m_markUP)
		{
			m_rot.y = atan2f(m_pDistVector.z, m_pDistVector.x);
		}

		//敵が前方の半円に入っているかつ、距離が近かったら敵を感知する
		if (m_markUP || m_pRotEnemy.y < D3DX_PI * 0.5f && D3DXVec3Length(&m_pDistVector) < SEARCH_RANGE)
		{
			//ランダムサーチのフラグを下す
			m_randSearch = false;

			//XZそれぞれにcos,sinを渡す、移動量を求める
			if (!m_modeRotWall)
			{
				m_move.x = cosf(m_rot.y) * VALUE_MOVE_AI;
				m_move.z = sinf(m_rot.y) * VALUE_MOVE_AI;
			}

			//敵が自分の高さよりも高い場所にいたら
			//坂や階段の下を目指す
			if (m_pPos.y > m_beforePos.y + 60.0f && !m_modeRotWall && !m_upStairs)			//ジャンプ量が大体６０
			{
				//上を目指すフラグを立てる
				m_markUP = true;


				//上に行くために一時的に目指す座標（水平方向、ｙ座標０）を格納
				m_markUpPos = D3DXVECTOR3(0.0f, 0.0f, 800.0f);						//仮でおいてあります	

				//AIからの距離(ベクトル)を求める
				m_markUpDistVector = m_markUpPos - m_beforePos;

				//AIからの角度を求める
				m_rot.y = atan2f(m_markUpDistVector.z, m_markUpDistVector.x);
			}

			if (m_pPos.y < m_beforePos.y + 60.0f)
			{
				m_markUP = false;
				m_upStairs = false;
			}

			//目標の座標まで来たら
			if (D3DXVec3Length(&m_upStairsVector) < 10.0f && m_upStairs && !m_modeRotWall)
			{
				//上を目指すフラグをおろす
				m_markUP = false;
				m_upStairs = false;
			}


			//坂や階段の下に来たら
			if (m_markUP && D3DXVec3Length(&m_markUpDistVector) < 10.0f && !m_modeRotWall)
			{
				//上昇中フラグを立てる
				m_upStairs = true;

				//坂や階段の上に当たる座標を格納
				m_upStairsPos = D3DXVECTOR3(0.0f, 0.0f, -800.0f);						//仮で置いてあります

				m_upStairsVector = m_upStairsPos - m_beforePos;

				m_rot.y = atan2f(m_upStairsVector.z, m_upStairsVector.x);
			}


			//AIがキャラクターから一定以上近かったら
			if (D3DXVec3Length(&m_pDistVector) < ATTACK_RANGE)
			{
				//移動しない
				m_move.x = cosf(m_rot.y) * 0.0f;
				m_move.z = sinf(m_rot.y) * 0.0f;


				//AI攻撃
				if (m_waterVolume >= 1.0f)
				{
					if (!m_water) {
						D3DXVECTOR3 angle, pos;
						pos = m_beforePos;
						pos.y += 10.0f;
						m_water = new CWaterAtk(pos, m_rot.y*-1.0f + D3DX_PI*0.5f, m_waterVolume, m_bucketType);
						m_waterVolume = 0.0f;
					}
				}
			}

			//遠かったら
			//向きの更新と同様に移動量の更新も制御
			else if (!m_modeRotWall /*&& m_life >= 50*/)
			{
				//移動量更新
				m_move.x = cosf(m_rot.y) * VALUE_MOVE_AI;
				m_move.z = sinf(m_rot.y) * VALUE_MOVE_AI;
			}

			/*if (m_life < 50)
			{
			m_refugePos = D3DXVECTOR3(800.0f, 0.0f, 0.0f);

			m_refugeVector = m_refugePos - m_beforePos;

			m_rot.y = atan2f(m_refugeVector.z, m_refugeVector.x);

			//移動量更新
			m_move.x = cosf(m_rot.y) * VALUE_MOVE_AI;
			m_move.z = sinf(m_rot.y) * VALUE_MOVE_AI;
			}
			*/
		}



		//敵を探して見つからなかったら、ランダムな座標に向かわせる
		//その座標に行くまでに敵が見つかったらサーチ成功のif文に入る
		//失敗したら、またランダム座標を与えるの繰り返し
		else if (!m_randSearch)
		{
			//マップ上のランダム座標を与える
			m_pRandPos = CStage::m_missingPoint[rand() % MAX_MISSING_POINT];

			//フラグをたてる
			m_randSearch = true;

		}

		else if (m_randSearch)
		{
			D3DXVECTOR3 temp = m_pRandPos - m_beforePos;
			if (D3DXVec3Length(&temp) < 50.0f)
			{
				m_randSearch = false;
			}
		}
		if (m_randSearch && !m_modeRotWall && !m_markUP) {
			//ランダム座標とAIとの距離(ベクトル)を求める
			m_pDistVector = m_pRandPos - m_beforePos;


			//AIからランダム座標の角度を求める
			m_rot.y = atan2f(m_pDistVector.z, m_pDistVector.x);

			//移動量を与える
			//XZそれぞれにcos,sinを渡す
			m_move.x = cosf(m_rot.y) * VALUE_MOVE_AI;
			m_move.z = sinf(m_rot.y) * VALUE_MOVE_AI;
		}
	}

	//ジャンプで越えられるオブジェクトの最短距離を出す
	if (CollisionRayStage(m_beforePos, m_move, D3DXVECTOR3(0.0f, 50.0f, 0.0f), CStage::m_stage->m_pColStageMeshModel, &m_wallDistJump))
	{
	}
	else
	{
		m_wallDistJump = 9999.0f;
	}

	//背の低い障害物をジャンプで飛び越える
	if (m_wallDistFront <= JUMP_RANGE && m_wallDistJump > JUMP_RANGE && !m_jumpFlag)
	{
		m_move += D3DXVECTOR3(0.0f, 5.0f, 0.0f);
		m_jumpFlag = true;
	}


	//前方にオブジェクトがあったらオブジェクトをよける(向きを回転)
	if (m_wallDistFront < SENSOR_DIST && m_wallTime <= 0 && m_wallDistJump < JUMP_RANGE)
	{
		m_modeRotWall = true;

		if (m_rotCount <= 0)
		{
			//回転方向を抽選
			m_rotPattern = rand() % 2;
			m_rotCount = MAX_ROTPATCOUNT;
		}

		//右か左、ランダム方向に回転
		if (m_rotPattern == 0)
		{
			m_rot.y += D3DX_PI / 12.0f;
		}

		else
		{
			m_rot.y -= D3DX_PI / 12.0f;
		}
		m_wallTime = MAX_ROTCOUNT;
		m_resetCount = MAX_RESETCOUNT;

		//移動量更新
		m_move.x = cosf(m_rot.y) * VALUE_MOVE_AI;
		m_move.z = sinf(m_rot.y) * VALUE_MOVE_AI;

	}



	//曲がるフラグを下す
	else if (m_wallDistFront >= SENSOR_DIST && m_modeRotWall == true && m_resetCount < 0)
	{
		m_modeRotWall = false;
	}

	//壁認識カウント減少
	m_wallTime--;

	//回転リセットカウント減少
	m_resetCount--;

	//回転方向変更カウント
	m_rotCount--;

	if (m_searchCount <= 0)
	{
		//５秒経過ごとに敵とAIの距離を最大値に設定しなおす
		m_pDistVector = D3DXVECTOR3(CStage::m_stage->GetStageSize().x, CStage::m_stage->GetStageSize().y, CStage::m_stage->GetStageSize().z);
		m_puddleDistvectorMin = D3DXVECTOR3(9999, 9999, 9999);
		m_searchCount = MAX_SEARCHCOUNT;
	}

	//最短距離にいる敵探索カウント減少
	m_searchCount--;

	// 横歩きを回避
	m_rot.y += D3DX_PI*-0.5f;
	delete y;
}

///////////////////////////////////////////////////
//
// タイトル
//
//////////////////////////////////////////////////
#ifndef _TITLE_H_
#define _TITLE_H_
//----------------インクルード-------------------
#include "main.h"
#include "scene_mgr.h"
#include "sprite.h"
#include "fade.h"

//列挙体
enum TITLE_TEXTURE	//タイトルに使う画像のナンバリング
{
	NUM_BACK = 0,	//バック画面
	NUM_TEAMLOGO,	//チームロゴマーク
	NUM_STAFF,		//スタッフ
	NUM_TITLEBACK,	//タイトルバック画面
	NUM_GAMELOGO,	//ゲームロゴ
	NUM_SPLASH,		//スプラッシュ
	NUM_BUCKET,		//バケツ
	NUM_BUTTON,		//ボタン

	MAX_NUM_TEXTURE,//テクスチャの最大数
};
//
typedef struct
{
	D3DXVECTOR2 pos;
	float sizeX;
	float sizeY;

}BASIC_PARAM;

//-------------クラス----------------
class CTitle : public CBaseScene
{
public:

	CTitle();
	~CTitle();

	void Init();
	void Uninit();
	void Update();
	void Draw();

private:

	//バックグラウンド情報
	BASIC_PARAM m_backGroundParam;
	//タイトルバック
	BASIC_PARAM m_titleBackParam;
	//チームロゴ情報
	BASIC_PARAM m_teamLogoParam;
	//スタッフ画像情報
	BASIC_PARAM m_staffParam;
	//ゲームロゴ情報
	BASIC_PARAM m_gameLogoParam;
	//スプラッシュエフェクト情報
	BASIC_PARAM m_splashParam;
	//バケツ情報
	BASIC_PARAM m_BucketParam;
	//ボタン設定情報
	BASIC_PARAM m_buttonParam;

	//フェード待ち時間
	int m_fadeCount;
	//切り替わる番号
	int m_switchNo;

	//カラー設定
	int m_alpha;

	//ボタンカウント
	int m_buttomCount;
	//ボタンのスイッチ
	bool m_switchButton;
	//押されていたら
	bool pushflag;
	//バケツ
	bool m_bucketSet;
	float m_bucketMove;
	//スプラッシュ
	//ゲームロゴ
	//ゲームロゴの表示が終わったか
	bool m_gamelogoflag;
	//
	D3DXVECTOR2 m_gamelogoScaleCnt;
	//
	int m_gamelogNo;
	int m_gamelogoNum;
	//
	float m_gamelogoMove;
	//
	int m_gamelogoCnt;

	//デバイス取得
	LPDIRECT3DDEVICE9 m_pDevice;
	// 作成テクスチャ
	LPDIRECT3DTEXTURE9	m_pTexSplitTitle[MAX_NUM_TEXTURE];
	// 作成テクスチャサーフェイス
	LPDIRECT3DSURFACE9	m_pSurfaceSplitTitle;
	// 画面のバッファー
	LPDIRECT3DSURFACE9 m_pBackBuffer;

	//タイトルに使うテクスチャの数
	CSprite *m_spriteTitle[MAX_NUM_TEXTURE];

	//ゲームロゴ用
	CSprite*m_pSpriteGamaeLogo[5];

	CFade *m_fade;
	//テクスチャーのロード
	void LoadTexture();
	void StartTitle();

};

#endif // !_TITLE_H_

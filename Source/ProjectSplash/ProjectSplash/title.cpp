///////////////////////////////////////////////////
//
// タイトル
//
//////////////////////////////////////////////////
//----------------インクルード--------------
#include "title.h"
#include "input.h"
#include "game_pad.h"
#include "debugproc.h"
#include <math.h>

// 定数定義

//バックグラウンドの位置
#define BACKGROUND_POS_X (SCREEN_CENTER_X)
#define BACKGROUND_POS_Y (SCREEN_CENTER_Y)
//バックグラウンドの大きさ
#define BACKGROUND_WIDTH (SCREEN_WIDTH)
#define BACKGROUND_HEIGTH (SCREEN_HEIGHT)

//タイトルバックの位置
#define TITLEBACK_POS_X (SCREEN_CENTER_X)
#define TITLEBACK_POS_Y (SCREEN_CENTER_Y)
//タイトルバックの大きさ
#define TITLEBACK_WIDTH (SCREEN_WIDTH)
#define TITLEBACK_HEIGTH (SCREEN_HEIGHT)

//スタッフ画像の位置
#define STAFF_POS_X (SCREEN_CENTER_X)
#define STAFF_POS_Y (SCREEN_CENTER_Y)
//スタッフ画像の大きさ
#define STAFF_WIDTH (830)
#define STAFF_HEIGTH (500)

//チームロゴマークの位置
#define TEAMLOGO_POS_X (SCREEN_CENTER_X)
#define TEAMLOGO_POS_Y (SCREEN_CENTER_Y)
//チームロゴの大きさ
#define TEAMLOGO_WIDTH  (700.0f)
#define TEAMLOGO_HEIGTH (400.0f)

//ゲームロゴマークの位置
#define GAMELOGO_POS_X (SCREEN_CENTER_X)
#define GAMELOGO_POS_Y (SCREEN_CENTER_Y)
//ゲームロゴの大きさ
#define GAMELOGO_WIDTH  (100.0f)
#define GAMELOGO_HEIGTH (50.0f)

//スプラッシュ画像の位置
#define SPLASH_POS_X (SCREEN_CENTER_X)
#define SPLASH_POS_Y (SCREEN_CENTER_Y)
//スプラッシュ画像大きさ
#define SPLASH_WIDTH  (600.0f)
#define SPLASH_HEIGTH (300.0f)

//バケツの位置
#define BUCKET_POS_X (SCREEN_WIDTH+100.0f)
#define BUCKET_POS_Y (SCREEN_CENTER_Y+200.0f)
//バケツ大きさ
#define BUCKET_WIDTH  (200.0f)
#define BUCKET_HEIGTH (200.0f)

//ボタンの位置
#define BUTTON_POS_X (SCREEN_CENTER_X )
#define BUTTON_POS_Y (SCREEN_CENTER_Y + 200.0f)
//ボタンの大きさ
#define BUTTON_WIDTH (200.0f)
#define BUTTON_HEIGTH (100.0f)


#define MAX_FADE_COUNT (20)
#define MAX_BUTTON_COUNT (30)

#define TITLE_DEVIDE_U (1)
#define TITLE_DEVIDE_V (1)

#define TITLE_BACK_PATH		 ("data/TEXTURE/title/background.png")
#define TITLE_TEAMLOGO_PATH	 ("data/TEXTURE/title/teamlogo.png")
#define TITLE_STAFF_PATH	 ("data/TEXTURE/title/staff.png")
#define TITLE_TITLEBACK_PATH ("data/TEXTURE/title/titleback.png")
#define TITLE_GAMELOGO_PATH	 ("data/TEXTURE/title/gamelogo.png")
#define TITLE_SPLASH_PATH	 ("data/TEXTURE/title/splash.png")
#define TITLE_BUCKET_PATH	 ("data/TEXTURE/title/bucket.png")
#define TITLE_BUTTON_PATH	 ("data/TEXTURE/title/button.png")

//---------------メンバ関数---------------

// コンストラクタ
CTitle::CTitle()
{
	m_pDevice = GetDevice();
	//バックグラウンド
	m_backGroundParam.pos = D3DXVECTOR2(0.0f, 0.0f);
	m_backGroundParam.sizeX = 0.0f;
	m_backGroundParam.sizeY = 0.0f;
	//タイトルバック
	m_titleBackParam.pos = D3DXVECTOR2(0.0f, 0.0f);
	m_titleBackParam.sizeX = 0.0f;
	m_titleBackParam.sizeY = 0.0f;
	//チームロゴ
	m_teamLogoParam.pos = D3DXVECTOR2(0.0f, 0.0f);;
	m_teamLogoParam.sizeX = 0.0f;
	m_teamLogoParam.sizeY = 0.0f;
	//スタッフ画像
	m_staffParam.pos = D3DXVECTOR2(0.0f, 0.0f);;
	m_staffParam.sizeX = 0.0f;
	m_staffParam.sizeY = 0.0f;
	//ゲームロゴ
	m_gameLogoParam.pos = D3DXVECTOR2(0.0f, 0.0f);;
	m_gameLogoParam.sizeX = 0.0f;
	m_gameLogoParam.sizeY = 0.0f;
	//スプラッシュエフェクト
	m_splashParam.pos = D3DXVECTOR2(0.0f, 0.0f);;
	m_splashParam.sizeX = 0.0f;
	m_splashParam.sizeY = 0.0f;
	//バケツ
	m_BucketParam.pos = D3DXVECTOR2(0.0f, 0.0f);;
	m_BucketParam.sizeX = 0.0f;
	m_BucketParam.sizeY = 0.0f;
	//ボタン設定
	m_buttonParam.pos = D3DXVECTOR2(0.0f, 0.0f);;
	m_buttonParam.sizeX = 0.0f;
	m_buttonParam.sizeY = 0.0f;


	m_fadeCount = 0;
	m_buttomCount = 0;
	m_switchNo = 0;
	m_alpha = 255;
	m_switchButton = false;
	pushflag = false;
	for (int i = 0; i < MAX_NUM_TEXTURE; i++)
	{
		m_spriteTitle[i] = new CSprite();
	}
	for (int i = 0; i < 5; i++)
	{
		m_pSpriteGamaeLogo[i] = new CSprite();
	}
	m_fade = new CFade();
	m_gamelogoflag = false;
	m_gamelogNo = 0;
	m_gamelogoMove = 0.0f;
	m_gamelogoScaleCnt = D3DXVECTOR2(0.0f, 0.0f);
	m_gamelogoCnt = 10;
	m_gamelogoNum = 0;
	m_bucketSet = false;
	m_bucketMove = 0.0f;
	LoadTexture();
}

// デストラクタ
CTitle::~CTitle()
{
	Uninit();
}

//初期化
void CTitle::Init()
{
	//バックグラウンド
	m_backGroundParam.pos = D3DXVECTOR2(BACKGROUND_POS_X, BACKGROUND_POS_Y);
	m_backGroundParam.sizeX = BACKGROUND_WIDTH;
	m_backGroundParam.sizeY = BACKGROUND_HEIGTH;
	//タイトルバック
	m_titleBackParam.pos = D3DXVECTOR2(TITLEBACK_POS_X, TITLEBACK_POS_Y);
	m_titleBackParam.sizeX = TITLEBACK_WIDTH;
	m_titleBackParam.sizeY = TITLEBACK_HEIGTH;

	m_teamLogoParam.pos = D3DXVECTOR2(TEAMLOGO_POS_X, TEAMLOGO_POS_Y);
	m_teamLogoParam.sizeX = TEAMLOGO_WIDTH;
	m_teamLogoParam.sizeY = TEAMLOGO_HEIGTH;

	m_staffParam.pos = D3DXVECTOR2(STAFF_POS_X, STAFF_POS_Y);
	m_staffParam.sizeX = STAFF_WIDTH;
	m_staffParam.sizeY = STAFF_HEIGTH;

	m_gameLogoParam.pos = D3DXVECTOR2(GAMELOGO_POS_X - 200.0f, GAMELOGO_POS_Y);
	m_gameLogoParam.sizeX = GAMELOGO_WIDTH;
	m_gameLogoParam.sizeY = GAMELOGO_HEIGTH;

	m_splashParam.pos = D3DXVECTOR2(SPLASH_POS_X, SPLASH_POS_Y);
	m_splashParam.sizeX = SPLASH_WIDTH;
	m_splashParam.sizeY = SPLASH_HEIGTH;

	m_BucketParam.pos = D3DXVECTOR2(BUCKET_POS_X, BUCKET_POS_Y);
	m_BucketParam.sizeX = BUCKET_WIDTH;
	m_BucketParam.sizeY = BUCKET_HEIGTH;

	m_buttonParam.pos = D3DXVECTOR2(BUTTON_POS_X, BUTTON_POS_Y);
	m_buttonParam.sizeX = BUTTON_WIDTH;
	m_buttonParam.sizeY = BUTTON_HEIGTH;

	m_spriteTitle[NUM_BACK]->InitSprite(m_backGroundParam.pos, m_backGroundParam.sizeX, m_backGroundParam.sizeY, 1, 1);
	m_spriteTitle[NUM_TITLEBACK]->InitSprite(m_titleBackParam.pos, m_titleBackParam.sizeX, m_titleBackParam.sizeY, 1, 1);
	m_spriteTitle[NUM_TEAMLOGO]->InitSprite(m_teamLogoParam.pos, m_teamLogoParam.sizeX, m_teamLogoParam.sizeY, 1, 1);
	m_spriteTitle[NUM_STAFF]->InitSprite(m_staffParam.pos, m_staffParam.sizeX, m_staffParam.sizeY, 1, 1);
	for (int i = 0; i < 5; i++)
	{
		m_pSpriteGamaeLogo[i]->InitSprite((m_gameLogoParam.pos + D3DXVECTOR2(m_gameLogoParam.sizeX * i, 0.0f)), m_gameLogoParam.sizeX, m_gameLogoParam.sizeY, 5, 1);
		m_pSpriteGamaeLogo[i]->m_patNum = i;
		m_pSpriteGamaeLogo[i]->m_scale = D3DXVECTOR2(0.0f, 0.0f);
	}
	m_spriteTitle[NUM_SPLASH]->InitSprite(m_splashParam.pos, m_splashParam.sizeX, m_splashParam.sizeY, 1, 1);
	m_spriteTitle[NUM_SPLASH]->m_scale = D3DXVECTOR2(0.0f, 0.0f);;
	m_spriteTitle[NUM_BUCKET]->InitSprite(m_BucketParam.pos, m_BucketParam.sizeX, m_BucketParam.sizeY, 1, 1);
	m_spriteTitle[NUM_BUTTON]->InitSprite(m_buttonParam.pos, m_buttonParam.sizeX, m_buttonParam.sizeY, 1, 1);
	m_spriteTitle[NUM_BUTTON]->m_color = D3DCOLOR_ARGB(0, 255, 255, 255);

	m_fadeCount = MAX_FADE_COUNT;

	m_buttomCount = MAX_BUTTON_COUNT;

	StartFadeIn();
}

//終了
void CTitle::Uninit()
{
	for (int i = 0; i < MAX_NUM_TEXTURE; i++)
	{
		if (m_spriteTitle[i])
		{
			delete m_spriteTitle[i];
			m_spriteTitle[i] = NULL;
		}

		if (m_fade)
		{
			delete m_fade;
			m_fade = NULL;
		}

		SAFE_RELEASE(m_pTexSplitTitle[i]);
	}
	for (int i = 0; i < 5; i++)
	{
		if (m_pSpriteGamaeLogo[i])
		{
			delete m_pSpriteGamaeLogo[i];
			m_pSpriteGamaeLogo[i] = NULL;
		}
	}
	SAFE_RELEASE(m_pBackBuffer);
	SAFE_DELETE(m_fade);
	SAFE_RELEASE(m_pSurfaceSplitTitle);

}

//更新
void CTitle::Update()
{
	if (EndFadeIn())
	{
		m_alpha = m_fade->UpdateFadeTex();

		if (m_switchNo < NUM_TITLEBACK)
		{
			if (m_fade->EndFadeInTex())
			{
				m_switchNo++;
				m_fade->StartFadeOutTex();
			}

			if (m_fade->EndFadeOutTex())
			{
				if (m_fadeCount < 0)
				{
					m_fade->StartFadeInTex();
					m_fadeCount = 1;
				}
				m_fadeCount--;
			}
		}
	}
	switch (m_switchNo)
	{
	case NUM_TEAMLOGO:
		m_spriteTitle[m_switchNo]->m_color = D3DCOLOR_ARGB(m_alpha, 255, 255, 255);
		break;
	case NUM_STAFF:
		m_spriteTitle[m_switchNo]->m_color = D3DCOLOR_ARGB(m_alpha, 255, 255, 255);
		break;
	case NUM_TITLEBACK:

		StartTitle();
		break;
	}


}
//描画
void CTitle::Draw()
{
	m_spriteTitle[NUM_BACK]->DrawSprite(m_pTexSplitTitle[NUM_BACK]);
	switch (m_switchNo)
	{
	case NUM_TEAMLOGO:
		m_spriteTitle[NUM_TEAMLOGO]->DrawSprite(m_pTexSplitTitle[NUM_TEAMLOGO]);
		break;
	case NUM_STAFF:
		m_spriteTitle[NUM_STAFF]->DrawSprite(m_pTexSplitTitle[NUM_STAFF]);
		break;
	case NUM_TITLEBACK:
		m_spriteTitle[NUM_TITLEBACK]->DrawSprite(m_pTexSplitTitle[NUM_TITLEBACK]);
		m_spriteTitle[NUM_SPLASH]->DrawSprite(m_pTexSplitTitle[NUM_SPLASH]);
		for (int i = 0; i < 5; i++)
		{
			m_pSpriteGamaeLogo[i]->DrawSprite(m_pTexSplitTitle[NUM_GAMELOGO]);
		}
		m_spriteTitle[NUM_BUCKET]->DrawSprite(m_pTexSplitTitle[NUM_BUCKET]);
		m_spriteTitle[NUM_BUTTON]->DrawSprite(m_pTexSplitTitle[NUM_BUTTON]);
		break;
	}
}

//ロード
void CTitle::LoadTexture()
{
	for (int i = 0; i < MAX_NUM_TEXTURE; i++)
	{
		// バックバッファのアドレス取得
		m_pDevice->GetRenderTarget(0, &m_pBackBuffer);

		// テクスチャ作成
		m_pDevice->CreateTexture(BACKGROUND_WIDTH, BACKGROUND_HEIGTH, 0, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pTexSplitTitle[i], NULL);

		// テクスチャからサーフェイスを作成
		m_pTexSplitTitle[i]->GetSurfaceLevel(0, &m_pSurfaceSplitTitle);
		switch (i)
		{
		case NUM_BACK:
			m_pTexSplitTitle[i] = SetTexture(TITLE_BACK_PATH);
			break;
		case NUM_TEAMLOGO:
			m_pTexSplitTitle[i] = SetTexture(TITLE_TEAMLOGO_PATH);
			break;
		case NUM_STAFF:
			m_pTexSplitTitle[i] = SetTexture(TITLE_STAFF_PATH);
			break;
		case NUM_TITLEBACK:
			m_pTexSplitTitle[i] = SetTexture(TITLE_TITLEBACK_PATH);
			break;
		case NUM_GAMELOGO:
			m_pTexSplitTitle[i] = SetTexture(TITLE_GAMELOGO_PATH);
			break;
		case NUM_SPLASH:
			m_pTexSplitTitle[i] = SetTexture(TITLE_SPLASH_PATH);
			break;
		case NUM_BUCKET:
			m_pTexSplitTitle[i] = SetTexture(TITLE_BUCKET_PATH);
			break;
		case NUM_BUTTON:
			m_pTexSplitTitle[i] = SetTexture(TITLE_BUTTON_PATH);
			break;
		}
		m_pBackBuffer = NULL;
	}

}

//タイトル画面の処理
void CTitle::StartTitle()
{
	//タイトルバックのフェイド
	if (m_fadeCount < 0)
		m_alpha = m_fade->UpdateFadeTex();
	m_spriteTitle[NUM_TITLEBACK]->m_color = D3DCOLOR_ARGB(m_alpha, 255, 255, 255);
	if (m_fade->EndFadeOutTex())
	{
		//バケツ
		if (!m_bucketSet)
		{
			m_BucketParam.pos.x -= (float)sin(m_bucketMove * D3DX_PI)*30.0f;
			m_BucketParam.pos.y -= (float)sin(m_bucketMove * D3DX_PI)*5.0f;
			if (m_BucketParam.pos.x >SCREEN_CENTER_X + 200.0f)
				m_bucketMove += 0.01f;

			if (m_BucketParam.pos.x <= SCREEN_CENTER_X + 200.0f)
			{
				m_BucketParam.pos.x = SCREEN_CENTER_X + 200.0f;
				m_BucketParam.pos.y = SCREEN_CENTER_Y + 50.0f;

				m_spriteTitle[NUM_BUCKET]->m_scale -= D3DXVECTOR2(0.05f, 0.05f);
				//スプラッシュ 
				m_spriteTitle[NUM_SPLASH]->m_scale += D3DXVECTOR2(0.05f, 0.05f);
				if (m_spriteTitle[NUM_BUCKET]->m_scale.x <= 0.0f && m_spriteTitle[NUM_BUCKET]->m_scale.y <= 0.0f)
				{
					m_spriteTitle[NUM_BUCKET]->m_color = D3DCOLOR_ARGB(0, 255, 255, 255);
					m_bucketSet = true;
				}
			}
			m_spriteTitle[NUM_BUCKET]->m_pos = m_BucketParam.pos;
		}
		else
		{
			//ゲームロゴ処
			for (int i = 0; i < 5; i++)
			{
				if (!m_gamelogoflag)
				{
					m_pSpriteGamaeLogo[i]->m_scale = m_gamelogoScaleCnt;
				}
				else
				{
					m_gamelogNo = i;
					if (m_gamelogNo >= 2)
					{
						int j = m_gamelogNo;
						j %= 2;
						if (j == 0)
						{
							m_gamelogNo = 0;
						}
						else
						{
							m_gamelogNo = 1;
						}
					}

					switch (m_gamelogNo)
					{
					case 0:
						m_pSpriteGamaeLogo[i]->m_pos.y -= (float)sin(m_gamelogoMove * D3DX_PI);
						break;
					case 1:
						m_pSpriteGamaeLogo[i]->m_pos.y += (float)sin(m_gamelogoMove * D3DX_PI);
						break;
					}
					m_gamelogoMove += 0.005f;
				}
			}
			m_gamelogoScaleCnt += D3DXVECTOR2(0.01f, 0.01f);

			if (m_gamelogoScaleCnt.x > 1.0f &&m_gamelogoScaleCnt.y > 1.0f)
			{
				m_gamelogoScaleCnt = D3DXVECTOR2(1.0f, 1.0f);
				if (m_gamelogoCnt < 0)
				{
					m_gamelogoflag = true;
					m_gamelogoCnt = 10;
				}
				m_gamelogoCnt--;
			}
		}
		//ボタンの処理
		if (m_buttomCount < 0)
		{
			m_spriteTitle[NUM_BUTTON]->m_color = D3DCOLOR_ARGB(255, 255, 255, 255);
			m_switchButton = true;
		}
		else if (m_buttomCount > MAX_BUTTON_COUNT)
		{
			m_spriteTitle[NUM_BUTTON]->m_color = D3DCOLOR_ARGB(0, 255, 255, 255);
			m_switchButton = false;
		}

		if (!m_switchButton)
		{
			m_buttomCount--;
		}
		else
		{
			m_buttomCount++;
		}
		if (GetPadButton(0, PAD_FLAG_TRIGGER, PAD_A) || DIGetKeyTrigger(DIK_RETURN) && !pushflag)
		{
			StartFadeOut();
			pushflag = true;
		}

		if (EndFadeOut())
		{
			g_pScene->ChangeScene(SCENE_SELECT);
		}
	}
}
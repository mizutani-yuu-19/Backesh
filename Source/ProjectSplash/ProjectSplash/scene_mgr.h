///////////////////////////////////////////////////////////
//
// ÉVÅ[Éìä«óù
//
//////////////////////////////////////////////////////////

#ifndef _SCENE_MGR_H_
#define _SCENE_MGR_H_

#include "main.h"

enum ESceneName{
	SCENE_TITLE,
	SCENE_SELECT,
	SCENE_GAME,
	SCENE_NONE,
};

class CBaseScene
{
public:
	CBaseScene();
	~CBaseScene();
	virtual void Init() {};
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
private:
};

class CScene
{
public:
	CScene();
	~CScene();

	void Init();
	void Uninit();
	void Update();
	void Draw();
	void ChangeScene(int nextScene);
private:
	CBaseScene *m_scene;
	int m_nextScene;
protected:
};
extern CScene	*g_pScene;
#endif

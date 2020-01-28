//////////////////////////////////////////
//
// debug.h
// デバッグに使えそうなもの
//
/////////////////////////////////////////
#ifndef _DEBUG_H_
#define _DEBUG_H_

//------------インクルード--------------
#include "main.h"


HRESULT InitDebug();
void DrawDebugLine(D3DXVECTOR2 vertex1, D3DXVECTOR2 vertex2);
void DrawDebugBox(D3DXVECTOR3 pos, D3DXVECTOR3 size);

#endif // !_DEBUG_H_

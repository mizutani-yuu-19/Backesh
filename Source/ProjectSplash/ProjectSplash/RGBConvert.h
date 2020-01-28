// RGB•ÏŠ·
#pragma once

#include "main.h"

typedef struct {
	int H;
	int S;
	int V;
	int A;
}HSVA;

D3DCOLOR HSVConvert(int H, int S, int V, int A);
D3DCOLOR HSVConvert(HSVA);

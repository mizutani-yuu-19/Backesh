#include "RGBConvert.h"
//
// HSVÅ®RGBïœä∑
// HÇÕ360ÅASÇÕ100ÅAVÇÕ100ÅAAÇÕ255
//
D3DCOLOR HSVConvert(int H, int S, int V, int A) {
	int R;
	int G;
	int B;
	float MAX;
	float MIN;

	H %= 360;

	if (V < 50) {
		MAX = 2.55*(V + V*(S / 100));
		MIN = 2.55*(V - V*(S / 100));
	}
	else {
		MAX = 2.55*(V + (100 - V)*(S / 100));
		MIN = 2.55*(V - (100 - V)*(S / 100));
	}
	if (H < 60) {
		R = MAX;
		G = (H / 60.0f) *(MAX - MIN) + MIN;
		B = MIN;
	}
	else if (H < 120) {
		R = ((120 - H) / 60.0f) *(MAX - MIN) + MIN;
		G = MAX;
		B = MIN;
	}
	else if (H < 180) {
		R = MIN;
		G = MAX;
		B = ((H - 120) / 60.0f) *(MAX - MIN) + MIN;
	}
	else if (H < 240) {
		R = MIN;
		G = ((240 - H) / 60.0f) *(MAX - MIN) + MIN;
		B = MAX;
	}
	else if (H < 300) {
		R = ((H - 240) / 60.0f) *(MAX - MIN) + MIN;
		G = MIN;
		B = MAX;
	}
	else if (H < 361) {
		R = MAX;
		G = MIN;
		B = ((360 - H) / 60.0f) *(MAX - MIN) + MIN;
	}
	return D3DCOLOR_ARGB(A, R, G, B);
}

D3DCOLOR HSVConvert(HSVA hsva) {
	return HSVConvert(hsva.H, hsva.S, hsva.V, hsva.A);
}
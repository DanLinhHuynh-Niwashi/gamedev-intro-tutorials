#include "Koopa.h"


CKoopa::CKoopa(float x, float y, float vx) :CGameObject(x, y)
{
	this->vx = vx;
};

void CKoopa::Update(DWORD dt)
{
	x += vx * dt;

	int BackBufferWidth = CGame::GetInstance()->GetBackBufferWidth();
	if (x <= 0 || x >= BackBufferWidth) {

		vx = -vx;

		if (x <= 0)
		{
			x = 0;
		}
		else if (x >= BackBufferWidth)
		{
			x = (float)(BackBufferWidth);
		}
	}
}

void CKoopa::Render()
{
	LPANIMATION ani;

	//[RED FLAG][TODO]: Student needs to think about how to associate this animation/asset to Mario!!
	if (vx > 0) ani = CAnimations::GetInstance()->Get(KOOPA_WALK_RIGHT_ID);
	else ani = CAnimations::GetInstance()->Get(KOOPA_WALK_LEFT_ID);

	ani->Render(x, y);
}

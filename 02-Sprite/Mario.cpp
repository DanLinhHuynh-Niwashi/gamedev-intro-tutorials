#include "Game.h"
#include "Mario.h"

CMario::CMario(float x, float y, float vx):CGameObject(x, y)
{
	this->vx = vx;
};

void CMario::Update(DWORD dt)
{
	x += vx * dt;

	int BackBufferWidth = CGame::GetInstance()->GetBackBufferWidth();
	if (x <= 0)
	{
		x = 0;
	}
	else if (x >= BackBufferWidth - MARIO_WIDTH)
	{
		x = (float)(BackBufferWidth - MARIO_WIDTH);
	}

}

void CMario::Render()
{
	LPANIMATION ani;

	//[RED FLAG][TODO]: Student needs to think about how to associate this animation/asset to Mario!!
	if (vx > 0) ani = CAnimations::GetInstance()->Get(MARIO_RIGHT_WALK_ID);
	else if (vx < 0) ani = CAnimations::GetInstance()->Get(MARIO_LEFT_WALK_ID);
	else ani = CAnimations::GetInstance()->Get(MARIO_IDLE_ID);

	ani->Render(x, y);
}

void CBrick::Render() {

	LPANIMATION ani = CAnimations::GetInstance()->Get(710);

	ani->Render(x, y);

}
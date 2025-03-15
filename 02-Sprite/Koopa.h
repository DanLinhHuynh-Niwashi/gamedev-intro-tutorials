#pragma once
#include "GameObject.h"

#include "Animation.h"
#include "Animations.h"
#define KOOPA_WALK_LEFT_ID 600
#define KOOPA_WALK_RIGHT_ID 601
class CKoopa :
    public CGameObject
{
protected:
	float vx;
public:
	CKoopa(float x, float y, float vx);
	void Update(DWORD dt);
	void Render();
};


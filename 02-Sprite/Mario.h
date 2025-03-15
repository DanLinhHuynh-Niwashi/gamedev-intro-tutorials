#pragma once

#include "GameObject.h"

#include "Animation.h"
#include "Animations.h"

#define MARIO_WIDTH 14
#define MARIO_LEFT_WALK_ID 510
#define MARIO_RIGHT_WALK_ID 500
#define MARIO_IDLE_ID 520

class CBrick : public CGameObject {
public: 
	CBrick(float x, float y) : CGameObject(x, y) {}
	void Render();
	void Update(DWORD dt) {}
};

class CMario : public CGameObject
{
protected:
	float vx;
public:
	CMario(float x, float y, float vx);
	void SetVx(float vx) {
		this->vx = vx;
	}
	void Update(DWORD dt);
	void Render();
};



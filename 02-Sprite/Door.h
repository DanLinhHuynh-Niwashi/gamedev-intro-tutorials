#pragma once
#include "GameObject.h"

#include "Animation.h"
#include "Animations.h"

#define DOOR_CLOSE_ID 800
#define DOOR_OPEN_ID 801

#define DOOR_CLOSING_ID 810
#define DOOR_OPENING_ID 811

#define DOOR_WIDTH 17.0f
class CDoor :
    public CGameObject
{
	enum State{
		CLOSE,
		CLOSING,
		OPEN,
		OPENING
	};
	ULONGLONG lastUpdateTime = 0;
	bool isCollidedWithMario = false;
	State state = CLOSE;
public:
	CDoor(float x, float y) : CGameObject(x, y) {}
	bool isCollidedWith(CGameObject * obj);

	void OnCollidedWithMario();
	void OnNoCollision();
	void Render();
	void SetIsCollidedWithMario(bool collided) {
		this->isCollidedWithMario = collided;
	}
	void Update(DWORD dt) {}
};


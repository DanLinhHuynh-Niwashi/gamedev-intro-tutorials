#include "Door.h"
#include "Mario.h"
void CDoor::Render() {
	
	LPANIMATION ani = CAnimations::GetInstance()->Get(DOOR_CLOSE_ID);
	switch (state) {
	case OPEN:
		ani = CAnimations::GetInstance()->Get(DOOR_OPEN_ID);
		break;
	case CLOSE:
		ani = CAnimations::GetInstance()->Get(DOOR_CLOSE_ID);
		break;
	case OPENING:
		ani = CAnimations::GetInstance()->Get(DOOR_OPENING_ID);
		break;
	case CLOSING:
		ani = CAnimations::GetInstance()->Get(DOOR_CLOSING_ID);
		break;
	}
	
	ani->Render(x, y);

	if (isCollidedWithMario) {
		OnCollidedWithMario();
	} else OnNoCollision();
}

bool CDoor::isCollidedWith(CGameObject* obj)
{
	if (dynamic_cast<CMario*>(obj)) {
		int halfWidthA = MARIO_WIDTH / 2;
		int halfWidthB = DOOR_WIDTH / 2;

		return (std::abs(this->GetX() - obj->GetX()) < (halfWidthA + halfWidthB));
	}
}

void CDoor::OnCollidedWithMario() {
	if (state == OPEN) return;
	if (state == CLOSING || state == CLOSE) {
		state = OPENING;
		lastUpdateTime = GetTickCount64();
		return;
	}

	ULONGLONG now = GetTickCount64();
	if (state == OPENING && now - lastUpdateTime >= 500) {
		state = OPEN;
		return;
	}
}
void CDoor::OnNoCollision() {
	if (state == CLOSE) return;
	if (state == OPENING || state == OPEN) {
		state = CLOSING;
		lastUpdateTime = GetTickCount64();
		return;
	}

	ULONGLONG now = GetTickCount64();
	if (state == CLOSING && now - lastUpdateTime >= 500) {
		state = CLOSE;
		return;
	}
}
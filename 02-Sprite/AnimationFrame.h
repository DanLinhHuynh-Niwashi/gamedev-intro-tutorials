#pragma once

#include "Sprite.h"
#include <unordered_map>

class CAnimationFrame
{
	unordered_map<LPSPRITE, pair<float, float>> sprites;
	DWORD time;

public:
	CAnimationFrame(unordered_map<LPSPRITE, pair<float, float>> sprites, int time)
	{
		this->sprites = sprites;
		this->time = time;
	}
	DWORD GetTime() { return time; }

	void Render(float x, float y) {
		for (auto it : sprites)
		{
			it.first->Draw(x + it.second.first, y + it.second.second);
		}
	}
};


typedef CAnimationFrame* LPANIMATION_FRAME;


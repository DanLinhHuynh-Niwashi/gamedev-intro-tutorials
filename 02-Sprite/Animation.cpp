#include "Animation.h"

void CAnimation::Add(int spriteId, DWORD time)
{
	int t = time;
	if (time == 0) t = this->defaultTime;

	unordered_map<LPSPRITE, pair<float, float>> spriteList;
	pair<float, float> offset = make_pair(0.0f, 0.0f);
	spriteList.insert({ CSprites::GetInstance()->Get(spriteId), offset});

	LPANIMATION_FRAME frame = new CAnimationFrame(spriteList, t);
	frames.push_back(frame);
}

void CAnimation::Add(vector<int> spriteIds, vector<pair<float, float>> offsets, DWORD time)
{
	int t = time;
	if (time == 0) t = this->defaultTime;

	unordered_map<LPSPRITE, pair<float, float>> spriteList;
	for (int i = 0; i < spriteIds.size(); i++) {
		spriteList.insert({ CSprites::GetInstance()->Get(spriteIds[i]), offsets[i]});
	}

	LPANIMATION_FRAME frame = new CAnimationFrame(spriteList, t);
	frames.push_back(frame);
}

void CAnimation::Render(float x, float y)
{
	ULONGLONG now = GetTickCount64();
	if (currentFrame == -1)
	{
		currentFrame = 0;
		lastFrameTime = now;
	}
	else
	{
		DWORD t = frames[currentFrame]->GetTime();
		if (now - lastFrameTime > t)
		{
			currentFrame++;
			lastFrameTime = now;
			if (currentFrame == frames.size()) currentFrame = 0;
			//DebugOut(L"now: %d, lastFrameTime: %d, t: %d\n", now, lastFrameTime, t);
		}

	}

	frames[currentFrame]->Render(x, y);
}


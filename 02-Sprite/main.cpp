/* =============================================================
	INTRODUCTION TO GAME PROGRAMMING SE102
	
	SAMPLE 02 - SPRITE AND ANIMATION

	This sample illustrates how to:

		1/ Render a sprite (within a sprite sheet)
		2/ How to manage sprites/animations in a game
		3/ Enhance CGameObject with sprite animation
================================================================ */

#include <Windows.h>
#include <d3d10.h>
#include <d3dx10.h>
#include <unordered_map>

using namespace std;

#include "debug.h"
#include "Game.h"
#include "Textures.h"

#include "Sprite.h"
#include "Sprites.h"

#include "Animation.h"
#include "Animations.h"

#include "Koopa.h"
#include "Mario.h"
#include "Door.h"

#define WINDOW_CLASS_NAME L"SampleWindow"
#define MAIN_WINDOW_TITLE L"02 - Sprite animation"
#define WINDOW_ICON_PATH L"mario.ico"

#define BACKGROUND_COLOR D3DXCOLOR(200.0f/255, 200.0f/255, 255.0f/255,0.0f)
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#define ID_TEX_MARIO 0
#define ID_TEX_ENEMY 10
#define ID_TEX_MISC 20

#define TEXTURES_DIR L"textures"
#define TEXTURE_PATH_MARIO TEXTURES_DIR "\\mario_transparent.png"
#define TEXTURE_PATH_MISC TEXTURES_DIR "\\misc_transparent.png"
#define TEXTURE_PATH_ENEMIES TEXTURES_DIR "\\enemies_transparent.png"

CKoopa* koopa;
#define KOOPA_START_X 130.0f
#define KOOPA_START_Y 100.0f
#define KOOPA_START_VX 0.05f

CMario *mario;
#define MARIO_START_X 10.0f
#define MARIO_START_Y 130.0f
#define MARIO_START_VX 0.0f

CDoor* door;
#define DOOR_START_X 130.0f
#define DOOR_START_Y 120.0f

CBrick *brick;

vector<CGameObject *> objects;
void KeyDownResponse(WPARAM wParam);
void KeyUpResponse(WPARAM wParam);
LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_KEYDOWN:
		KeyDownResponse(wParam);
		break;
	case WM_KEYUP:
		KeyUpResponse(wParam);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

void KeyDownResponse(WPARAM wParam) {
	switch (wParam) {
	case 'A':  // Left movement
		mario->SetVx(-0.1f);
		break;
	case 'D':  // Right movement
		mario->SetVx(0.1f);
		break;
	default:
		break;
	}
}

void KeyUpResponse(WPARAM wParam) {
	switch (wParam) {
	case 'A':  // Left movement
		mario->SetVx(0.0f);
		break;
	case 'D':  // Right movement
		mario->SetVx(0.0f);
		break;
	default:
		break;
	}
}

/*
	Load all game resources 
	In this example: load textures, sprites, animations and mario object
*/
void LoadResources()
{
	CTextures * textures = CTextures::GetInstance();

	textures->Add(ID_TEX_MARIO, TEXTURE_PATH_MARIO);
	textures->Add(ID_TEX_ENEMY, TEXTURE_PATH_ENEMIES);
	textures->Add(ID_TEX_MISC, TEXTURE_PATH_MISC);


	CSprites * sprites = CSprites::GetInstance();
	
	LPTEXTURE texMario = textures->Get(ID_TEX_MARIO);

	// readline => id, left, top, right 

	sprites->Add(500, 246, 154, 259, 181, texMario);
	sprites->Add(510, 275, 154, 290, 181, texMario);
	sprites->Add(520, 304, 154, 321, 181, texMario);

	sprites->Add(501, 186, 154, 200, 181, texMario);
	sprites->Add(511, 155, 154, 171, 181, texMario);
	sprites->Add(521, 125, 154, 141, 181, texMario);

	sprites->Add(502, 215, 154, 231, 181, texMario);
	CAnimations * animations = CAnimations::GetInstance();
	LPANIMATION ani;

	ani = new CAnimation(100);
	ani->Add(500);
	ani->Add(510);
	ani->Add(520);
	animations->Add(MARIO_RIGHT_WALK_ID, ani);

	ani = new CAnimation(100);
	ani->Add(501);
	ani->Add(511);
	ani->Add(521);
	animations->Add(MARIO_LEFT_WALK_ID, ani);

	ani = new CAnimation(1000);
	ani->Add(502);
	animations->Add(MARIO_IDLE_ID, ani);


	LPTEXTURE texMisc = textures->Get(ID_TEX_MISC);
	sprites->Add(20001, 300, 135, 317, 152, texMisc);
	sprites->Add(20002, 318, 135, 335, 152, texMisc);
	sprites->Add(20003, 336, 135, 353, 152, texMisc);
	sprites->Add(20004, 354, 135, 371, 152, texMisc);

	ani = new CAnimation(100);
	ani->Add(20001,1000);
	ani->Add(20002);
	ani->Add(20003);
	ani->Add(20004);
	animations->Add(710, ani);
	
	//Door
	sprites->Add(8001, 281, 98, 298, 114, texMisc);
	sprites->Add(8002, 281, 117, 298, 133, texMisc);

	sprites->Add(8011, 281, 134, 298, 150, texMisc);
	sprites->Add(8012, 281, 153, 298, 169, texMisc);

	sprites->Add(8021, 281, 170, 298, 186, texMisc);
	sprites->Add(8022, 281, 189, 298, 205, texMisc);

	sprites->Add(8031, 281, 206, 298, 222, texMisc);
	sprites->Add(8032, 281, 225, 298, 241, texMisc);

	ani = new CAnimation(1000);
	vector<int>* spriteIds = new vector<int>({8001, 8002});
	vector <pair<float, float>>* offsets = new vector <pair<float, float>>({ make_pair(0.0f, 0.0f) , make_pair(0.0f, 16.0f) });
	ani->Add(*spriteIds, *offsets);
	animations->Add(DOOR_CLOSE_ID, ani);


	ani = new CAnimation(1000);
	spriteIds = new vector<int>({ 8031, 8032 });
	ani->Add(*spriteIds, *offsets);
	animations->Add(DOOR_OPEN_ID, ani);

	ani = new CAnimation(100);
	spriteIds = new vector<int>({ 8031, 8032 });
	ani->Add(*spriteIds, *offsets);
	spriteIds = new vector<int>({ 8021, 8022 });
	ani->Add(*spriteIds, *offsets);
	spriteIds = new vector<int>({ 8011, 8012 });
	ani->Add(*spriteIds, *offsets);
	spriteIds = new vector<int>({ 8001, 8002 });
	ani->Add(*spriteIds, *offsets, 1000);
	animations->Add(DOOR_CLOSING_ID, ani);

	ani = new CAnimation(100);
	spriteIds = new vector<int>({ 8001, 8002 });
	ani->Add(*spriteIds, *offsets);
	spriteIds = new vector<int>({ 8011, 8012 });
	ani->Add(*spriteIds, *offsets);
	spriteIds = new vector<int>({ 8021, 8022 });
	ani->Add(*spriteIds, *offsets);
	spriteIds = new vector<int>({ 8031, 8032 });
	ani->Add(*spriteIds, *offsets, 1000);
	animations->Add(DOOR_OPENING_ID, ani);

	LPTEXTURE texEnemy = textures->Get(ID_TEX_ENEMY);

	// readline => id, left, top, right 

	sprites->Add(610, 179, 124, 203, 156, texEnemy);
	sprites->Add(620, 209, 124, 233, 156, texEnemy);

	sprites->Add(611, 327, 124, 351, 156, texEnemy);
	sprites->Add(621, 296, 124, 320, 156, texEnemy);

	ani = new CAnimation(200);
	ani->Add(610);
	ani->Add(620);
	animations->Add(KOOPA_WALK_LEFT_ID, ani);

	ani = new CAnimation(200);
	ani->Add(611);
	ani->Add(621);
	animations->Add(KOOPA_WALK_RIGHT_ID, ani);

	koopa = new CKoopa(KOOPA_START_X, KOOPA_START_Y, KOOPA_START_VX);
	mario = new CMario(MARIO_START_X, MARIO_START_Y, MARIO_START_VX);
	door = new CDoor(DOOR_START_X, DOOR_START_Y);
	brick = new CBrick(100.0f, 70.0f);

	objects.push_back(koopa);
	objects.push_back(mario);
	objects.push_back(door);
	objects.push_back(brick);
}

/*
	Update world status for this frame
	dt: time period between beginning of last frame and beginning of this frame
*/
void Update(DWORD dt)
{
	if (door->isCollidedWith(mario)) {
		door->SetIsCollidedWithMario(true);
	}
	else {
		door->SetIsCollidedWithMario(false);
	}

	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt);
	}
	
	
}

void Render()
{
	CGame* g = CGame::GetInstance();

	ID3D10Device* pD3DDevice = g->GetDirect3DDevice();
	IDXGISwapChain* pSwapChain = g->GetSwapChain();
	ID3D10RenderTargetView* pRenderTargetView = g->GetRenderTargetView();
	ID3DX10Sprite* spriteHandler = g->GetSpriteHandler();

	if (pD3DDevice != NULL)
	{
		// clear the background 
		pD3DDevice->ClearRenderTargetView(pRenderTargetView, BACKGROUND_COLOR);

		spriteHandler->Begin(D3DX10_SPRITE_SORT_TEXTURE);

		// Use Alpha blending for transparent sprites
		FLOAT NewBlendFactor[4] = { 0,0,0,0 };
		pD3DDevice->OMSetBlendState(g->GetAlphaBlending(), NewBlendFactor, 0xffffffff);

		for (int i = 0; i < objects.size(); i++)
		{
			objects[i]->Render();
		}

		// Uncomment this line to see how to draw a porttion of a texture  
		//g->Draw(10, 10, texMisc, 300, 117, 316, 133);


		spriteHandler->End();
		pSwapChain->Present(0, 0);
	}
}

HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = (HICON)LoadImage(hInstance, WINDOW_ICON_PATH, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	HWND hWnd =
		CreateWindow(
			WINDOW_CLASS_NAME,
			MAIN_WINDOW_TITLE,
			WS_OVERLAPPEDWINDOW, // WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			ScreenWidth,
			ScreenHeight,
			NULL,
			NULL,
			hInstance,
			NULL);

	if (!hWnd)
	{
		DWORD ErrCode = GetLastError();
		DebugOut(L"[ERROR] CreateWindow failed! ErrCode: %d\nAt: %s %d \n", ErrCode, _W(__FILE__), __LINE__);
		return 0;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	SetDebugWindow(hWnd);

	return hWnd;
}

int Run()
{
	MSG msg;
	int done = 0;
	ULONGLONG frameStart = GetTickCount64();
	ULONGLONG tickPerFrame = 1000 / MAX_FRAME_RATE;

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) done = 1;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		ULONGLONG now = GetTickCount64();

		// dt: the time between (beginning of last frame) and now
		// this frame: the frame we are about to render
		ULONGLONG dt = now - frameStart;

		if (dt >= tickPerFrame)
		{
			frameStart = now;
			Update((DWORD)dt);
			Render();
		}
		else
			Sleep((DWORD)(tickPerFrame - dt));
	}

	return 1;
}

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow
) {
	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);

	CGame *game = CGame::GetInstance();
	game->Init(hWnd);

	LoadResources();

	SetWindowPos(hWnd, 0, 0, 0, SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);

	Run();

	return 0;
}
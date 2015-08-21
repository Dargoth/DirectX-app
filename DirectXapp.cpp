// Win32Project1.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "DirectXapp.h"

#define MAX_LOADSTRING 100

// Global Variables:
D3DDISPLAYMODE Display;
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

LPDIRECT3D9 pDirect3D = NULL;
LPDIRECT3DDEVICE9 pDirect3DDevice = NULL;

LPD3DXFONT pFont = NULL;
RECT TextRect;

LPDIRECT3DTEXTURE9 pTexture001 = NULL;
LPDIRECT3DTEXTURE9 pTexture002 = NULL;

struct CUSTOMVERTEX {
	FLOAT X, Y, Z;
	FLOAT nx, ny, nz;
	FLOAT tu, tv;
	//DWORD color;
};
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1)

LPDIRECT3DVERTEXBUFFER9 pVertexBuffer = NULL;
LPDIRECT3DINDEXBUFFER9 pIndexBuffer = NULL;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

//InitialDirect3D
HRESULT InitialDirect3D(HWND hwnd){
	if ((pDirect3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
		return E_FAIL;
	if (FAILED(pDirect3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &Display)))
		return E_FAIL;
	D3DPRESENT_PARAMETERS Direct3DParameters;
	ZeroMemory(&Direct3DParameters, sizeof(Direct3DParameters));
	Direct3DParameters.Windowed = FALSE;
	Direct3DParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	Direct3DParameters.BackBufferFormat = Display.Format;
	Direct3DParameters.EnableAutoDepthStencil = TRUE;
	Direct3DParameters.AutoDepthStencilFormat = D3DFMT_D16;
	//fullscreen-----------------------------
	Direct3DParameters.BackBufferWidth = GetSystemMetrics(SM_CXSCREEN);
	Direct3DParameters.BackBufferHeight = GetSystemMetrics(SM_CYSCREEN);
	Direct3DParameters.BackBufferCount = 3;
	Direct3DParameters.FullScreen_RefreshRateInHz = Display.RefreshRate;
	//---------------------------------------
	if (FAILED(pDirect3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING, &Direct3DParameters, &pDirect3DDevice)))
		return E_FAIL;
	//отключить отсечение Direct3D
	pDirect3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	//создание буфера глубины
	pDirect3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	//альфа-блендинг
	pDirect3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDirect3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDirect3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	return S_OK;
}

//InitialObject
HRESULT InitialObject(){
	CUSTOMVERTEX Vertexes[]{
	//		x	  y		z		nx		ny	 nz		tu	  tv
		{ 1.0f, -1.0f, -1.0f,	0.0f, 0.0f, -1.0f,	1.0f, 1.0f },//A1
		{ 1.0f, 1.0f, -1.0f,	0.0f, 0.0f, -1.0f,	0.0f, 1.0f }, //B
		{ -1.0f, 1.0f, -1.0f,	0.0f, 0.0f, -1.0f,	0.0f, 0.0f }, //C
		{ -1.0f, -1.0f, -1.0f,	0.0f, 0.0f, -1.0f,	1.0f, 0.0f }, //D
		//////////////////
		{ -1.0f, -1.0f, -1.0f,	-1.0f, 0.0f, 0.0f,	1.0f, 1.0f }, //A2
		{ -1.0f, 1.0f, -1.0f,	-1.0f, 0.0f, 0.0f,	0.0f, 1.0f }, //B
		{ -1.0f, 1.0f, 1.0f,	-1.0f, 0.0f, 0.0f,	0.0f, 0.0f }, //C
		{ -1.0f, -1.0f, 1.0f,	-1.0f, 0.0f, 0.0f,	1.0f, 0.0f }, //D
		/////////////////////////////////////////////////
		{ -1.0f, -1.0f, 1.0f,	0.0f, 0.0f, 1.0f,	1.0f, 1.0f }, //A3
		{ -1.0f, 1.0f, 1.0f,	0.0f, 0.0f, 1.0f,	0.0f, 1.0f }, //B
		{ 1.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f }, //C
		{ 1.0f, -1.0f, 1.0f,	0.0f, 0.0f, 1.0f,	1.0f, 0.0f }, //D
		/////////////////////////////////////////////////
		{ 1.0f, -1.0f, 1.0f,	1.0f, 0.0f, 0.0f,	1.0f, 1.0f }, //A4
		{ 1.0f, 1.0f, 1.0f,		1.0f, 0.0f, 0.0f,	0.0f, 1.0f }, //B
		{ 1.0f, 1.0f, -1.0f,	1.0f, 0.0f, 0.0f,	0.0f, 0.0f }, //C
		{ 1.0f, -1.0f, -1.0f,	1.0f, 0.0f, 0.0f,	1.0f, 0.0f }, //D
		///////////////////////////////////////////////
		{ 1.0f, -1.0f, -1.0f,	0.0f, -1.0f, 0.0f,	1.0f, 1.0f }, //A5
		{ -1.0f, -1.0f, -1.0f,	0.0f, -1.0f, 0.0f,	0.0f, 1.0f }, //B
		{ -1.0f, -1.0f, 1.0f,	0.0f, -1.0f, 0.0f,	0.0f, 0.0f }, //C
		{ 1.0f, -1.0f, 1.0f,	0.0f, -1.0f, 0.0f,	1.0f, 0.0f }, //D
		////////////////////
		{ 1.0f, 1.0f, 1.0f,		0.0f, 1.0f, 0.0f,	1.0f, 1.0f }, //A
		{ -1.0f, 1.0f, 1.0f,	0.0f, 1.0f, 0.0f,	0.0f, 1.0f }, //B
		{ -1.0f, 1.0f, -1.0f,	0.0f, 1.0f, 0.0f,	0.0f, 0.0f }, //C
		{ 1.0f, 1.0f, -1.0f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f }, //D
	};
	const unsigned short Index[] = {
		0, 1, 2, 2, 3, 0,
		4, 5, 6, 6, 7, 4,
		8, 9, 10, 10, 11, 8,
		12, 13, 14, 14, 15, 12,
		16, 17, 18, 18, 19, 16,
		20, 21, 22, 22, 23, 20,
	};
	//Load texture
	if (FAILED(D3DXCreateTextureFromFile(pDirect3DDevice, L"stone_2.jpg", &pTexture001)))
		return E_FAIL;
	if (FAILED(D3DXCreateTextureFromFileEx(pDirect3DDevice, L"reshetka.jpg", 0, 0, 0, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT,
		D3DX_DEFAULT, D3DX_DEFAULT, 0xFF000000, NULL, NULL, &pTexture002)))
		return E_FAIL;

	//create VertexBuffer
	if (FAILED(pDirect3DDevice->CreateVertexBuffer(36 * sizeof(CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT, &pVertexBuffer, NULL)))
		return E_FAIL;
	VOID* pBV;
	//lock VertexBuffer
	if (FAILED(pVertexBuffer->Lock(0, sizeof(Vertexes), (void**)&pBV, 0)))
		return E_FAIL;
	//memcopy
	memcpy(pBV, Vertexes, sizeof(Vertexes));
	//unlock VertexBuffer
	pVertexBuffer->Unlock();
	//create IndexBuffer
	if (FAILED(pDirect3DDevice->CreateIndexBuffer(36 * sizeof(Index), 0, D3DFMT_INDEX16,
		D3DPOOL_DEFAULT, &pIndexBuffer, NULL)))
		return E_FAIL;
	VOID* pBI;
	//lock IndexBuffer
	if (FAILED(pIndexBuffer->Lock(0, sizeof(Index), (void**)&pBI, 0)))
		return E_FAIL;
	//memcopy
	memcpy(pBI, Index, sizeof(Index));
	//unlock IndexBuffer
	pIndexBuffer->Unlock();
	D3DXCreateFont(pDirect3DDevice, 30, 10, 0, 0, FALSE, UNICODE, 0, 0, DEFAULT_PITCH | FF_MODERN, L"Arial", &pFont);
	return S_OK;
}

//Matrix
VOID Matrix(){
	D3DXMATRIX MatrixWorld;
	D3DXMATRIX MatrixWorldX;
	D3DXMATRIX MatrixWorldY;
	D3DXMATRIX MatrixView;
	D3DXMATRIX MatrixProjection;

	//MatrixWorld
	UINT Time = timeGetTime() % 5000;
	FLOAT Angle = Time * (2.0f * D3DX_PI) / 5000.0f;
	D3DXMatrixRotationX(&MatrixWorldX, Angle);
	D3DXMatrixRotationY(&MatrixWorldY, Angle);
	D3DXMatrixMultiply(&MatrixWorld, &MatrixWorldX, &MatrixWorldY);
	pDirect3DDevice->SetTransform(D3DTS_WORLD, &MatrixWorld);
	//MatrixView
	D3DXMatrixLookAtLH(&MatrixView, &D3DXVECTOR3(0.0f, 0.0f, -8.0f), &D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	pDirect3DDevice->SetTransform(D3DTS_VIEW, &MatrixView);
	//MatrixProjection
	D3DXMatrixPerspectiveFovLH(&MatrixProjection, D3DX_PI / 4, (float)  Display.Width / Display.Height, 1.0f, 100.0f);
	pDirect3DDevice->SetTransform(D3DTS_PROJECTION, &MatrixProjection);
}

//Light Material
VOID LightMaterial(){
	D3DMATERIAL9 Material;
	D3DLIGHT9 Light;

	ZeroMemory(&Material, sizeof(D3DMATERIAL9));

	Material.Diffuse.r = Material.Ambient.r = 1.0f;
	Material.Diffuse.g = Material.Ambient.g = 1.0f;
	Material.Diffuse.b = Material.Ambient.b = 1.0f;
	Material.Diffuse.a = Material.Ambient.a = 1.0f;
	pDirect3DDevice->SetMaterial(&Material);

	//Устанавливаем свет
	ZeroMemory(&Light, sizeof(D3DLIGHT9));
	Light.Type = D3DLIGHT_DIRECTIONAL;
	Light.Diffuse.r = 1.0f;
	Light.Diffuse.g = 1.0f;
	Light.Diffuse.b = 1.0f;
	Light.Range = 1000.0f;

	//нормализация...
	D3DXVECTOR3 LightDir(0.0f, 0.0f, 1.0f);
	D3DXVec3Normalize((D3DXVECTOR3*)&Light.Direction, &LightDir);

	pDirect3DDevice->SetLight(0, &Light);
	pDirect3DDevice->LightEnable(0, TRUE);

	pDirect3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	pDirect3DDevice->SetRenderState(D3DRS_AMBIENT, 0);
}

VOID DrawMyText(LPDIRECT3DDEVICE9 pDirect3DDevice, LPCWSTR MyText, int x, int y, int x1, int y1, D3DCOLOR MyColor){
	//text rect pos
	TextRect.left = x;
	TextRect.top = y;
	TextRect.right = x1;
	TextRect.bottom = y1;
	//Draw
	pFont->DrawText(0, MyText, -1, &TextRect, DT_WORDBREAK, MyColor);
}

VOID RenderingDirect3D(){
	if (pDirect3DDevice == NULL)
		return;
	pDirect3DDevice -> Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(60, 100, 150), 1.0f, 0);
	pDirect3DDevice->BeginScene();
	//здесь происходит прорисовка сцены
	LightMaterial();
	Matrix();
	pDirect3DDevice->SetStreamSource(0, pVertexBuffer, 0, sizeof(CUSTOMVERTEX));
	pDirect3DDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
	pDirect3DDevice->SetIndices(pIndexBuffer);
	//texturing----------------------------------------
	pDirect3DDevice->SetTexture(0, pTexture001);
	pDirect3DDevice->SetTexture(1, pTexture002);
	pDirect3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDirect3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	pDirect3DDevice->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 0);
	pDirect3DDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDirect3DDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
	//-------------------------------------------------
	pDirect3DDevice-> DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 36, 0, 12);
	DrawMyText(pDirect3DDevice, L"Lorem ipsum dolor sit amet", 50, 50, 200, 200, D3DCOLOR_RGBA (50, 50, 50, 230));
	pDirect3DDevice->EndScene();
	pDirect3DDevice->Present(NULL, NULL, NULL, NULL);
}

VOID ReleaseDirect3D(){
	if (pTexture001) pTexture002->Release();
	if (pTexture001) pTexture001->Release();
	if (pFont) pFont->Release();
	if (pIndexBuffer) pIndexBuffer->Release();
	if (pVertexBuffer) pVertexBuffer->Release();
	if (pDirect3DDevice) pDirect3DDevice->Release();
	if (pDirect3D) pDirect3D->Release();
}

//InitialVertexBuffer
/*HRESULT InitialVertexBuffer(){
	CUSTOMVERTEX Vertexes[]{
		{ 1.0f, -1.0f, 1.0f, 0x00000fff }, //A
		{ -1.0f, -1.0f, 1.0f, 0x00000fff }, //B
		{ -1.0f, 1.0f, 1.0f, 0x00000fff }, //C

		{ -1.0f, 1.0f, 1.0f, 0xfff00000 }, //C
		{ 1.0f, 1.0f, 1.0f, 0xfff00000 }, //D
		{ 1.0f, -1.0f, 1.0f, 0xfff00000 }, //A
	//		x	  y		z		color
		{ -1.0f, -1.0f, 1.0f, 0x000fffff }, //A
		{ -1.0f, -1.0f, -1.0f, 0x000fffff }, //B
		{ -1.0f, 1.0f, -1.0f, 0x000fffff }, //C

		{ -1.0f, 1.0f, -1.0f, 0xfff00fff }, //C
		{ -1.0f, 1.0f, 1.0f, 0xfff00fff }, //D
		{ -1.0f, -1.0f, 1.0f, 0xfff00fff }, //A
	/////////////////////////////////////////////////
		{ 1.0f, -1.0f, -1.0f, 0x00000fff }, //A
		{ -1.0f, -1.0f, -1.0f, 0x00000fff }, //B
		{ -1.0f, 1.0f, -1.0f, 0x00000fff }, //C

		{ -1.0f, 1.0f, -1.0f, 0xfff00000 }, //C
		{ 1.0f, 1.0f, -1.0f, 0xfff00000 }, //D
		{ 1.0f, -1.0f, -1.0f, 0xfff00000 }, //A
	/////////////////////////////////////////////////
		{ 1.0f, -1.0f, -1.0f, 0x000fffff }, //A
		{ 1.0f, -1.0f, 1.0f, 0x000fffff }, //B
		{ 1.0f, 1.0f, 1.0f, 0x000fffff }, //C

		{ 1.0f, 1.0f, 1.0f, 0xfff00fff }, //C
		{ 1.0f, 1.0f, -1.0f, 0xfff00fff }, //D
		{ 1.0f, -1.0f, -1.0f, 0xfff00fff }, //A
	///////////////////////////////////////////////
		{ 1.0f, 1.0f, 1.0f, 0xffffffff }, //A
		{ -1.0f, 1.0f, 1.0f, 0xffffffff }, //B
		{ -1.0f, 1.0f, -1.0f, 0xffffffff }, //C

		{ -1.0f, 1.0f, -1.0f, 0x00000000 }, //C
		{ 1.0f, 1.0f, -1.0f, 0x00000000 }, //D
		{ 1.0f, 1.0f, 1.0f, 0x00000000 }, //A
		////////////////////
		{ 1.0f, -1.0f, 1.0f, 0xffffffff }, //A
		{ -1.0f, -1.0f, 1.0f, 0xffffffff }, //B
		{ -1.0f, -1.0f, -1.0f, 0xffffffff }, //C

		{ -1.0f, -1.0f, -1.0f, 0x00000000 }, //C
		{ 1.0f, -1.0f, -1.0f, 0x00000000 }, //D
		{ 1.0f, -1.0f, 1.0f, 0x00000000 }, //A
	};
		//create VertexBuffer
		if (FAILED(pDirect3DDevice -> CreateVertexBuffer(36 * sizeof(CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX, 
			D3DPOOL_DEFAULT, &pVertexBuffer, NULL)))
			return E_FAIL;
		VOID* pBV;
		//lock VertexBuffer
		if (FAILED(pVertexBuffer->Lock(0, sizeof(Vertexes), (void**)&pBV, 0)))
			return E_FAIL;
		//memcopy
		memcpy(pBV, Vertexes, sizeof(Vertexes));
		//unlock VertexBuffer
		pVertexBuffer->Unlock();
		return S_OK;
}*/

//function WinMain
int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	
	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_DIRECTXAPP, szWindowClass, MAX_LOADSTRING);

	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
		return FALSE;
	ZeroMemory(&msg, sizeof(msg));
	
	// Main message loop:
	while (msg.message != WM_QUIT){
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
			RenderingDirect3D();
	}
	return (int) msg.wParam;
}

// создаем и регистрируем WindowClass
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DIRECTXAPP));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+5);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, L"Базовое окно для DirectX", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
      300, 150, 500, 500, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   } 
   if (FAILED(InitialDirect3D(hWnd))) return FALSE;
   if (FAILED(InitialObject())) return FALSE;
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
  
   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	
	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		
		// Parse the menu selections:
		switch (wmId){
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		ReleaseDirect3D();
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			PostQuitMessage(0);
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


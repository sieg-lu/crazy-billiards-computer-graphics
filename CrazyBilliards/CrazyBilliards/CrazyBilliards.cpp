#include "HighLevelLogic.h"
#include "TSControl.h"

//#define __MY_DEBUG__

#ifdef __MY_DEBUG__
TSConsole tc;
#endif

HGLRC								ghRC = NULL;
HDC									ghDC = NULL;
HWND								ghWnd = NULL;
HINSTANCE							ghInstance;

bool								gKeys[256];
bool								gIsActive = true;
bool								gIsFullscreen =  true;

bool								gMouseClicked = false;
int									gMouseClickX = -1, gMouseClickY = -1;
int									gMouseMoveX = -1, gMouseMoveY = -1;

GameLogicManager*					gGameLogicMgr;

LRESULT CALLBACK					WndProc(HWND, UINT, WPARAM, LPARAM);
bool								InitializeScene();
bool								UpdateScene();
bool								DestroyScene();

void ResizeScene(GLsizei width, GLsizei height)
{
	if (!height) {
		height = 1;
	}
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0.0, width, 0.0, height, -1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

bool InitializeGL()
{
	if (!InitializeScene()) {
		return false;
	}

	timeBeginPeriod(1);
	glClearColor(0.3, 0.0, 0.3, 0.0);

	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0f);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glLoadIdentity();

	return true;
}

bool InitializeScene()
{
	gGameLogicMgr = new GameLogicManager();
	return gGameLogicMgr->Initialize(ghDC);
}

bool UpdateScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	gGameLogicMgr->Update();

//	gWhiteBall->DebugOutput();

	if (gMouseClicked) {
		glLineWidth(2.5f);
		glEnable(GL_LINE_STIPPLE);
		glLineStipple(1, 0x0FFF);
		glBegin(GL_LINES);
		{
			glColor3f(0.3f, 0.0f, 0.3f);
//			glVertex2d(gMouseClickX, gMouseClickY);
			glVertex2d(gGameLogicMgr->getWhiteBall()->getCoordX(), gGameLogicMgr->getWhiteBall()->getCoordY());
			glVertex2d(gMouseMoveX, gMouseMoveY);
			glColor3f(1.0f, 1.0f, 1.0f);
		}
		glEnd();
		glDisable(GL_LINE_STIPPLE);
		glLineWidth(1.0f);
//		printf("%d %d %d %d\n", gMouseClickX, gMouseClickY, gMouseMoveX, gMouseMoveY);
	}

#ifdef __MY_DEBUG__
	glColor3f(1.0f, 0.0f, 0.0f);
	glLineWidth(5.0f);
	glBegin(GL_LINE_LOOP);
	{
		glVertex2d(gsMiddleHolePosX[0], gsMiddleHolePosY[0]);
		glVertex2d(gsMiddleHolePosX[1], gsMiddleHolePosY[1]);
	}
	glEnd();
	glLineWidth(1.0f);
	glColor3f(1.0f, 1.0f, 1.0f);

	glColor3f(1.0f, 0.0f, 0.0f);
	glLineWidth(5.0f);
	glBegin(GL_LINE_LOOP);
	{
		for (int i = 0; i < 4; i++) {
			glVertex2d(gsCornerHolePosX[i], gsCornerHolePosY[i]);
		}
	}
	glEnd();
	glLineWidth(1.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
#endif

	return true;
}

bool DestroyScene()
{
	gGameLogicMgr->Destroy();
	delete gGameLogicMgr;

	return true;
}

void KillGLWindow()
{
	DestroyScene();
	if (gIsFullscreen) {
		ChangeDisplaySettings(NULL, 0);
		ShowCursor(true);
	}
	if (ghRC) {
		if(!wglMakeCurrent(NULL, NULL))
			MessageBox(NULL, "Release Of DC And Rc Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		if(!wglDeleteContext(ghRC))
			MessageBox(NULL, "Release Rendering Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		ghRC = NULL;
	}
	if (ghDC && !ReleaseDC(ghWnd, ghDC)) {
		MessageBox(NULL, "Release Device Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		ghDC = NULL;
	}
	if (ghWnd && !DestroyWindow(ghWnd)) {
		MessageBox(NULL, "Could Not Release hWnd.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		ghWnd = NULL;
	}
	if (!UnregisterClass("OpenGL", ghInstance)) {
		MessageBox(NULL, "Could Not Unregister Class.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		ghInstance = NULL;
	}
}

bool CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint PixelFormat;
	WNDCLASS wc;
	DWORD dwExStyle;
	DWORD dwStyle;
	RECT WindowRect;

	WindowRect.left = (long)0;
	WindowRect.right = (long)width;
	WindowRect.top = (long)0;
	WindowRect.bottom = (long)height;

	gIsFullscreen = fullscreenflag;

	ghInstance = GetModuleHandle(NULL);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = (WNDPROC) WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = ghInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "OpenGL";

	if (!RegisterClass(&wc)) {
		MessageBox(NULL, "Failed To Register The Window Class.", "ERROR", MB_OK | MB_ICONINFORMATION);
		return false;
	}

	if (gIsFullscreen) {
		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = width;
		dmScreenSettings.dmPelsHeight = height;
		dmScreenSettings.dmBitsPerPel = bits;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_BADDUALVIEW) {
			if (MessageBox(NULL, "The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?", "NeHe GL", MB_YESNO | MB_ICONINFORMATION) == IDYES) {
				gIsFullscreen = false;
			} else {
				MessageBox(NULL, "Program Will Now Close.", "ERROR", MB_OK | MB_ICONSTOP);
				return false;
			}
		}
	}
	if (gIsFullscreen) {
		dwExStyle = WS_EX_APPWINDOW;
		dwStyle = WS_POPUP;
		ShowCursor(false);
	} else {
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		dwStyle = WS_OVERLAPPEDWINDOW;
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);

	if (!(ghWnd = CreateWindowEx(dwExStyle,
		"OpenGL",
		title, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | dwStyle,
		0, 0,
		WindowRect.right-WindowRect.left, WindowRect.bottom-WindowRect.top,
		NULL, NULL, ghInstance, NULL))) {
		KillGLWindow();
		MessageBox(NULL, "Window Creation Error.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		bits,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 16, 0, 0,
		PFD_MAIN_PLANE,
		0, 0, 0, 0
	};

	if (!(ghDC = GetDC(ghWnd))) {
		KillGLWindow();
		MessageBox(NULL, "Can¡¯t Create A GL Device Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	if (!(PixelFormat = ChoosePixelFormat(ghDC, &pfd))) {
		KillGLWindow();
		MessageBox(NULL, "Can¡¯t Find A Suitable PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	if (!SetPixelFormat(ghDC, PixelFormat, &pfd)) {
		KillGLWindow();
		MessageBox(NULL,"Can¡¯t Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;
	}

	if (!(ghRC = wglCreateContext(ghDC))) {
		KillGLWindow();
		MessageBox(NULL, "Can¡¯t Create A GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	if (!wglMakeCurrent(ghDC, ghRC)) {
		KillGLWindow();
		MessageBox(NULL, "Can¡¯t Activate The GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	ShowWindow(ghWnd, SW_SHOW);
	SetForegroundWindow(ghWnd);
	SetFocus(ghWnd);
	ResizeScene(width, height);

	if (!InitializeGL()) {
		KillGLWindow();
		MessageBox(NULL, "Initialization Failed.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	return true;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_ACTIVATE:
		{
			if(!HIWORD(wParam)) {
				gIsActive = true;
			} else {
				gIsActive = false;
			}
			return 0;
		}
	case WM_SYSCOMMAND:
		{
			switch(wParam)
			{
			case SC_SCREENSAVE:
			case SC_MONITORPOWER:
				return 0;
			}
			break;
		}
	case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}
	case WM_KEYDOWN:
		{
			gKeys[wParam] = true;
			return 0;
		}
	case WM_KEYUP:
		{
			gKeys[wParam] = false;
			return 0;
		}
	case WM_SIZE:
		{
			ResizeScene(LOWORD(lParam), HIWORD(lParam));
			return 0;
		}
	case WM_MOUSEMOVE:
		{
			int x = (int)LOWORD(lParam), y = WINDOW_HEIGHT - (int)HIWORD(lParam);
			if (((x - gMouseClickX) * (x - gMouseClickX) + (y - gMouseClickY) * (y - gMouseClickY)) < (MAXIMUM_FORCE * MAXIMUM_FORCE)) {
				gGameLogicMgr->getWhiteBall()->MouseMove(x, y);
				gGameLogicMgr->getHitting()->MouseMove(x, y);
				gGameLogicMgr->getStrength()->MouseMove(x - gMouseClickX, y - gMouseClickY);
				gMouseMoveX = x;
				gMouseMoveY = y;
			}
			return 0;
		}
	case WM_LBUTTONDOWN:
		{
			if (!gGameLogicMgr->getWhiteBall()->getRoundOver()) {
				return 0;
			}

			if (gGameLogicMgr->getWhiteBall()->LeftButtonDown((int)LOWORD(lParam), WINDOW_HEIGHT - (int)HIWORD(lParam))) {
				gMouseClicked = true;
				gMouseClickX = gMouseMoveX = (int)LOWORD(lParam);
				gMouseClickY = gMouseMoveY = WINDOW_HEIGHT - (int)HIWORD(lParam);
				gGameLogicMgr->getStrength()->LeftButtonDown(true);
				gGameLogicMgr->getWhiteBall()->setRoundOver(false);
			}
			if (gGameLogicMgr->getHitting()->LeftButtonDown((int)LOWORD(lParam), WINDOW_HEIGHT - (int)HIWORD(lParam))) {
				std::cout << "Hitting clicked!\n";
			}
			printf("%d %d\n", (int)LOWORD(lParam), WINDOW_HEIGHT - (int)HIWORD(lParam));
			return 0;
		}
	case WM_LBUTTONUP:
		{
			if (gGameLogicMgr->getWhiteBall()->IsSelected()) {
				gGameLogicMgr->ChangePlayer();
			}
			gGameLogicMgr->getWhiteBall()->LeftButtonUp(gMouseMoveX, gMouseMoveY, gGameLogicMgr->getHitting()->GetHittingDist(), gGameLogicMgr->getHitting()->GetHittingPhi());
			gGameLogicMgr->getHitting()->LeftButtonUp((int)LOWORD(lParam), WINDOW_HEIGHT - (int)HIWORD(lParam));
			gGameLogicMgr->getStrength()->LeftButtonUp();
			gMouseClicked = false;
			gMouseClickX = gMouseMoveX = -1;
			gMouseClickY = gMouseMoveY = -1;
			return 0;
		}
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrecInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	bool done = false;

	gIsFullscreen = false;
	if (!CreateGLWindow("Crazy Snooker", WINDOW_WIDTH, WINDOW_HEIGHT, 32, gIsFullscreen)) {
		return 0;
	}
	while (!done) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				done = true;
			} else {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		} else {
			if (gIsActive) {
				if (gKeys[VK_ESCAPE]) {
					done = TRUE;
				} else {
					DWORD starttime = timeGetTime();
#ifdef __MY_DEBUG__
					if (gKeys['E']) {
						gGameLogicMgr->getBoard()->setFriction(5.0f);
					}
					if (gKeys['R']) {
						gGameLogicMgr->getBoard()->setFriction(1.0f);
					}
					if (gKeys['T']) {
						gGameLogicMgr->getBoard()->setFriction(0.5f);
					}
#endif
					if (gKeys['P']) {
						DestroyScene();
						InitializeScene();
					}
					UpdateScene();
					SwapBuffers(ghDC);

					// limit the FPS
					DWORD endtime = timeGetTime() - starttime;
					if (endtime < 1000 / FPS) {
						Sleep(1000 / FPS - endtime);
					}
					endtime = timeGetTime() - starttime;
					if (endtime == 0) {
						gFps = 60;
					} else {
						gFps = 1000 / endtime;
					}
					//std::cout << gFps << std::endl;
				}
			}
		}
	}
	KillGLWindow();
	return (msg.wParam);
}

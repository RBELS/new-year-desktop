#include "framework.h"
#include "draw-desktop.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include <ctime>
#include "gametime.h"
#include "view.h"
#include <string>

#define MAX_LOADSTRING 100

HINSTANCE hInst;
HWND hDesktop = NULL;

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void InitGlContext();
void Draw();

typedef LRESULT (CALLBACK __stdcall* HookProc)(int, WPARAM, LPARAM);
typedef void (__stdcall* InstallHook)();
typedef void (__stdcall* UninstallHook)();

LRESULT CALLBACK HookProcedure(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode >= 0)
    {
        MessageBox(GetDesktopWindow(), "Text", "text", MB_OK);
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

HWND hWorker = NULL;
BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam)
{
    HWND p = FindWindowEx(hwnd, NULL, "SHELLDLL_DefView", NULL);

    if (p != NULL)
    {
        hWorker = FindWindowEx(NULL, hwnd, "WorkerW", NULL);
        return FALSE;
    }

    return TRUE;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    HWND parentFolderView = FindWindowEx(0, 0, "Progman", "Program Manager");
    if (parentFolderView == 0)
    {
        MessageBox(GetDesktopWindow(), "Can not find Progman window", "Error", MB_OK);
        return -1;
    }

    DWORD result;
    SendMessageTimeout(parentFolderView, 0x052C, NULL, 0, SMTO_NORMAL, 1000, (PDWORD_PTR) &result); // Хуйня какая-то!
    EnumWindows(EnumChildProc, NULL);
    if (hWorker == NULL)
    {
        MessageBox(GetDesktopWindow(), "Can not find WorkerW window", "Error", MB_OK);
        return -1;
    }

    hDesktop = hWorker;
    InitGlContext();
    while (true)
    {
        gametime::UpdateTicks();
        view::MoveSnow();
        view::MoveAllFireworks();
        Draw();
        Sleep(16);
    }

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}


HDC hdc;
HGLRC hrc;
PIXELFORMATDESCRIPTOR pfd;
RECT WIN_RECT = {};
HANDLE hControlThread;

BOOL glContextInited = FALSE;


void InitGlContext()
{
    hdc = GetDC(hDesktop);
    SetCursor(NULL);
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_SUPPORT_OPENGL + PFD_DOUBLEBUFFER + PFD_DRAW_TO_WINDOW;
    pfd.iLayerType = PFD_MAIN_PLANE;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 16;
    pfd.cDepthBits = 16;
    pfd.cAccumBits = 0;
    pfd.cStencilBits = 0;

    int choosePixelFormatResult = ChoosePixelFormat(hdc, &pfd);
    SetPixelFormat(hdc, choosePixelFormatResult, &pfd);
    hrc = wglCreateContext(hdc);
    wglMakeCurrent(hdc, hrc);
    GetClientRect(hDesktop, &WIN_RECT);
    glViewport(0, 0, WIN_RECT.right, WIN_RECT.bottom);

    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    InvalidateRect(hDesktop, &WIN_RECT, true);
    srand(time(NULL));
    view::InitScreen();
    gametime::InitTicks();
    view::InitSnow();
    view::InitMoonTex();
    view::InitTreesTextures();
    view::InitFirework();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-view::aspect , view::aspect, -1.0, 1.0);

    glContextInited = TRUE;
}

void Draw()
{
    LARGE_INTEGER counter = {};
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    view::DrawAllFireworks();
    view::DrawSnow();
    view::DrawMoon();
    view::DrawTrees();

    SwapBuffers(hdc);
}

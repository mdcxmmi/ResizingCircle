// ResizingCircle.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "ResizingCircle.h"

#define MAX_LOADSTRING 100
#define MARGINTOP 50
#define MARGINLEFT 50
#define MARGINRIGHT 50
#define MARGINBOTTOM 50

#define DEFAULTSPEED 5

#define backgroundcolor RGB(222,222,222)
#define  bordercolor RGB(0,0,0)

#define CircleBackgroupndColor RGB(222,222,222)
#define CircleBorderColor RGB(0,0,222)


HWND hWnd;


struct stCONFIG
{
    POINT topleft;
    POINT bottomright;
    RECT winrect;
    BOOL dirty;
} MoveBoxConfig;

struct Circle {
    POINT position;
    UINT radius;
    COLORREF color;
    COLORREF backgroundColor;
    UCHAR thickness;
    BOOL visible;
    BOOL dirty;
} Circle;


enum TChangeVector
{
    Increase,
    Decrease,
    Nothing
};


struct TResizing {
    TChangeVector vector;
    UCHAR speed;
}Resizing;



void InitMovingBox() {
    RECT R;

    GetClientRect(hWnd, &R);

    if (R.top != MoveBoxConfig.winrect.top || R.left != MoveBoxConfig.winrect.left ||
        R.right != MoveBoxConfig.winrect.right || R.bottom != MoveBoxConfig.winrect.bottom)
    {
        MoveBoxConfig.dirty = true;
        MoveBoxConfig.topleft.x = MARGINLEFT;
        MoveBoxConfig.topleft.y = MARGINTOP;
        MoveBoxConfig.bottomright.x = R.right - MARGINRIGHT;
        MoveBoxConfig.bottomright.y = R.bottom - MARGINBOTTOM;

        MoveBoxConfig.winrect.top = R.top;
        MoveBoxConfig.winrect.left = R.left;
        MoveBoxConfig.winrect.right = R.right;
        MoveBoxConfig.winrect.bottom = R.bottom;
    }

}


void DrawMovingBox(HDC dc) {
    if (MoveBoxConfig.dirty) {
        HBRUSH bkg = CreateSolidBrush(backgroundcolor);
        HBRUSH bOld = (HBRUSH)SelectObject(dc, bkg);
        HPEN borderpen = CreatePen(PS_SOLID, 1, bordercolor);
        HPEN pOld = (HPEN)SelectObject(dc, borderpen);

        Rectangle(dc, MoveBoxConfig.topleft.x, MoveBoxConfig.topleft.y, MoveBoxConfig.bottomright.x, MoveBoxConfig.bottomright.y);
        SelectObject(dc, bOld);
        SelectObject(dc, pOld);

        DeleteObject(bkg);
        DeleteObject(borderpen);
        MoveBoxConfig.dirty = false;
    }
}

void InitCircle() {
    Circle.position.x = (MoveBoxConfig.topleft.x + MoveBoxConfig.bottomright.x) / 2;
    Circle.position.y = (MoveBoxConfig.topleft.y + MoveBoxConfig.bottomright.y) / 2;
    Circle.radius = 50;
    Circle.color = CircleBorderColor;
    Circle.backgroundColor = CircleBackgroupndColor;
    Circle.thickness = 3;
    Circle.visible = true;
    Circle.dirty = true;
}


void ResizeCircle()
{
    switch (Resizing.vector)
    {
    case Increase:
        Circle.radius += Resizing.speed;
        Resizing.vector = Nothing;
        break;

    case Decrease:
        Circle.radius -= Resizing.speed;
        Resizing.vector = Nothing;
        break;


    }
}




void UpdateCircle() {
    if (Resizing.vector == Nothing) return;

    Circle.position.x = (MoveBoxConfig.topleft.x + MoveBoxConfig.bottomright.x) / 2;
    Circle.position.y = (MoveBoxConfig.topleft.y + MoveBoxConfig.bottomright.y) / 2;
    ResizeCircle();
    Circle.dirty = true;
}




void DrawCircle(HDC dc) {
    RECT ClearBox;
    POINT LeftTopPos;
    //  if (!Circle.dirty) return;

    HBRUSH BKG = CreateSolidBrush(Circle.backgroundColor);
    HBRUSH bOld = (HBRUSH)SelectObject(dc, BKG);
    HPEN pen = CreatePen(PS_SOLID, Circle.thickness, Circle.color);
    HPEN pOld = (HPEN)SelectObject(dc, pen);

    if (Circle.visible)
    {
        LeftTopPos.x = Circle.position.x - Circle.radius;
        LeftTopPos.y = Circle.position.y - Circle.radius;

        Ellipse(dc, LeftTopPos.x, LeftTopPos.y, LeftTopPos.x + Circle.radius * 2, LeftTopPos.y + Circle.radius * 2);

        Circle.dirty = false;

        //            InvalidateRect(hWnd, &ClearBox, true);
    }

    SelectObject(dc, bOld);
    SelectObject(dc, pOld);
    DeleteObject(BKG);
    DeleteObject(pen);


}


void ClearCircle(HDC dc)
{
    HBRUSH bkg = CreateSolidBrush(backgroundcolor);
    HBRUSH bOld = (HBRUSH)SelectObject(dc, bkg);
    HPEN pen = CreatePen(PS_SOLID, Circle.thickness, backgroundcolor);
    HPEN pOld = (HPEN)SelectObject(dc, pen);
    if (Circle.visible)
    {
        Ellipse(dc, Circle.position.x, Circle.position.y, Circle.position.x + Circle.radius, Circle.position.y + Circle.radius);
    }
    SelectObject(dc, bOld);
    SelectObject(dc, pOld);
    DeleteObject(bkg);
    DeleteObject(pen);
}






void ReDraw(HDC hdc)
{
    ClearCircle(hdc);
    ResizeCircle();
    DrawCircle(hdc);
}








// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_RESIZINGCIRCLE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_RESIZINGCIRCLE));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_RESIZINGCIRCLE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_RESIZINGCIRCLE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }


   InitMovingBox();
   Resizing.vector = Nothing;
   Resizing.speed = DEFAULTSPEED;
   InitCircle();


   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Разобрать выбор в меню:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;

    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_UP:
            Resizing.vector = Increase;
            InvalidateRect(hWnd, NULL, true);
            MoveBoxConfig.dirty = true;

            break;

        case VK_DOWN:
            Resizing.vector = Decrease;
            InvalidateRect(hWnd, NULL, true);
            MoveBoxConfig.dirty = true;

            break;

        case VK_LEFT:
            Resizing.vector = Decrease;
            InvalidateRect(hWnd, NULL, true);
            MoveBoxConfig.dirty = true;

            break;

        case VK_RIGHT:
            Resizing.vector = Increase;
            InvalidateRect(hWnd, NULL, true);
            MoveBoxConfig.dirty = true;

            break;

        case  VK_ESCAPE:
        {
            DestroyWindow(hWnd);
            break;
        }
        break;

        }

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: Добавьте сюда любой код прорисовки, использующий HDC...
        InitMovingBox();
        DrawMovingBox(hdc);
        //           ClearCircle(hdc);
        //           ResizeCircle();
        UpdateCircle();
        DrawCircle(hdc);

        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

#include <Windows.h>
#include <vector>
#include <map>
#include <set>
#include"winmain.h"
#include "scene.h"
#include"Role.h"
#pragma comment(lib,"ws2_32.lib")
extern char m_col;
extern card  arr[54];//牌堆
extern card  Desk[54];//桌面上的牌
extern int arr_loc;//牌堆顶部对应数组位置
extern int Desk_n;//当前桌面上牌数
Scene::Scene(Role *r)  //构造函数
	: role(r)
{
	sceneSize.cx = 850;
	sceneSize.cy = 540;
	cardSize.cx = 71;
	cardSize.cy = 96;
}
Scene::~Scene()//析构函数
{
	DeleteDC(hdcScene);
	DeleteDC(hdcBkg);
	DeleteDC(hdcHumanCards);
	DeleteDC(hdcCardBack);
	DeleteDC(hdcCards);
	DeleteObject(hbmBkg);
	DeleteObject(hbmCardBack);
	DeleteObject(hbmCards);
	DeleteObject(hbmScene);
	DeleteObject(hbmHumanCards);
	DeleteObject(hbrush);
}
SIZE Scene::GetSize()
{
	return sceneSize;
}
//初始化游戏场景
void Scene::InitScene(HWND hwnd)
{
	int x = sceneSize.cx / 2;//425
	int y = sceneSize.cy - 130 - 40;//370
	discand = CreateWindow(TEXT("mybutton"), TEXT("discard"), WS_CHILD,//出牌
		sceneSize.cx - 340, y, 68, 35, hwnd, (HMENU)Discard, GetModuleHandle(NULL), NULL);
	pass = CreateWindow(TEXT("mybutton"), TEXT("pass"), WS_CHILD,//不出
		sceneSize.cx - 260, y, 68, 35, hwnd, (HMENU)Pass, GetModuleHandle(NULL), NULL);
	change= CreateWindow(TEXT("mybutton"), TEXT("change"), WS_CHILD,//换牌
		sceneSize.cx - 180, y, 68, 35, hwnd, (HMENU)Change, GetModuleHandle(NULL), NULL);
	//创建相关位图内存设备环境
	HDC hdc = GetDC(hwnd);
	HINSTANCE hInst = GetModuleHandle(NULL);

	HBITMAP hbitmap = LoadBitmap(hInst, TEXT("background"));//加载位图资源
	hbrush = CreatePatternBrush(hbitmap);
	DeleteObject(hbitmap);
	//函数创建与指定的设备环境相关的设备兼容的位图
	hbmScene = CreateCompatibleBitmap(hdc, sceneSize.cx, sceneSize.cy);
	hbmBkg = CreateCompatibleBitmap(hdc, sceneSize.cx, sceneSize.cy);
	hbmHumanCards = CreateCompatibleBitmap(hdc, 650, 128);
	hbmCardBack = LoadBitmap(hInst, TEXT("cardback"));
	hbmCards = LoadBitmap(hInst, TEXT("cards"));
	hbmNoDiscard = LoadBitmap(hInst, TEXT("nodiscard"));

	hdcScene = CreateCompatibleDC(hdc);
	hdcBkg = CreateCompatibleDC(hdc);
	hdcHumanCards = CreateCompatibleDC(hdc);
	hdcCardBack = CreateCompatibleDC(hdc);
	hdcCards = CreateCompatibleDC(hdc);
	hdcNoDiscard = CreateCompatibleDC(hdc);
	SelectObject(hdcScene, hbmScene);
	SelectObject(hdcBkg, hbmBkg);
	SelectObject(hdcBkg, hbrush);
	SelectObject(hdcHumanCards, hbmHumanCards);
	SelectObject(hdcHumanCards, hbrush);
	SelectObject(hdcCardBack, hbmCardBack);
	SelectObject(hdcCards, hbmCards);
	SelectObject(hdcNoDiscard, hbmNoDiscard);//后者加入前者，替换前者

	ReleaseDC(hwnd, hdc);
}
void Scene::ShowScene(HDC hdc)
{
	BitBlt(hdcScene, 0, 0, sceneSize.cx, sceneSize.cy, hdcBkg, 0, 0, SRCCOPY);
	DrawComputerCards();
	DrawHumanCards(hdcScene);//选择绘制到游戏画面环境中是为了防止二次绘制产生的闪烁
	BitBlt(hdc, 0, 0, sceneSize.cx, sceneSize.cy, hdcScene, 0, 0, SRCCOPY);
}
//显示游戏画面2
void Scene::ShowScene(HWND hwnd)
{
	HDC hdc = GetDC(hwnd);
	ShowScene(hdc);
	ReleaseDC(hwnd, hdc);
}
//显示出牌按钮
void Scene::ShowDiscardBtn()
{
	PostMessage(discand, WM_MYBUTTON, FALSE, 0);
	PostMessage(pass, WM_MYBUTTON, FALSE, 0);
	ShowWindow(discand, SW_SHOW);
	ShowWindow(pass, SW_SHOW);
	ShowWindow(change, SW_SHOW);

}
//隐藏出牌按钮
void Scene::HideDiscardBtn()
{
	ShowWindow(discand, SW_HIDE);
	ShowWindow(pass, SW_HIDE);
	ShowWindow(change, SW_HIDE);
}
//绘制背景
void Scene::DrawBackground(void)
{
	PatBlt(hdcBkg, 0, 0, sceneSize.cx, sceneSize.cy, PATCOPY);
	HPEN hpen = CreatePen(PS_SOLID, 3, RGB(192, 192, 192));
	HPEN oldpen = (HPEN)SelectObject(hdcBkg, hpen);
	Rectangle(hdcBkg, 100, -5, sceneSize.cx - 100, sceneSize.cy - 130);
	SelectObject(hdcBkg, oldpen);
	DeleteObject(hpen);
}
//画对方手牌
void Scene::DrawComputerCards(void)
{
	int  x ;
	SIZE size;
	TCHAR szText[5];
	HFONT hfont = CreateFont(72, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, TEXT("Calibri"));
	HFONT oldfont = (HFONT)SelectObject(hdcScene, hfont);
	COLORREF color = SetTextColor(hdcScene, RGB(255, 0, 128));
	SetBkMode(hdcScene, TRANSPARENT);
	//画背面
	TransparentBlt(hdcScene, 390, 10, cardSize.cx, cardSize.cy,
			hdcCardBack, 0, 0, cardSize.cx, cardSize.cy, RGB(0, 0, 255));
		//然后写上剩余牌数
	 x = role->Get_num();
	wsprintf(szText, TEXT("%d"), x);
		GetTextExtentPoint32(hdcScene, szText, wcslen((const wchar_t*)szText), &size);
		TextOut(hdcScene, 390, 10, szText, wcslen((const wchar_t*)szText));
		if (role->att_get() == 0) {//显示“不出”
			TransparentBlt(hdcScene, 390, 200, 63, 27,hdcNoDiscard, 0, 0, 63, 27, RGB(255, 255, 255));
		}
		else {                
		         //显示打出的牌
			TransparentBlt(hdcScene, 390,200, cardSize.cx, cardSize.cy, hdcCards, cardSize.cx , 0,cardSize.cx, cardSize.cy, RGB(0, 0, 255));
			}
	}
//画自己手牌
void Scene::DrawHumanCards(HDC hdc, int highlight)
{
	PatBlt(hdcHumanCards, 0, 0, sceneSize.cx, sceneSize.cy, PATCOPY);
	int i = 0, y1,
		c = role->Get_num(),
		x = (650 - cardSize.cx - 22 * c + 22) / 2,
		y = 128 - cardSize.cy - 15;
	///先不加上移
		/*if (i=0/*选中*//*)//将已选择的牌上移一段，突出显示
			y1 = y - 15;
		else
			y1 = y;*/
		TransparentBlt(hdcHumanCards, x + 22 * i, y, cardSize.cx, cardSize.cy, hdcCards,
			cardSize.cx , 0, cardSize.cx, cardSize.cy, RGB(0, 0, 255));
	BitBlt(hdc, 100, 412, 650, 128, hdcHumanCards, 0, 0, SRCCOPY);
}
//绘制玩家牌的另一种重载形式
void Scene::DrawHumanCards(HWND hwnd, int highlight)
{
	HDC hdc = GetDC(hwnd);
	DrawHumanCards(hdc);
	ReleaseDC(hwnd, hdc);
}
//判断某一点上是否有牌，如有牌，返回该牌的值（0-53）重要
int Scene::PointInWhich(POINT point)
{
	auto human = role;

	int c = human->Get_num(),
		x = (sceneSize.cx - cardSize.cx - 22 * c + 22) / 2,
		y = sceneSize.cy - cardSize.cy - 15;
	RECT rect;
	auto b = 1;//开始

	for (int i = c; i > 0 && b != human->att_get(); --i, ++b) {
		rect.top = y;
		rect.left = x + 22 * (i - 1);
		rect.right = rect.left + cardSize.cx;
		rect.bottom = rect.top + cardSize.cy;
		if (PtInRect(&rect, point))
			return b;
	}
	return -1;
}
//选择出牌    MAYBE有问题
void Scene::SelectCard(POINT point)
{
	int num = PointInWhich(point);
	if (num >= 0) {
		PostMessage(discand, WM_MYBUTTON, TRUE, 0);
		DrawHumanCards(GetParent(pass));
	}

}
//画牌堆
void Scene::showpaidui(HDC hdc)
{
	int i ;
	SIZE size;
	TCHAR szText[5];
	HFONT hfont = CreateFont(72, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, TEXT("Calibri"));
	HFONT oldfont = (HFONT)SelectObject(hdcScene, hfont);
	COLORREF color = SetTextColor(hdcScene, RGB(255, 0, 128));
	SetBkMode(hdcScene, TRANSPARENT);
	i = 43-arr_loc;
		TransparentBlt(hdcScene, 30, 10, cardSize.cx, cardSize.cy,hdcCardBack, 0, 0, cardSize.cx, cardSize.cy, RGB(0, 0, 255));
		wsprintf(szText, TEXT("%d"), i);
		GetTextExtentPoint32(hdcScene, szText, wcslen((const wchar_t*)szText), &size);
		TextOut(hdcScene, 30, 10, szText, wcslen((const wchar_t*)szText));
}
//展示底牌
void Scene::showbestcard(HDC hdc)
{        
	TransparentBlt(hdcScene, 800, 10, cardSize.cx, cardSize.cy,
		hdcCards, 0, 0, cardSize.cx, cardSize.cy, RGB(0, 0, 255));
}

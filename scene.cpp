#include <Windows.h>
#include <vector>
#include <map>
#include <set>
#include "card.h"
#include"windef.h"
#include"WinUser.h"
#include "Pc.h"
#include"winmain.h"
#include "plvpc.h"
#include "plvpl.h"
#include "scene.h"
#include"Role.h"



Scene::Scene(Role *r)  //构造函数
	: role(r)
{
	sceneSize.cx = 850;
	sceneSize.cy = 540;
	cardSize.cx = 71;
	cardSize.cy = 96;
	//game->RegisterScene(this);
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
	DrawChars();
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

	/*if (!game->lastone)
		PostMessage(pass, WM_MYBUTTON, FALSE, 0);
	else
		PostMessage(pass, WM_MYBUTTON, TRUE, 0);*/
	ShowWindow(discand, SW_SHOW);
	ShowWindow(pass, SW_SHOW);
	ShowWindow(change, SW_SHOW);
	//InvalidateRgn(change, NULL, FALSE);
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
void Scene::DrawComputerCards(void)
{
	int c, i = 0;
	SIZE size;
	TCHAR szText[5];
	HFONT hfont = CreateFont(72, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, TEXT("Calibri"));
	HFONT oldfont = (HFONT)SelectObject(hdcScene, hfont);
	COLORREF color = SetTextColor(hdcScene, RGB(255, 0, 128));
	SetBkMode(hdcScene, TRANSPARENT);

	/*i = game->player[1]->cards.size();
	if (i) {//先显示牌的背面
		TransparentBlt(hdcScene, sceneSize.cx - 50 - cardSize.cx / 2, 65, cardSize.cx, cardSize.cy,
			hdcCardBack, 0, 0, cardSize.cx, cardSize.cy, RGB(0, 0, 255));

		//然后写上剩余牌数
		wsprintf(szText, TEXT("%d"), i);
		GetTextExtentPoint32(hdcScene, szText, wcslen(szText), &size);
		TextOut(hdcScene, sceneSize.cx - 50 - size.cx / 2,
			65 + (cardSize.cy - size.cy) / 2, szText, wcslen(szText));*/
	/*if (game->player[1]->nodiscard){//显示“不出”
		TransparentBlt(hdcScene, sceneSize.cx - 168, 200, 63, 27,
			hdcNoDiscard, 0, 0, 63, 27, RGB(255, 255, 255));
	}
	else{                      打出的牌
		i = 0;
		c = game->player[1]->discard.count;
		for (auto rb = game->player[1]->discard.cards.rbegin();
			rb != game->player[1]->discard.cards.rend(); ++rb){//显示打出的牌
			TransparentBlt(hdcScene, sceneSize.cx - cardSize.cx - 20 * c - 85 + 20 * i,
				160, cardSize.cx, cardSize.cy, hdcCards, cardSize.cx * *rb, 0,
				cardSize.cx, cardSize.cy, RGB(0, 0, 255));
			++i;
		}
	}*/

	}
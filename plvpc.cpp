#include"Role.h" 
#include <graphics.h>
#include <conio.h>
#include"Pc.h"
#include"plvpc.h"
#include<iostream>
#include<stdlib.h>

using namespace std;
extern char m_col;
extern card  arr[54];//牌堆
extern card  Desk[54];//桌面上的牌
extern int arr_loc;//牌堆顶部对应数组位置
extern int Desk_n;//当前桌面上牌数

int PVP(Role pl, Pc pc);
int PVP(Pc pc, Role pl);
int PVP_Turn(Role pl, Pc pc);
int PVP_Turn(Pc pc, Role pl);
int PVP(Role pl, Pc pc)
{
	char n[10]; sprintf_s(n, "%d", (54-arr_loc));
	outtextxy(925, 220, "剩余牌数"); outtextxy(925, 270, n);
	if (arr_loc == 54 && pl.Get_num() == 1 && Desk_n == 0)//牌堆耗尽，桌面无牌，此时若进攻方手中仅余一张，
	{										//为鬼牌，则进攻方判输
		if (pl.joker_left() == 1)
			return 2;
	}
	int k;
	//outtextxy(200, 170, k);
	pl.Show();
	pc.Showem();
	for (k = pl.Attack(pl.att_get()); k == 0; k = pl.Attack(pl.att_get()));//k=1或2，2：放弃出牌
	if (k == 2 || k == 0)
	{
		pl.Get_cards();
		pc.Get_cards();//////
		Desk_clean();
		pl.clean();
		if (arr_loc == 54 && pl.Get_num() == 0)//经过上次判定，r_1在本回合不会获胜
			return 1;
		if (arr_loc == 54 && pc.Get_num() == 0)//经过上次判定，r_1在本回合不会获胜
			return 2;
		return PVP(pc, pl);
	}
	Desk_Show();
	if (arr_loc == 54 && pl.Get_num() == 0)
		return 1;
	int def = pc.Defend();
	Desk_Show();
	pc.Check_self(def);
	pl.Check_defender(def);

	if (def == 2 || def == 0) {
		pl.Get_cards();
		pc.Get_cards();
		Desk_clean();
		pl.clean();
	}
	if (arr_loc == 54 && pl.Get_num() == 0)//经过上次判定，r_1在本回合不会获胜
		return 1;
	if (arr_loc == 54 && pc.Get_num() == 0)//经过上次判定，r_1在本回合不会获胜
		return 2;				//此时判断r_2是否在本回合获胜
	switch (def)
	{
	case 0:return PVP(pl, pc);
	case 1:return PVP(pl, pc);
	case 2:return PVP(pc, pl);
	}
}
int PVP(Pc pc, Role pl)
{
	char n[10]; sprintf_s(n, "%d", (54 - arr_loc));
	outtextxy(925, 220, "剩余牌数"); outtextxy(925, 270, n);
	if (arr_loc == 54 && pc.Get_num() == 1 && Desk_n == 0)//牌堆耗尽，桌面无牌，此时若进攻方手中仅余一张，
	{										//为鬼牌，则进攻方判输
		if (pc.joker_left() == 1)
			return 1;
	}
	int k = pc.Attack();
	//outtextxy(200, 170, k);
	pc.Showem();
	pl.Show();
	////////////////改为pc出牌操作
	//k=1或2，2：放弃出牌
	if (k == 2)
	{
		pc.Get_cards();
		pl.Get_cards();
		Desk_clean();
		pl.clean();
		if (arr_loc == 54 && pl.Get_num() == 0)//经过上次判定，r_1在本回合不会获胜
			return 1;
		if (arr_loc == 54 && pc.Get_num() == 0)//经过上次判定，r_1在本回合不会获胜
			return 2;
		return PVP(pl, pc);
	}
	Desk_Show();
	if (arr_loc == 54 && pc.Get_num() == 0)//pc win
		return 2;
	int def = pl.Defend(pl.def_get());
	Desk_Show();
	int i = pl.Check_self(def);
	pl.Check_defender(i);
	if (i == 2 || i == 0) {
		pl.Get_cards(); Desk_clean(); pc.Get_cards();
		pc.clean();
	}
	if (arr_loc == 54 && pl.Get_num() == 0)//经过上次判定，r_1在本回合不会获胜
		return 1;
	if (arr_loc == 54 && pc.Get_num() == 0)//经过上次判定，r_1在本回合不会获胜
		return 2;
	//此时判断r_2是否在本回合获胜
	switch (i)
	{
	case 0:return PVP(pc, pl);
	case 1:return PVP(pc, pl);
	case 2:

		return PVP(pl, pc);
	}

}
int PVP_Turn(Role pl, Pc pc)
{
	IMAGE win, lose, yesc, noc, bck;
	loadimage(&win, "G:\\ceshi\\win.jpg", 200, 100);
	loadimage(&lose, "G:\\ceshi\\lose.jpg", 200, 100);
	IMAGE cardimage[53];
	loadimage(&cardimage[53], "G:\\ceshi\\bigk.jpg", 75, 100);
	loadimage(&cardimage[0], "G:\\ceshi\\hk.jpg", 75, 100);
	loadimage(&cardimage[1], "G:\\ceshi\\ha.jpg", 75, 100);
	loadimage(&cardimage[2], "G:\\ceshi\\h2.jpg", 75, 100);
	loadimage(&cardimage[3], "G:\\ceshi\\h3.jpg", 75, 100);
	loadimage(&cardimage[4], "G:\\ceshi\\h4.jpg", 75, 100);
	loadimage(&cardimage[5], "G:\\ceshi\\h5.jpg", 75, 100);
	loadimage(&cardimage[6], "G:\\ceshi\\h6.jpg", 75, 100);
	loadimage(&cardimage[7], "G:\\ceshi\\h7.jpg", 75, 100);
	loadimage(&cardimage[8], "G:\\ceshi\\h8.jpg", 75, 100);
	loadimage(&cardimage[9], "G:\\ceshi\\h9.jpg", 75, 100);
	loadimage(&cardimage[10], "G:\\ceshi\\h10.jpg", 75, 100);
	loadimage(&cardimage[11], "G:\\ceshi\\hj.jpg", 75, 100);
	loadimage(&cardimage[12], "G:\\ceshi\\hq.jpg", 75, 100);
	loadimage(&cardimage[13], "G:\\ceshi\\bk.jpg", 75, 100);
	loadimage(&cardimage[14], "G:\\ceshi\\ba.jpg", 75, 100);
	loadimage(&cardimage[15], "G:\\ceshi\\b2.jpg", 75, 100);
	loadimage(&cardimage[16], "G:\\ceshi\\b3.jpg", 75, 100);
	loadimage(&cardimage[17], "G:\\ceshi\\b4.jpg", 75, 100);
	loadimage(&cardimage[18], "G:\\ceshi\\b5.jpg", 75, 100);
	loadimage(&cardimage[19], "G:\\ceshi\\b6.jpg", 75, 100);
	loadimage(&cardimage[20], "G:\\ceshi\\b7.jpg", 75, 100);
	loadimage(&cardimage[21], "G:\\ceshi\\b8.jpg", 75, 100);
	loadimage(&cardimage[22], "G:\\ceshi\\b9.jpg", 75, 100);
	loadimage(&cardimage[23], "G:\\ceshi\\b10.jpg", 75, 100);
	loadimage(&cardimage[24], "G:\\ceshi\\bj.jpg", 75, 100);
	loadimage(&cardimage[25], "G:\\ceshi\\bq.jpg", 75, 100);
	loadimage(&cardimage[26], "G:\\ceshi\\fk.jpg", 75, 100);
	loadimage(&cardimage[27], "G:\\ceshi\\fa.jpg", 75, 100);
	loadimage(&cardimage[28], "G:\\ceshi\\f2.jpg", 75, 100);
	loadimage(&cardimage[29], "G:\\ceshi\\f3.jpg", 75, 100);
	loadimage(&cardimage[30], "G:\\ceshi\\f4.jpg", 75, 100);
	loadimage(&cardimage[31], "G:\\ceshi\\f5.jpg", 75, 100);
	loadimage(&cardimage[32], "G:\\ceshi\\f6.jpg", 75, 100);
	loadimage(&cardimage[33], "G:\\ceshi\\f7.jpg", 75, 100);
	loadimage(&cardimage[34], "G:\\ceshi\\f8.jpg", 75, 100);
	loadimage(&cardimage[35], "G:\\ceshi\\f9.jpg", 75, 100);
	loadimage(&cardimage[36], "G:\\ceshi\\f10.jpg", 75, 100);
	loadimage(&cardimage[37], "G:\\ceshi\\fj.jpg", 75, 100);
	loadimage(&cardimage[38], "G:\\ceshi\\fq.jpg", 75, 100);
	loadimage(&cardimage[39], "G:\\ceshi\\rk.jpg", 75, 100);
	loadimage(&cardimage[40], "G:\\ceshi\\ra.jpg", 75, 100);
	loadimage(&cardimage[41], "G:\\ceshi\\r2.jpg", 75, 100);
	loadimage(&cardimage[42], "G:\\ceshi\\r3.jpg", 75, 100);
	loadimage(&cardimage[43], "G:\\ceshi\\r4.jpg", 75, 100);
	loadimage(&cardimage[44], "G:\\ceshi\\r5.jpg", 75, 100);
	loadimage(&cardimage[45], "G:\\ceshi\\r6.jpg", 75, 100);
	loadimage(&cardimage[46], "G:\\ceshi\\r7.jpg", 75, 100);
	loadimage(&cardimage[47], "G:\\ceshi\\r8.jpg", 75, 100);
	loadimage(&cardimage[48], "G:\\ceshi\\r9.jpg", 75, 100);
	loadimage(&cardimage[49], "G:\\ceshi\\r10.jpg", 75, 100);
	loadimage(&cardimage[50], "G:\\ceshi\\rj.jpg", 75, 100);
	loadimage(&cardimage[51], "G:\\ceshi\\rq.jpg", 75, 100);
	loadimage(&cardimage[52], "G:\\ceshi\\minik.jpg", 75, 100);
	loadimage(&bck, "G:\\ceshi\\greenbck.jpg", 1000, 600);
	loadimage(&yesc, "G:\\ceshi\\yesc.jpg", 100, 100);
	loadimage(&noc, "G:\\ceshi\\noc.jpg", 100, 100);
	putimage(0, 0, &bck);
	outtextxy(450, 300, "是否继续游戏？");
	putimage(430, 400, &yesc);
	putimage(560, 400, &noc);
	/*cout << "是否继续？0：no，1：yes";
	int in;
	cin >> in;
	if (!in)
		return 0;*/MOUSEMSG m;
	while (true)
	{
		m = GetMouseMsg();
		switch (m.uMsg)
		{
		case WM_LBUTTONDOWN:
			if (m.x >= 430 && m.x <= 530 && m.y <= 500 && m.y >= 400)
			{
				putimage(0, 0, &bck);
				m_col = arr[53].color;
				putimage(825, 170, &cardimage[arr[53].numforimage]);
				outtextxy(925, 170, "主花色"); 
				Shuffle();//开始新的一局
				arr_loc = 0; //重置
				Desk_n = 0;
				char n[10]; sprintf_s(n, "%d", 54 - arr_loc);
				outtextxy(925, 220, "剩余牌数"); outtextxy(925, 270, n);
				//pl.Get_cards(); 
				//pc.Get_cards();
				outtextxy(300, 50, "对方手牌");
				outtextxy(300, 525, "我方手牌");
				int i = PVP(pl, pc);//获得本局胜家
				if (i == 2)//第一局，根据返回值获得胜家
				{
					putimage(300, 200, &lose);
					Sleep(2000);
					PVP_Turn(pl, pc);
				}
				else
				{
					putimage(300, 200, &win);
					Sleep(2000);
					PVP_Turn(pc, pl);//根据第一局胜家，进入接下来的几局
				}
			}
			else if (m.x >= 560 && m.x <= 760 && m.y <= 500 && m.y >= 400)

				return 0;

		}
	}

}
int PVP_Turn(Pc pc, Role pl)
{
	IMAGE win, lose, yesc, noc, bck;
	loadimage(&win, "G:\\ceshi\\win.jpg", 200, 100);
	loadimage(&lose, "G:\\ceshi\\lose.jpg", 200, 100);
	IMAGE cardimage[53];
	loadimage(&cardimage[53], "G:\\ceshi\\bigk.jpg", 75, 100);
	loadimage(&cardimage[0], "G:\\ceshi\\hk.jpg", 75, 100);
	loadimage(&cardimage[1], "G:\\ceshi\\ha.jpg", 75, 100);
	loadimage(&cardimage[2], "G:\\ceshi\\h2.jpg", 75, 100);
	loadimage(&cardimage[3], "G:\\ceshi\\h3.jpg", 75, 100);
	loadimage(&cardimage[4], "G:\\ceshi\\h4.jpg", 75, 100);
	loadimage(&cardimage[5], "G:\\ceshi\\h5.jpg", 75, 100);
	loadimage(&cardimage[6], "G:\\ceshi\\h6.jpg", 75, 100);
	loadimage(&cardimage[7], "G:\\ceshi\\h7.jpg", 75, 100);
	loadimage(&cardimage[8], "G:\\ceshi\\h8.jpg", 75, 100);
	loadimage(&cardimage[9], "G:\\ceshi\\h9.jpg", 75, 100);
	loadimage(&cardimage[10], "G:\\ceshi\\h10.jpg", 75, 100);
	loadimage(&cardimage[11], "G:\\ceshi\\hj.jpg", 75, 100);
	loadimage(&cardimage[12], "G:\\ceshi\\hq.jpg", 75, 100);
	loadimage(&cardimage[13], "G:\\ceshi\\bk.jpg", 75, 100);
	loadimage(&cardimage[14], "G:\\ceshi\\ba.jpg", 75, 100);
	loadimage(&cardimage[15], "G:\\ceshi\\b2.jpg", 75, 100);
	loadimage(&cardimage[16], "G:\\ceshi\\b3.jpg", 75, 100);
	loadimage(&cardimage[17], "G:\\ceshi\\b4.jpg", 75, 100);
	loadimage(&cardimage[18], "G:\\ceshi\\b5.jpg", 75, 100);
	loadimage(&cardimage[19], "G:\\ceshi\\b6.jpg", 75, 100);
	loadimage(&cardimage[20], "G:\\ceshi\\b7.jpg", 75, 100);
	loadimage(&cardimage[21], "G:\\ceshi\\b8.jpg", 75, 100);
	loadimage(&cardimage[22], "G:\\ceshi\\b9.jpg", 75, 100);
	loadimage(&cardimage[23], "G:\\ceshi\\b10.jpg", 75, 100);
	loadimage(&cardimage[24], "G:\\ceshi\\bj.jpg", 75, 100);
	loadimage(&cardimage[25], "G:\\ceshi\\bq.jpg", 75, 100);
	loadimage(&cardimage[26], "G:\\ceshi\\fk.jpg", 75, 100);
	loadimage(&cardimage[27], "G:\\ceshi\\fa.jpg", 75, 100);
	loadimage(&cardimage[28], "G:\\ceshi\\f2.jpg", 75, 100);
	loadimage(&cardimage[29], "G:\\ceshi\\f3.jpg", 75, 100);
	loadimage(&cardimage[30], "G:\\ceshi\\f4.jpg", 75, 100);
	loadimage(&cardimage[31], "G:\\ceshi\\f5.jpg", 75, 100);
	loadimage(&cardimage[32], "G:\\ceshi\\f6.jpg", 75, 100);
	loadimage(&cardimage[33], "G:\\ceshi\\f7.jpg", 75, 100);
	loadimage(&cardimage[34], "G:\\ceshi\\f8.jpg", 75, 100);
	loadimage(&cardimage[35], "G:\\ceshi\\f9.jpg", 75, 100);
	loadimage(&cardimage[36], "G:\\ceshi\\f10.jpg", 75, 100);
	loadimage(&cardimage[37], "G:\\ceshi\\fj.jpg", 75, 100);
	loadimage(&cardimage[38], "G:\\ceshi\\fq.jpg", 75, 100);
	loadimage(&cardimage[39], "G:\\ceshi\\rk.jpg", 75, 100);
	loadimage(&cardimage[40], "G:\\ceshi\\ra.jpg", 75, 100);
	loadimage(&cardimage[41], "G:\\ceshi\\r2.jpg", 75, 100);
	loadimage(&cardimage[42], "G:\\ceshi\\r3.jpg", 75, 100);
	loadimage(&cardimage[43], "G:\\ceshi\\r4.jpg", 75, 100);
	loadimage(&cardimage[44], "G:\\ceshi\\r5.jpg", 75, 100);
	loadimage(&cardimage[45], "G:\\ceshi\\r6.jpg", 75, 100);
	loadimage(&cardimage[46], "G:\\ceshi\\r7.jpg", 75, 100);
	loadimage(&cardimage[47], "G:\\ceshi\\r8.jpg", 75, 100);
	loadimage(&cardimage[48], "G:\\ceshi\\r9.jpg", 75, 100);
	loadimage(&cardimage[49], "G:\\ceshi\\r10.jpg", 75, 100);
	loadimage(&cardimage[50], "G:\\ceshi\\rj.jpg", 75, 100);
	loadimage(&cardimage[51], "G:\\ceshi\\rq.jpg", 75, 100);
	loadimage(&cardimage[52], "G:\\ceshi\\minik.jpg", 75, 100);
	loadimage(&bck, "G:\\ceshi\\greenbck.jpg", 1000, 600);
	loadimage(&yesc, "G:\\ceshi\\yesc.jpg", 100, 100);
	loadimage(&noc, "G:\\ceshi\\noc.jpg", 100, 100);
	putimage(0, 0, &bck);
	outtextxy(450, 300, "是否继续游戏？");
	putimage(430, 400, &yesc);
	putimage(560, 400, &noc);
	/*cout << "是否继续？0：no，1：yes";
	int in;
	cin >> in;
	if (!in)
		return 0;*/
	MOUSEMSG m;
	while (true)
	{
		m = GetMouseMsg();
		switch (m.uMsg)
		{
		case WM_LBUTTONDOWN:
			if (m.x >= 430 && m.x <= 530 && m.y <= 500 && m.y >= 400)
			{
				putimage(0, 0, &bck);
				outtextxy(300, 50, "对方手牌");
				outtextxy(300, 525, "我方手牌");
				m_col = arr[53].color;
				putimage(825, 170, &cardimage[arr[53].numforimage]); 
				outtextxy(925, 170, "主花色");
				Shuffle();//开始新的一局
				arr_loc = 0; //重置
				char n[10]; sprintf_s(n, "%d", 54 - arr_loc);
				outtextxy(925, 220, "剩余牌数"); outtextxy(925, 270, n);
				//pc.Get_cards();
				//pl.Get_cards();
				int i = PVP(pl, pc);//获得本局胜家
				if (i == 2)//第一局，根据返回值获得胜家
				{
					putimage(300, 200, &lose);
					Sleep(2000);
					PVP_Turn(pl, pc);
				}
				else
				{
					putimage(300, 200, &win);
					Sleep(2000);
					PVP_Turn(pc, pl);//根据第一局胜家，进入接下来的几局
				}
			}
			else if (m.x >= 560 && m.x <= 760 && m.y <= 500 && m.y >= 400)

				return 0;
		}
	}

}
int plVpc()
{
	IMAGE win, lose;
	loadimage(&win, "G:\\ceshi\\win.jpg", 200, 100);
	loadimage(&lose, "G:\\ceshi\\lose.jpg", 200, 100);
	IMAGE bck, mei, hong, hei, fang, emcard;
	initgraph(1000, 600);
	loadimage(&emcard, "G:\\ceshi\\cardback.jpg", 75, 100);
	loadimage(&bck, "G:\\ceshi\\greenbck.jpg", 1000, 600);
	loadimage(&mei, "G:\\ceshi\\mei.jpg", 1000, 600);
	loadimage(&hong, "G:\\ceshi\\hong.jpg", 1000, 600);
	loadimage(&fang, "G:\\ceshi\\fang.jpg", 1000, 600);
	loadimage(&hei, "G:\\ceshi\\hei.jpg", 1000, 600);
	IMAGE cardimage[53];
	loadimage(&cardimage[53], "G:\\ceshi\\bigk.jpg", 75, 100);
	loadimage(&cardimage[0], "G:\\ceshi\\hk.jpg", 75, 100);
	loadimage(&cardimage[1], "G:\\ceshi\\ha.jpg", 75, 100);
	loadimage(&cardimage[2], "G:\\ceshi\\h2.jpg", 75, 100);
	loadimage(&cardimage[3], "G:\\ceshi\\h3.jpg", 75, 100);
	loadimage(&cardimage[4], "G:\\ceshi\\h4.jpg", 75, 100);
	loadimage(&cardimage[5], "G:\\ceshi\\h5.jpg", 75, 100);
	loadimage(&cardimage[6], "G:\\ceshi\\h6.jpg", 75, 100);
	loadimage(&cardimage[7], "G:\\ceshi\\h7.jpg", 75, 100);
	loadimage(&cardimage[8], "G:\\ceshi\\h8.jpg", 75, 100);
	loadimage(&cardimage[9], "G:\\ceshi\\h9.jpg", 75, 100);
	loadimage(&cardimage[10], "G:\\ceshi\\h10.jpg", 75, 100);
	loadimage(&cardimage[11], "G:\\ceshi\\hj.jpg", 75, 100);
	loadimage(&cardimage[12], "G:\\ceshi\\hq.jpg", 75, 100);
	loadimage(&cardimage[13], "G:\\ceshi\\bk.jpg", 75, 100);
	loadimage(&cardimage[14], "G:\\ceshi\\ba.jpg", 75, 100);
	loadimage(&cardimage[15], "G:\\ceshi\\b2.jpg", 75, 100);
	loadimage(&cardimage[16], "G:\\ceshi\\b3.jpg", 75, 100);
	loadimage(&cardimage[17], "G:\\ceshi\\b4.jpg", 75, 100);
	loadimage(&cardimage[18], "G:\\ceshi\\b5.jpg", 75, 100);
	loadimage(&cardimage[19], "G:\\ceshi\\b6.jpg", 75, 100);
	loadimage(&cardimage[20], "G:\\ceshi\\b7.jpg", 75, 100);
	loadimage(&cardimage[21], "G:\\ceshi\\b8.jpg", 75, 100);
	loadimage(&cardimage[22], "G:\\ceshi\\b9.jpg", 75, 100);
	loadimage(&cardimage[23], "G:\\ceshi\\b10.jpg", 75, 100);
	loadimage(&cardimage[24], "G:\\ceshi\\bj.jpg", 75, 100);
	loadimage(&cardimage[25], "G:\\ceshi\\bq.jpg", 75, 100);
	loadimage(&cardimage[26], "G:\\ceshi\\fk.jpg", 75, 100);
	loadimage(&cardimage[27], "G:\\ceshi\\fa.jpg", 75, 100);
	loadimage(&cardimage[28], "G:\\ceshi\\f2.jpg", 75, 100);
	loadimage(&cardimage[29], "G:\\ceshi\\f3.jpg", 75, 100);
	loadimage(&cardimage[30], "G:\\ceshi\\f4.jpg", 75, 100);
	loadimage(&cardimage[31], "G:\\ceshi\\f5.jpg", 75, 100);
	loadimage(&cardimage[32], "G:\\ceshi\\f6.jpg", 75, 100);
	loadimage(&cardimage[33], "G:\\ceshi\\f7.jpg", 75, 100);
	loadimage(&cardimage[34], "G:\\ceshi\\f8.jpg", 75, 100);
	loadimage(&cardimage[35], "G:\\ceshi\\f9.jpg", 75, 100);
	loadimage(&cardimage[36], "G:\\ceshi\\f10.jpg", 75, 100);
	loadimage(&cardimage[37], "G:\\ceshi\\fj.jpg", 75, 100);
	loadimage(&cardimage[38], "G:\\ceshi\\fq.jpg", 75, 100);
	loadimage(&cardimage[39], "G:\\ceshi\\rk.jpg", 75, 100);
	loadimage(&cardimage[40], "G:\\ceshi\\ra.jpg", 75, 100);
	loadimage(&cardimage[41], "G:\\ceshi\\r2.jpg", 75, 100);
	loadimage(&cardimage[42], "G:\\ceshi\\r3.jpg", 75, 100);
	loadimage(&cardimage[43], "G:\\ceshi\\r4.jpg", 75, 100);
	loadimage(&cardimage[44], "G:\\ceshi\\r5.jpg", 75, 100);
	loadimage(&cardimage[45], "G:\\ceshi\\r6.jpg", 75, 100);
	loadimage(&cardimage[46], "G:\\ceshi\\r7.jpg", 75, 100);
	loadimage(&cardimage[47], "G:\\ceshi\\r8.jpg", 75, 100);
	loadimage(&cardimage[48], "G:\\ceshi\\r9.jpg", 75, 100);
	loadimage(&cardimage[49], "G:\\ceshi\\r10.jpg", 75, 100);
	loadimage(&cardimage[50], "G:\\ceshi\\rj.jpg", 75, 100);
	loadimage(&cardimage[51], "G:\\ceshi\\rq.jpg", 75, 100);
	loadimage(&cardimage[52], "G:\\ceshi\\minik.jpg", 75, 100);
	putimage(0, 0, &bck);
	outtextxy(300, 50, "对方手牌");
	outtextxy(300, 525, "我方手牌");
	Create_cards();
	/*for (int k = 0; k < 54; k++)
		cout << arr[k].color << arr[k].value << " ";*/
	Shuffle();
	arr_loc = 0; Desk_n = 0;
	Role player(1);
	Pc pc(2);
	player.Get_cards();
	pc.Get_cards();
	int m_loc = choose_m_col_1();
	int m_num = choosenum(); 
	putimage(825, 170, &cardimage[m_num]);
	outtextxy(925, 170, "主花色"); char n[10]; sprintf_s(n, "%d", 54 - arr_loc);
	outtextxy(925, 220, "剩余牌数"); outtextxy(925, 270, n);
	/*putimage(75, 170, &emcard);
	outtextxy(25, 170, "牌堆");*/
	int Turn_1_winner;
	if (m_loc < 8)
		Turn_1_winner = PVP(pc, player);
	else
		Turn_1_winner = PVP(player, pc);

	if (Turn_1_winner == 2)//第一局，根据返回值获得胜家
	{
		putimage(300, 200, &lose);
		Sleep(2000);
		PVP_Turn(player, pc);
	}
	else
	{
		putimage(300, 200, &win);
		Sleep(2000);
		PVP_Turn(pc, player);//根据第一局胜家，进入接下来的几局
	}
	player.~Role();
	pc.~Pc();
	system("pause");
	return 0;
}

//int PVP_Turn(Role r_1, Role r_2);
//int PVP_Turn_1(Role r_1, Role r_2);
//int PVP(Role r_1, Role r_2)
//{
//	if (arr_loc == 54 && r_1.Get_num() == 1 &&Desk_n==0)//牌堆耗尽，桌面无牌，此时若进攻方手中仅余一张，
//	{										//为鬼牌，则进攻方判输
//		if (r_1.joker_left() == 1)			
//			return r_2.Get_NO();
//	}
//	r_1.Show(); 
//	int k;
//	for ( k = r_1.Attack(r_1.att_get()); k ==0;k=r_1.Attack(r_1.att_get()));//k=1或2，2：放弃出牌
//	if (k == 2)
//		{
//			r_1.Get_cards();
//			r_2.Get_cards();
//			Desk_clean();
//			return PVP(r_2, r_1);
//		}
//	Desk_Show();
//	if (arr_loc == 54 && r_1.Get_num() == 0)//r_1获胜
//		return r_1.Get_NO();
//	r_2.Show();
//	int def = r_2.Defend(r_2.def_get());
//	Desk_Show();
//	int i = r_2.Check_self(def);
//	r_1.Check_defender(i);
//	if (i == 2){r_2.Get_cards(); Desk_clean();}
//	if (arr_loc == 54 && r_2.Get_num() == 0)//经过上次判定，r_1在本回合不会获胜
//		return r_2.Get_NO();				//此时判断r_2是否在本回合获胜
//	switch (i)
//		{
//		case 0:return PVP(r_1, r_2);
//		case 1:return PVP(r_1, r_2);
//		case 2:return PVP(r_2, r_1);
//		}
//}
//int PVP_Turn_1(Role r_1, Role r_2)//返回本局胜者号
//{
//	r_1.Get_cards();
//	r_2.Get_cards();
//	int m_loc = choose_m_col_1();
//	cout << endl << "主花色" << m_col<<endl;
//	if (m_loc < 8)
//		return PVP(r_2, r_1);
//	else
//		return PVP(r_1, r_2);
//}
//int PVP_Turn(Role r_1, Role r_2)
//{
//	cout << "是否继续？0：no，1：yes";
//	int in;
//	cin >> in;
//	if (!in)
//		return 0;
//	Shuffle();//开始新的一局
//	arr_loc = 0; //重置
//	Desk_n = 0;
//	m_col = arr[53].color;
//	cout << endl << "主花色" << m_col << endl;
//	r_1.Get_cards(); r_2.Get_cards();
//	int i=PVP(r_1, r_2);//获得本局胜家
//	cout << i << "  win" << endl;
//	if (i == r_1.Get_NO())
//	{
//		return PVP_Turn(r_1, r_2);
//	}
//	else
//	{
//		return PVP_Turn(r_2, r_1);
//	}
//}
//int plVpl()
//{
//	
//	Create_cards(); cout << "生成牌堆"<<endl;
//	for (int k = 0; k < 54; k++)
//		cout << arr[k].color << arr[k].value << " ";
//	cout << '\n';
//	Shuffle();
//	cout << "打乱牌堆" << endl;
//	for (int k = 0; k < 54; k++)
//		cout <<arr[k].color<< arr[k].value << " ";
//	cout << '\n';
//	Role player(1);
//	Role pc(2);
//	if (PVP_Turn_1(player, pc) == 1)//进入第一局，根据返回值获得胜家
//	{
//		cout << "1 win" << endl; PVP_Turn(player, pc);//根据第一局胜家，进入接下来的几局
//	}
//	else
//	{
//		cout << "2 win" << endl; PVP_Turn(pc, player);//根据第一局胜家，进入接下来的几局
//	}
//	player.~Role();
//	pc.~Role();
//	system("pause");
//	return 0;
//}

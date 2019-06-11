#include "Role.h"
#include <graphics.h>
#include <conio.h>
#include<iostream>
using namespace std;
extern char m_col;
extern card  arr[54];//牌堆
extern card  Desk[54];//桌面上的牌
extern int arr_loc;//牌堆顶部对应数组位置
extern int Desk_n;//当前桌面上牌数


Role::Role(int i)
{
	card Cards[30];//手牌上限30张
	num = 0;
	NO = i;
}


Role::~Role()
{
	cout <<NO<< " end"<<endl;
}
void Role::clean() {
	IMAGE bck;
	loadimage(&bck, "G:\\ceshi\\greenbck.jpg", 400,100);
	putimage(390,250, &bck);

}
void Role::Sequence()
{
	for (int i = 0; i < num; i++)
		for (int j = 0; j < num - 1 - i; j++)
			if (Cards[j].value > Cards[j + 1].value)
				Swap(&Cards[j], &Cards[j + 1]);
}
void Role::Get_cards() //i为牌堆现在堆顶牌所在数组位置
{
	if (num < 8)
		for (; num < 8 && arr_loc < 54; num++, arr_loc++)//手牌补到8张 或者 牌堆耗尽
		{
			agetb(&Cards[num], arr [arr_loc]);
		}
	Sequence();
}
void Role::GetBack_cards()//*i为桌面上已有牌数
{
	for (; Desk_n > 0; Desk_n--)
	{
		agetb(&Cards[num] , Desk[Desk_n - 1]);
		num++;
	}
	Desk_clean();
	Sequence();
}
void Role::Showem()
{
	IMAGE  emcard,bck;
	loadimage(&emcard, "G:\\ceshi\\cardback.jpg", 75, 100);
	loadimage(&bck, "G:\\ceshi\\greenbck.jpg", 600,100);
	putimage(375, 10, &bck);
	for (int i = 0; i < num; i++)
	{
		putimage(375 + i * 30, 10, &emcard);
	}
//{
//	IMAGE bck, no, chu;
//	loadimage(&bck, "G:\\ceshi\\greenbck.jpg", 600, 100);
//	loadimage(&no, "G:\\ceshi\\no.jpg", 70, 30);
//	loadimage(&chu, "G:\\ceshi\\chu.jpg", 70, 30);
//	putimage(375, 490, &bck);
//	putimage(450, 425, &no);
//	putimage(525, 425, &chu);
//	IMAGE cardimage[54];
//	loadimage(&cardimage[53], "G:\\ceshi\\bigk.jpg", 75, 100);
//	loadimage(&cardimage[0], "G:\\ceshi\\hk.jpg", 75, 100);
//	loadimage(&cardimage[1], "G:\\ceshi\\ha.jpg", 75, 100);
//	loadimage(&cardimage[2], "G:\\ceshi\\h2.jpg", 75, 100);
//	loadimage(&cardimage[3], "G:\\ceshi\\h3.jpg", 75, 100);
//	loadimage(&cardimage[4], "G:\\ceshi\\h4.jpg", 75, 100);
//	loadimage(&cardimage[5], "G:\\ceshi\\h5.jpg", 75, 100);
//	loadimage(&cardimage[6], "G:\\ceshi\\h6.jpg", 75, 100);
//	loadimage(&cardimage[7], "G:\\ceshi\\h7.jpg", 75, 100);
//	loadimage(&cardimage[8], "G:\\ceshi\\h8.jpg", 75, 100);
//	loadimage(&cardimage[9], "G:\\ceshi\\h9.jpg", 75, 100);
//	loadimage(&cardimage[10], "G:\\ceshi\\h10.jpg", 75, 100);
//	loadimage(&cardimage[11], "G:\\ceshi\\hj.jpg", 75, 100);
//	loadimage(&cardimage[12], "G:\\ceshi\\hq.jpg", 75, 100);
//	loadimage(&cardimage[13], "G:\\ceshi\\bk.jpg", 75, 100);
//	loadimage(&cardimage[14], "G:\\ceshi\\ba.jpg", 75, 100);
//	loadimage(&cardimage[15], "G:\\ceshi\\b2.jpg", 75, 100);
//	loadimage(&cardimage[16], "G:\\ceshi\\b3.jpg", 75, 100);
//	loadimage(&cardimage[17], "G:\\ceshi\\b4.jpg", 75, 100);
//	loadimage(&cardimage[18], "G:\\ceshi\\b5.jpg", 75, 100);
//	loadimage(&cardimage[19], "G:\\ceshi\\b6.jpg", 75, 100);
//	loadimage(&cardimage[20], "G:\\ceshi\\b7.jpg", 75, 100);
//	loadimage(&cardimage[21], "G:\\ceshi\\b8.jpg", 75, 100);
//	loadimage(&cardimage[22], "G:\\ceshi\\b9.jpg", 75, 100);
//	loadimage(&cardimage[23], "G:\\ceshi\\b10.jpg", 75, 100);
//	loadimage(&cardimage[24], "G:\\ceshi\\bj.jpg", 75, 100);
//	loadimage(&cardimage[25], "G:\\ceshi\\bq.jpg", 75, 100);
//	loadimage(&cardimage[26], "G:\\ceshi\\fk.jpg", 75, 100);
//	loadimage(&cardimage[27], "G:\\ceshi\\fa.jpg", 75, 100);
//	loadimage(&cardimage[28], "G:\\ceshi\\f2.jpg", 75, 100);
//	loadimage(&cardimage[29], "G:\\ceshi\\f3.jpg", 75, 100);
//	loadimage(&cardimage[30], "G:\\ceshi\\f4.jpg", 75, 100);
//	loadimage(&cardimage[31], "G:\\ceshi\\f5.jpg", 75, 100);
//	loadimage(&cardimage[32], "G:\\ceshi\\f6.jpg", 75, 100);
//	loadimage(&cardimage[33], "G:\\ceshi\\f7.jpg", 75, 100);
//	loadimage(&cardimage[34], "G:\\ceshi\\f8.jpg", 75, 100);
//	loadimage(&cardimage[35], "G:\\ceshi\\f9.jpg", 75, 100);
//	loadimage(&cardimage[36], "G:\\ceshi\\f10.jpg", 75, 100);
//	loadimage(&cardimage[37], "G:\\ceshi\\fj.jpg", 75, 100);
//	loadimage(&cardimage[38], "G:\\ceshi\\fq.jpg", 75, 100);
//	loadimage(&cardimage[39], "G:\\ceshi\\rk.jpg", 75, 100);
//	loadimage(&cardimage[40], "G:\\ceshi\\ra.jpg", 75, 100);
//	loadimage(&cardimage[41], "G:\\ceshi\\r2.jpg", 75, 100);
//	loadimage(&cardimage[42], "G:\\ceshi\\r3.jpg", 75, 100);
//	loadimage(&cardimage[43], "G:\\ceshi\\r4.jpg", 75, 100);
//	loadimage(&cardimage[44], "G:\\ceshi\\r5.jpg", 75, 100);
//	loadimage(&cardimage[45], "G:\\ceshi\\r6.jpg", 75, 100);
//	loadimage(&cardimage[46], "G:\\ceshi\\r7.jpg", 75, 100);
//	loadimage(&cardimage[47], "G:\\ceshi\\r8.jpg", 75, 100);
//	loadimage(&cardimage[48], "G:\\ceshi\\r9.jpg", 75, 100);
//	loadimage(&cardimage[49], "G:\\ceshi\\r10.jpg", 75, 100);
//	loadimage(&cardimage[50], "G:\\ceshi\\rj.jpg", 75, 100);
//	loadimage(&cardimage[51], "G:\\ceshi\\rq.jpg", 75, 100);
//	loadimage(&cardimage[52], "G:\\ceshi\\minik.jpg", 75, 100);
//	for (int i = 0; i < num; i++)
//	{
//		int y = Cards[i].numforimage;
//		putimage(375 + i * 30, 10, &cardimage[y]);
//	}
//}
}
void Role::Show()
{
	IMAGE bck,no,chu;
	loadimage(&bck, "G:\\ceshi\\greenbck.jpg", 600,100);
	loadimage(&no, "G:\\ceshi\\no.jpg",70,30);
	loadimage(&chu, "G:\\ceshi\\chu.jpg", 70, 30);
	putimage(375, 490, &bck);
	putimage(450, 425, &no);
	putimage(525, 425, &chu);
	IMAGE cardimage[54];
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
	loadimage(&cardimage[23], "G:\\ceshi\\b10.jpg",75, 100);
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
	for (int i = 0; i <num; i++)
	{
		int y = Cards[i].numforimage;
		putimage(375 + i * 30, 490, &cardimage[y]);
	}
	//putimage(330 , 490, &cardimage[53]);
}
bool Role::a_judge( int i, card hand)//不对Desk_n直接操作
{
	for (; i > 0; i--)
		if (hand.value == Desk[i - 1].value)
			return 1;
	return 0;
}
void Role::card_out(int loc)//获得loc为X-1；第X张牌打出
{
	for (; loc < num-1; loc++)
		agetb(&Cards[loc], Cards[(loc + 1)]);//左移数组
		num--;							//手牌数-1
}
int Role::Attack( int loc)
{
	if (loc == 0)//选择不出
	{
		if (Desk_n>0)
			return 2;
		else
			return 0;//桌面无牌，此时必须出牌，输入0无效
	}
	if (Cards[loc-1].color == 'j')//不可打出鬼牌
		return 0;
	bool b;
	if (Desk_n == 0)
		b = 1;	//桌面无牌，直接打出
	else
		 b=a_judge( Desk_n, Cards[loc-1]);//判断是否可以打出
	if (b)
	{
		agetb(&Desk[Desk_n], Cards[loc - 1]);//桌面获得牌
		Desk_n++;							//桌面牌数+1
		card_out(loc - 1);					//除去该手牌
		return 1;
	}
	else
		return 0;
}
bool Role::d_judge(card desk, card hand)
{
	if (hand.color == 'j')//鬼牌
		return 1;
	if (hand.color == m_col && desk.color != m_col)//主牌
		return 1;
	if (hand.color == desk.color && hand.value > desk.value)//同花色比大小
		return 1;
	return 0;	
}
int Role::Defend(int loc)

{
	if (loc == 0){return 0;}
	bool b = d_judge(Desk[Desk_n - 1], Cards[loc - 1]);
	if (b)
	{
		agetb(&Desk[Desk_n], Cards[loc - 1]);
		Desk_n++;
		card_out(loc - 1);
	}
	else
	{	
		return 3;
	}
	if (num == 0)
	{	
		return 2;//无手牌,过牌
	}
	else
		return 1;//尚有手牌
}
int Role::Check_self(int i)//return !=3
{
	switch (i)
	{
	case 0:GetBack_cards();  Desk_clean(); return 0;
	case 1:break;
	case 2:break;
	case 3:
		cout << "本次选择无效";
		Check_self(i=Defend( def_get()));//choose_again
		break;
	}
	return i;
}
int Role::Check_defender(int i)
{
	switch (i)
	{
	case 0:Get_cards(); break;// 本轮结束，保持进攻
	case 1:break;//
	case 2:Get_cards();  break;//本轮结束，改变攻守
	}
	return i;
}
int Role::att_get() {
	MOUSEMSG m;	
	
	while (true)
	{
		// 获取一条鼠标消息
		m = GetMouseMsg();

		switch (m.uMsg)
		{
		case WM_LBUTTONDOWN:
			for (int i = 1; i <=num; i++)
				if (m.x >= 345 + i * 30 && m.x <= 375 + i * 30 && m.y <= 490 + 75 && m.y >= 490)
				{    
					/*putimage(375, 465, &bck);
					for (int k = 0; k < num; k++)
					{

						int y = Cards[k].numforimage;
						if (k == i-1)
						{
							putimage(375 + k * 30, 465, &cardimage[y]);
						}
						else
						{
							putimage(375 + k * 30, 490, &cardimage[y]);
						}*/
						//if (m.x >= 525 && m.x <= 525 + 70 && m.y <= 455 && m.y >= 425)
						{
							return i;
						}
					

					
				}
			if (m.x >= 450 && m.x <= 450 + 70 && m.y <= 455 && m.y >= 425)
			{
				return 0;
			}
				
		}
	}
	//return 123;

}
int Role::def_get() {

	MOUSEMSG m;

	int ia;
	while (true)
	{
		// 获取一条鼠标消息
		m = GetMouseMsg();

		switch (m.uMsg)
		{
		case WM_LBUTTONDOWN:
			for (int i = 1; i <= num; i++)
				if (m.x >= 345 + i * 30 && m.x <= 375 + i * 30 && m.y <= 490 + 75 && m.y >= 490)
				{
					ia = i;
					  /*putimage(375, 465, &bck);
					for (int k = 0; k < num; k++)
					{

						int y = Cards[k].numforimage;
						if (k == i-1
							)
						{
							putimage(375 + k * 30, 465, &cardimage[y]);
						}
						else
						{
							putimage(375 + k * 30, 490, &cardimage[y]);
						}*/
						//if (m.x >= 525 && m.x <= 525 + 70 && m.y <= 455 && m.y >= 425)
						{   
							
							return ia;
						}
					}
					
					
				
			if (m.x >= 450 && m.x <= 450 + 70 && m.y <= 455 && m.y >= 425)
			{
				return 0;
			}
		}
	}
	//return 123;
}

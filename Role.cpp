#include "Role.h"


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
void Role::Show()
{
	cout << NO << "号角色 ";
	for (int i = 0; i < num; i++)
		cout << Cards[i].color << Cards[i].value << " ";
	cout << endl;
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
			return 0;//桌面无牌，此时必须出牌
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
	if (loc == 0)
	{
		return 0;
	}
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
		return 2;//无手牌,选择过牌或收牌
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
	case 2:cout << "是否主动收牌?\n0:收" << endl; int i; cin >> i; cout << endl;
		if (i == 0)
		{
			return Check_self(0);
		}
		else
			return 2;
	case 3:
		cout << "本次选择无效";
		Check_self(Defend(i = def_get()));//choose_again
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

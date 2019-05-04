#include"Role.h" 
#include"Pc.h"
#include"pvp.h"
#include<iostream>
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
int PVP_Turn(Role r_1, Role r_2);
int PVP_Turn_1(Role r_1, Role r_2);
int PVP(Role r_1, Role r_2)
{
	if (arr_loc == 54 && r_1.Get_num() == 1 &&Desk_n==0)//牌堆耗尽，桌面无牌，此时若进攻方手中仅余一张，
	{										//为鬼牌，则进攻方判输
		if (r_1.joker_left() == 1)			
			return r_2.Get_NO();
	}
	r_1.Show(); 
	int k;
	for ( k = r_1.Attack(r_1.att_get()); k ==0;k=r_1.Attack(r_1.att_get()));//k=1或2，2：放弃出牌
	if (k == 2)
		{
			r_1.Get_cards();
			r_2.Get_cards();
			Desk_clean();
			return PVP(r_2, r_1);
		}
	Desk_Show();
	if (arr_loc == 54 && r_1.Get_num() == 0)//r_1获胜
		return r_1.Get_NO();
	r_2.Show();
	int def = r_2.Defend(r_2.def_get());
	Desk_Show();
	int i = r_2.Check_self(def);
	r_1.Check_defender(i);
	if (i == 2){r_2.Get_cards(); Desk_clean();}
	if (arr_loc == 54 && r_2.Get_num() == 0)//经过上次判定，r_1在本回合不会获胜
		return r_2.Get_NO();				//此时判断r_2是否在本回合获胜
	switch (i)
		{
		case 0:return PVP(r_1, r_2);
		case 1:return PVP(r_1, r_2);
		case 2:return PVP(r_2, r_1);
		}
}
int PVP_Turn_1(Role r_1, Role r_2)//返回本局胜者号
{
	r_1.Get_cards();
	r_2.Get_cards();
	int m_loc = choose_m_col_1();
	cout << endl << "主花色" << m_col<<endl;
	if (m_loc < 8)
		return PVP(r_2, r_1);
	else
		return PVP(r_1, r_2);
}
int PVP_Turn(Role r_1, Role r_2)
{
	cout << "是否继续？0：no，1：yes";
	int in;
	cin >> in;
	if (!in)
		return 0;
	Shuffle();//开始新的一局
	arr_loc = 0; //重置
	Desk_n = 0;
	m_col = arr[53].color;
	cout << endl << "主花色" << m_col << endl;
	int i=PVP(r_1, r_2);//获得本局胜家
	cout << i << "  win" << endl;
	if (i == r_1.Get_NO())
	{
		return PVP_Turn(r_1, r_2);
	}
	else
	{
		return PVP_Turn(r_2, r_1);
	}
}
int plVpl()
{
	
	Creat_cards(); cout << "生成牌堆"<<endl;
	for (int k = 0; k < 54; k++)
		cout << arr[k].color << arr[k].value << " ";
	cout << '\n';
	Shuffle();
	cout << "打乱牌堆" << endl;
	for (int k = 0; k < 54; k++)
		cout <<arr[k].color<< arr[k].value << " ";
	cout << '\n';
	Role player(1);
	Role pc(2);
	if (PVP_Turn_1(player, pc) == 1)//进入第一局，根据返回值获得胜家
	{
		cout << "1 win" << endl; PVP_Turn(player, pc);//根据第一局胜家，进入接下来的几局
	}
	else
	{
		cout << "2 win" << endl; PVP_Turn(pc, player);//根据第一局胜家，进入接下来的几局
	}
	player.~Role();
	pc.~Role();
	system("pause");
	return 0;
}

int PVP(Role pl, Pc pc)
{
	if (arr_loc == 54 && pl.Get_num() == 1 && Desk_n == 0)//牌堆耗尽，桌面无牌，此时若进攻方手中仅余一张，
	{										//为鬼牌，则进攻方判输
		if (pl.joker_left() == 1)
			return 2;
	}
	pl.Show();
	int k;
	for (k = pl.Attack(pl.att_get()); k == 0; k = pl.Attack(pl.att_get()));//k=1或2，2：放弃出牌
	if (k == 2)
	{
		pl.Get_cards();
		pc.Get_cards();
		Desk_clean();
		return PVP(pc, pl);
	}
	Desk_Show();
	if (arr_loc == 54 && pl.Get_num() == 0)
		return 1;
	pc.Show();
	int def = pc.Defend();////////////修改为pc压牌操作
	Desk_Show();
	pc.Check_self(def);
	pl.Check_defender(def); 
	if (def == 2) { pc.Get_cards(); Desk_clean(); }
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
	if (arr_loc == 54 && pc.Get_num() == 1 && Desk_n == 0)//牌堆耗尽，桌面无牌，此时若进攻方手中仅余一张，
	{										//为鬼牌，则进攻方判输
		if (pc.joker_left() == 1)
			return 1;
	}
	pc.Show();
	////////////////改为pc出牌操作
	int k = pc.Attack();//k=1或2，2：放弃出牌
	if (k == 2)
	{
		pc.Get_cards();
		pl.Get_cards();
		Desk_clean();
		return PVP(pl, pc);
	}
	Desk_Show();
	if (arr_loc == 54 && pc.Get_num() == 0)//pc win
		return 2;
	pl.Show();
	int def = pl.Defend(pl.def_get());
	Desk_Show();
	int i = pl.Check_self(def);
	pc.Check_defender(i);
	if (i == 2) { pl.Get_cards(); Desk_clean(); }
	if (arr_loc == 54 && pl.Get_num() == 0)//经过上次判定，r_1在本回合不会获胜
		return 1;				//此时判断r_2是否在本回合获胜
	switch (i)
	{
	case 0:return PVP(pc, pl);
	case 1:return PVP(pc, pl);
	case 2:return PVP(pl, pc);
	}
}
int PVP_Turn(Role pl, Pc pc)
{
	cout << "是否继续？0：no，1：yes";
	int in;
	cin >> in;
	if (!in)
		return 0;
	Shuffle();//开始新的一局
	arr_loc = 0; //重置
	Desk_n = 0;
	m_col = arr[53].color;
	cout << endl << "主花色" << m_col << endl;
	int i = PVP(pl, pc);//获得本局胜家
	cout << i << "  win" << endl;
	if (i == 1)
	{
		return PVP_Turn(pl, pc);
	}
	else
	{
		return PVP_Turn(pc, pl);
	}
}
int PVP_Turn(Pc pc, Role pl)
{
	cout << "是否继续？0：no，1：yes";
	int in;
	cin >> in;
	if (!in)
		return 0;
	Shuffle();//开始新的一局
	arr_loc = 0; //重置
	Desk_n = 0;
	m_col = arr[53].color;
	cout << endl << "主花色" << m_col << endl;
	int i = PVP(pc, pl);//获得本局胜家
	cout << i << "  win" << endl;
	if (i == 1)
	{
		return PVP_Turn(pl, pc);
	}
	else
	{
		return PVP_Turn(pc, pl);
	}
}
int plVpc()
{
	Creat_cards(); cout << "生成牌堆" << endl;
	for (int k = 0; k < 54; k++)
		cout << arr[k].color << arr[k].value << " ";
	cout << '\n';
	Shuffle();
	cout << "打乱牌堆" << endl;
	for (int k = 0; k < 54; k++)
		cout << arr[k].color << arr[k].value << " ";
	cout << '\n';
	Role player(1);
	Pc pc(2);
	
	player.Get_cards();
	pc.Get_cards();
	int m_loc = choose_m_col_1();
	cout << endl << "主花色" << m_col << endl;
	int Turn_1_winner;
	if (m_loc < 8)
		Turn_1_winner=PVP(pc, player);
	else
		Turn_1_winner=PVP(player, pc);

	if (Turn_1_winner == 1)//第一局，根据返回值获得胜家
	{
		cout << "victory" << endl; PVP_Turn(player, pc);//根据第一局胜家，进入接下来的几局
	}
	else
	{
		cout << "defeat" << endl; PVP_Turn(pc, player);//根据第一局胜家，进入接下来的几局
	}

	player.~Role();
	pc.~Pc();
	system("pause");
	return 0;
}

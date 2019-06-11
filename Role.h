#pragma once
#include"card.h"
#include<iostream>
using namespace std;
extern char m_col;
extern card  arr[54];//牌堆
extern card  Desk[54];//桌面上的牌
extern int arr_loc;//牌堆顶部对应数组位置
extern int Desk_n;//当前桌面上牌数
class Role
{
	friend class Scene;
private:
	

	int num;  //手牌数
	int NO;						//角色编号
public:
	Role(int i);
	~Role();
	card  Cards[54] ;//手牌
	void Sequence();
	char Get_col(int i) { return Cards[i - 1].color; }
	int  Get_val(int i) { return Cards[i - 1].value; }
	int Get_num() { return num; }
	int Get_NO() { return NO; }
	bool joker_left() { return (Cards[0].color == 'j'); }
    void Get_cards();//牌堆
	void GetBack_cards();//桌面牌组
	void Show();
	void Showem();
	void clean();
	bool a_judge(int i,card hand);//桌面牌组,数量，将要打出手牌
	bool d_judge(card desk, card hand);
	int Attack(int loc);//桌面牌组，桌面牌数量，手牌的位置
	int Defend(int loc);//返回0，直接收牌；返回1，防守成功；
						//返回2，手牌耗尽，直接结束本轮,选择过牌或收牌
						//返回3, 本次选择无效
	void card_out(int loc);//获得loc为X-1；第X张牌打出
	//待修改版本↓
	int att_get();
	//待修改版本↓
	int def_get();
	int Check_self(int i);//return !=3
	int Check_defender(int i);

};


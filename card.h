#pragma once
struct card
{
	char color='r';               //花色
	int value='0';					//大小				
};
extern char m_col;//主花色
extern card  arr[54];//牌堆
extern card  Desk[54];//桌面上的牌
extern int arr_loc ;//牌堆顶部对应数组位置
extern int Desk_n ;//当前桌面上牌数
void Desk_clean();//清空桌面牌组,
int choose_m_col_1();//第一局确定主花色。返回该牌位置。以便展示该牌
void choose_m_col();//后续几局获得牌堆最后一张牌，花色为主花色。以便展示该牌
void Swap(card a[], card b[]);
void Creat_cards();
void Shuffle();
void agetb(card* a, card b);
void Desk_Show();

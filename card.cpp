#include<iostream>
#include<time.h>
using namespace std;
struct card 
{ char color='r';               //花色
int value='0';					//大小				
};
char m_col;//主花色
card  arr[54];//牌堆
card  Desk[54];//桌面上的牌
int arr_loc = 0;//牌堆顶部对应数组位置
int Desk_n = 0;//当前桌面上牌数
void Desk_clean()
{
	Desk_n = 0;
}
void agetb(card *a,card b)
{
	a->color = b.color;
	a->value = b.value;
}
//交换
void Swap(card a[], card b[])
{
	card *temp=new card [8];
	*temp = *a;
	*a = *b;
	*b = *temp;
	delete[]temp;
	
}
//生成牌堆
void Creat_cards()
{
	for (int i = 0; i < 52; i++)
	{
		int col = i / 13;
		int val = i % 13;
		if (val == 1)
			arr[i].value = 14;//A(14)>K(13)>Q(12)>J(11)>10>....>2
		else if (val == 0)
			arr[i].value = 13;//K
		else
			arr[i].value = val;
		switch (col)
		{
		case 0:arr[i].color = 'r'; break;//红心
		case 1:arr[i].color = 'b'; break;//黑桃
		case 2:arr[i].color = 's'; break;//方片
		case 3:arr[i].color = 'f'; break;//梅花
		}
	}
	arr[52].color = 'j'; arr[52].value = 19;//小鬼
	arr[53].color = 'j'; arr[53].value = 20;//大鬼
}
//洗牌
void Shuffle()				//洗牌
{
	for (int i = 53; i >= 0; i--)
	{
		srand((unsigned)time(NULL));
		//Swap(&arr[i], &arr[(rand() + i) % 53]);
		Swap(&arr[rand() % (i + 1)], &arr[i]);//随机交换
	}
}
int choose_m_col_1()
{
	int i;
	do 
	{ srand((unsigned)time(NULL)); 
	i = rand() % 16;//0-15
	m_col = arr[i].color;
	}
	while (m_col == 'j');//主花色不可为鬼牌
	return i;
};
void choose_m_col()
{
	do {
		Shuffle();
		m_col = arr[53].color;
	} while (m_col == 'j');//不可为鬼牌
};
void Desk_Show()
{
	cout << "桌面牌组 ";
	if (Desk_n > 0)
	{
		for (int i = 0; i < Desk_n; i++)
			cout << Desk[i].color << Desk[i].value << " ";
	}
	cout << endl;
}
//int main()
//{
//	card a, b;
//	a.value = 1; b.value = 2;
//	Swap(&a,& b);
//	cout << a.value;
//	system("pause");
//	return 0;
//}

#include<iostream>
#include<time.h>
using namespace std;
struct card 
{ char color;               //花色
int value;					//大小				
};
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
void Creat_cards(card arr[])
{
	for (int i = 0; i < 52; i++)
	{
		int col = (i+1) / 13;
		int val = (i+1) % 13;
		if (val == 1)
			arr[i].value = 14;//A>K>Q>J>10>....>2
		else if (val == 0)
			arr[i].value = 13;//K
		else
			arr[i].value = val;
		switch (col)
		{
		case 0:arr[i].color = 'r';//红心
		case 1:arr[i].color = 'b';//黑桃
		case 2:arr[i].color = 's';//方片
		case 3:arr[i].color = 'f';//梅花
		}
	}
	arr[52].color = 'j'; arr[52].value = 19;//小鬼
	arr[53].color = 'j'; arr[53].value = 20;//大鬼
}
//洗牌
void Shuffle(card  arr[])				//洗牌
{
	for (int i = 53; i >= 0; i--)
	{
		srand((unsigned)time(NULL));
		//Swap(&arr[i], &arr[(rand() + i) % 53]);
		Swap(&arr[rand() % (i + 1)], &arr[i]);//随机交换
	}
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

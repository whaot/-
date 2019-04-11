#pragma once
struct card
{
	char color;               //花色
	int value;					//大小				
};
void Swap(card, card);
void Creat_cards(card arr[]);
void Shuffle(card  arr[]);
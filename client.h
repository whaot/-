#pragma once
#include"Role.h" 
#include"Pc.h"
#include"pvp.h"
#include "winsock2.h"
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996) 
#include<iostream>
using namespace std;
extern char m_col;
extern card  arr[54];//牌堆
extern card  Desk[54];//桌面上的牌
extern int arr_loc;//牌堆顶部对应数组位置
extern int Desk_n;//当前桌面上牌数
const int BUF_SIZE = 128;
WSADATA wsd; //WSADATA变量
SOCKET sHost; //服务器套接字
SOCKADDR_IN servAddr; //服务器地址
char buf[BUF_SIZE]; //接收数据缓冲区
char bufRecv[BUF_SIZE];
int retVal; //返回值
			//初始化套结字动态库
int get_arr()
{
	ZeroMemory(buf, 128);
	buf[0] = '1';
	retVal = send(sHost, buf, strlen(buf), 0);
	if (SOCKET_ERROR == retVal)
	{
		cout << "send failed!" << endl;
		closesocket(sHost); //关闭套接字
		WSACleanup(); //释放套接字资源
		return -1;
	}
	//RecvLine(sHost, bufRecv);
	ZeroMemory(bufRecv, 128);
	recv(sHost, bufRecv, 128, 0); // 接收服务器端的数据
	//获得bufRecv转为牌组
	for (int k = 0, i = 0; k < 54; k++)
	{
		arr[k].color = bufRecv[i++];
		arr[k].value = bufRecv[i++];
	}
	return 0;
}
int PVP_s(Role ser, Role cli);
int PVP_c(Role cli, Role ser);
int PVP_Turn_s(Role ser, Role cli);
int PVP_Turn_c(Role cli, Role ser);
int PVP_s(Role ser, Role cli)
{
	if (arr_loc == 54 && ser.Get_num() == 1 && Desk_n == 0)//牌堆耗尽，桌面无牌，此时若进攻方手中仅余一张，
	{										//为鬼牌，则进攻方判输
		if (ser.joker_left() == 1)
			return cli.Get_NO();
	}
	//向服务器发送数据
	ZeroMemory(buf, BUF_SIZE);
	buf[0]=0;
	retVal = send(sHost, buf, strlen(buf), 0);
	if (SOCKET_ERROR == retVal)
	{
		cout << "send failed!" << endl;
		closesocket(sHost); //关闭套接字
		WSACleanup(); //释放套接字资源
		return -1;
	}
	ZeroMemory(bufRecv, BUF_SIZE);
	recv(sHost, bufRecv, BUF_SIZE, 0); // 接收服务器端的数据
	//cout << endl << "从服务器接收数据：" << bufRecv << endl;
	int k= ser.Attack(bufRecv[0]);//k=1或2，2：放弃出牌
	if (k == 2)
	{
		ser.Get_cards();
		cli.Get_cards();
		Desk_clean();
		return PVP_c(cli, ser);
	}
	Desk_Show();
	if (arr_loc == 54 && ser.Get_num() == 0)
		return ser.Get_NO();
	cli.Show();
	int def,cli_out_num;
	for (def = cli.Defend(cli_out_num = cli.def_get()); def == 3; def = cli.Defend(cli_out_num = cli.def_get()));//def!=3
		//向服务器发送数据
	ZeroMemory(buf, BUF_SIZE);
	buf[0] = cli_out_num;//发送
	retVal = send(sHost, buf, strlen(buf), 0);
	if (SOCKET_ERROR == retVal)
	{
		cout << "send failed!" << endl;
		closesocket(sHost); //关闭套接字
		WSACleanup(); //释放套接字资源
		return -1;
	}
	//ZeroMemory(bufRecv, BUF_SIZE);
	//recv(sHost, bufRecv, BUF_SIZE, 0); // 接收服务器端的数据
	//cout << endl << "从服务器接收数据：" << bufRecv << endl;
	int i = cli.Check_self(def);
	Desk_Show();
	ser.Check_defender(i);
	if (i == 2) { cli.Get_cards(); Desk_clean(); }
	if (arr_loc == 54 && cli.Get_num() == 0)//经过上次判定，r_1在本回合不会获胜
		return cli.Get_NO();				//此时判断r_2是否在本回合获胜

	switch (def)
	{
	case 0:return PVP_s(ser, cli);
	case 1:return PVP_s(ser, cli);
	case 2:return PVP_c(cli, ser);
	}
}
int PVP_c(Role cli, Role ser)
{
	if (arr_loc == 54 && cli.Get_num() == 1 && Desk_n == 0)//牌堆耗尽，桌面无牌，此时若进攻方手中仅余一张，
	{										//为鬼牌，则进攻方判输
		if (cli.joker_left() == 1)
			return ser.Get_NO();
	}
	cli.Show();
	int k,cli_out_num;
	for (k = cli.Attack(cli_out_num=cli.att_get()); k == 0; k = cli.Attack(cli_out_num=cli.att_get()));//k=1或2，2：放弃出牌
	//向服务器发送数据
	ZeroMemory(buf, BUF_SIZE);
	buf[0] = cli_out_num;
	retVal = send(sHost, buf, strlen(buf), 0);
	if (SOCKET_ERROR == retVal)
	{
		cout << "send failed!" << endl;
		closesocket(sHost); //关闭套接字
		WSACleanup(); //释放套接字资源
		return -1;
	}
	
	if (k == 2)
	{
		cli.Get_cards();
		ser.Get_cards();
		Desk_clean();
		return PVP_s(ser, cli);
	}
	Desk_Show();
	if (arr_loc == 54 && cli.Get_num() == 0)//cli获胜
		return cli.Get_NO();
	//向服务器发送数据
	ZeroMemory(buf, BUF_SIZE);
	buf[0] = 0;
	retVal = send(sHost, buf, strlen(buf), 0);
	if (SOCKET_ERROR == retVal)
	{
		cout << "send failed!" << endl;
		closesocket(sHost); //关闭套接字
		WSACleanup(); //释放套接字资源
		return -1;
	}
	ZeroMemory(bufRecv, BUF_SIZE);
	recv(sHost, bufRecv, BUF_SIZE, 0); // 接收服务器端的数据
	int def = ser.Defend(bufRecv[0]);
	Desk_Show();
	int i = ser.Check_self(def);
	cli.Check_defender(i);
	if (i == 2) { ser.Get_cards(); Desk_clean(); }
	if (arr_loc == 54 && ser.Get_num() == 0)//经过上次判定，r_1在本回合不会获胜
		return ser.Get_NO();				//此时判断r_2是否在本回合获胜
	switch (i)
	{
	case 0:return PVP_c(cli, ser);
	case 1:return PVP_c(cli, ser);
	case 2:return PVP_s(ser, cli);
	}
}
int  PVP_Turn_s(Role ser, Role cli)
{
	arr_loc = 0; //重置
	Desk_n = 0;
	
	if (get_arr() == -1)return -1;
	
	m_col = arr[53].color;
	cout << endl << "主花色" << m_col << endl;
	int i = PVP_s(ser, cli);//获得本局胜家
	if (i == -1)return -1;
	cout << i << "  win" << endl;
	if (i == ser.Get_NO())
	{
		return PVP_Turn_s(ser, cli);
	}
	else
	{
		return PVP_Turn_c(cli, ser);
	}
}
int PVP_Turn_c(Role cli, Role ser)
{
	if (-1 == get_arr())return -1;
	arr_loc = 0; //重置
	Desk_n = 0;
	m_col = arr[53].color;
	cout << endl << "主花色" << m_col << endl;
	int i = PVP_c(cli, ser);//获得本局胜家
	if (i == -1)return -1;
	cout << i << "  win" << endl;
	if (i == ser.Get_NO())
	{
		return PVP_Turn_s(ser, cli);
	}
	else
	{
		return PVP_Turn_c(cli, ser);
	}
}
int client()
{
	//建立连接
	if (1)
	{
		if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
		{
			cout << "WSAStartup failed!" << endl;
			return -1;
		}
		//创建套接字
		sHost = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (INVALID_SOCKET == sHost)
		{
			cout << "socket failed!" << endl;
			WSACleanup();//释放套接字资源
			return  -1;
		}

		//设置服务器地址
		servAddr.sin_family = AF_INET;
		//servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");//local
		//cout << "in"; char a[20]; cin >> a;servAddr.sin_addr.s_addr = inet_addr(a);
		servAddr.sin_addr.s_addr = inet_addr("10.122.219.203");//ipv4
		servAddr.sin_port = htons((short)4999);
		int nServAddlen = sizeof(servAddr);

		//连接服务器
		retVal = connect(sHost, (LPSOCKADDR)&servAddr, sizeof(servAddr));
		if (SOCKET_ERROR == retVal)
		{
			cout << "connect failed!" << endl; system("pause");
			closesocket(sHost); //关闭套接字
			WSACleanup(); //释放套接字资源
			return -1;
		}
	}
	///////////////////////////////////获得牌堆
	ZeroMemory(buf, 128);
	buf[0] = '1';
	retVal = send(sHost, buf, strlen(buf), 0);
	if (SOCKET_ERROR == retVal)
	{
		cout << "send failed!" << endl;
		closesocket(sHost); //关闭套接字
		WSACleanup(); //释放套接字资源
		return -1;
	}
	//RecvLine(sHost, bufRecv);
	ZeroMemory(bufRecv, 128);
	recv(sHost, bufRecv, 128, 0); // 接收服务器端的数据
	//获得bufRecv转为牌组
	for (int k = 0, i = 0; k < 54; k++)
	{
		arr[k].color = bufRecv[i++];
		arr[k].value = bufRecv[i++];
	}
	Role ser(1);
	Role cli(2);
	ser.Get_cards(); cli.Get_cards();
	int m_loc = bufRecv[127];
	m_col = arr[m_loc].color;
	int Turn_1_winner;
	if (m_loc < 8)
		Turn_1_winner = PVP_c(cli, ser);
	else
		Turn_1_winner = PVP_s(ser, cli);
	switch (Turn_1_winner)//第一局，根据返回值获得胜家
	{
	case -1:ser.~Role();cli.~Role(); return -1;
	case 1:	cout << "defeat" << endl; PVP_Turn_s(ser, cli);//根据第一局胜家，进入接下来的几局
	case 2:	cout << "win" << endl; PVP_Turn_c(cli, ser);//根据第一局胜家，进入接下来的几局
	}
	closesocket(sHost); //关闭套接字
	WSACleanup(); //释放套接字资源
	ser.~Role();
	cli.~Role();
	system("pause");
	return 0;
}

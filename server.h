#pragma once
#include"Role.h" 
#include"Pc.h"
#include"pvp.h"
#include "winsock2.h"
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996) 
#include<iostream>
#include<cstring>
using namespace std;
extern char m_col;
extern card  arr[54];//牌堆
extern card  Desk[54];//桌面上的牌
extern int arr_loc;//牌堆顶部对应数组位置
extern int Desk_n;//当前桌面上牌数
const int BUF_SIZE = 128;
WSADATA         wsd;            //WSADATA变量
SOCKET          sServer;        //服务器套接字
SOCKET          sClient;        //客户端套接字
SOCKADDR_IN     addrServ;;      //服务器地址
char            buf[BUF_SIZE];  //接收数据缓冲区
char            sendBuf[BUF_SIZE];//返回给客户端得数据
int             retVal;         //返回值
								//初始化套结字动态库

int send_arr()
{
	Shuffle();
	cout << "打乱牌堆" << endl;
	for (int k = 0; k < 54; k++)
		cout << arr[k].color << arr[k].value << " ";
	cout << '\n';
	/////////////////////////////////发送牌堆给客户端
	ZeroMemory(buf, 128);
	retVal = recv(sClient, buf, 128, 0);
	if (SOCKET_ERROR == retVal)
	{
		cout << "recv failed!" << endl;
		closesocket(sServer);   //关闭套接字
		closesocket(sClient);   //关闭套接字     
		WSACleanup();           //释放套接字资源;
		return -1;
	}
	char *card_send = new char[128];
	for (int k = 0, i = 0; k < 54; k++)//将牌堆转化为字符数组
	{
		card_send[i++] = arr[k].color;
		card_send[i++] = arr[k].value;
	}
	send(sClient, card_send, strlen(card_send), 0);
	delete[]card_send;
	return 0;
}
int PVP_s(Role ser,Role cli);
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
	ser.Show();
	int k,ser_out_num;
	for (k = ser.Attack(ser_out_num=ser.att_get()); k == 0; k = ser.Attack(ser_out_num=ser.att_get()));//k=1或2，2：放弃出牌
	//接收客户端数据
	ZeroMemory(buf, BUF_SIZE);
	retVal = recv(sClient, buf, BUF_SIZE, 0);
	if (SOCKET_ERROR == retVal)
	{
		cout << "recv failed!" << endl;
		closesocket(sServer);   //关闭套接字
		closesocket(sClient);   //关闭套接字     
		WSACleanup();           //释放套接字资源;
		return -1;
	}
	//cout << "客户端发送的数据: " << buf << endl;
	sendBuf[0]=ser_out_num;
	send(sClient, sendBuf, strlen(sendBuf), 0);
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
	//cli.Show();
	//接收客户端数据
	ZeroMemory(buf, BUF_SIZE);
	retVal = recv(sClient, buf, BUF_SIZE, 0);
	if (SOCKET_ERROR == retVal)
	{
		cout << "recv failed!" << endl;
		closesocket(sServer);   //关闭套接字
		closesocket(sClient);   //关闭套接字     
		WSACleanup();           //释放套接字资源;
		return -1;
	}
	//cout << "客户端发送的数据: " << buf << endl;
	int def = cli.Defend(buf[0]);////////////修改为cli压牌操作
	Desk_Show();
	cli.Check_self(def);
	ser.Check_defender(def);
	if (def == 2) { cli.Get_cards(); Desk_clean(); }
	if (arr_loc == 54 && cli.Get_num() == 0)//经过上次判定，r_1在本回合不会获胜
		return cli.Get_NO();				//此时判断r_2是否在本回合获胜
	switch (def)
	{
	case 0:return PVP_s(ser, cli);
	case 1:return PVP_s(ser, cli);
	case 2:return PVP_c(cli, ser);
	}
}
int PVP_c(Role cli,Role ser)
{
	if (arr_loc == 54 && cli.Get_num() == 1 && Desk_n == 0)//牌堆耗尽，桌面无牌，此时若进攻方手中仅余一张，
	{										//为鬼牌，则进攻方判输
		if (cli.joker_left() == 1)
			return ser.Get_NO();
	}
	//cli.Show();
	int k;
	//接收客户端数据
	ZeroMemory(buf, BUF_SIZE);
	retVal = recv(sClient, buf, BUF_SIZE, 0);
	if (SOCKET_ERROR == retVal)
	{
		cout << "recv failed!" << endl;
		closesocket(sServer);   //关闭套接字
		closesocket(sClient);   //关闭套接字     
		WSACleanup();           //释放套接字资源;
		return -1;
	}
	k = cli.Attack(buf[0]);
	if (k == 2)
	{
		cli.Get_cards();
		ser.Get_cards();
		Desk_clean();
		return PVP_s(ser, cli);
	}
	Desk_Show();
	if (arr_loc == 54 && cli.Get_num() == 0)//cli win
		return cli.Get_NO();
	ser.Show();
	int def, ser_out_num;
	for (def = cli.Defend(ser_out_num = cli.def_get()); def == 3; def = cli.Defend(ser_out_num = cli.def_get()));//def!=3
	//接收客户端数据
	ZeroMemory(buf, BUF_SIZE);
	retVal = recv(sClient, buf, BUF_SIZE, 0);
	if (SOCKET_ERROR == retVal)
	{
		cout << "recv failed!" << endl;
		closesocket(sServer);   //关闭套接字
		closesocket(sClient);   //关闭套接字     
		WSACleanup();           //释放套接字资源;
		return -1;
	}
	sendBuf[0] = ser_out_num;
	send(sClient, sendBuf, strlen(sendBuf), 0);
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
	if(-1==send_arr())return -1;
	arr_loc = 0; //重置
	Desk_n = 0;
	m_col = arr[53].color;
	cout << endl << "主花色" << m_col << endl;
	ser.Get_cards(); cli.Get_cards();
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
	if (-1 == send_arr())return -1;
	arr_loc = 0; //重置
	Desk_n = 0;
	m_col = arr[53].color;
	cout << endl << "主花色" << m_col << endl;
	cli.Get_cards(); ser.Get_cards();
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
int PVP_Turn_1(Role ser,Role cli)
{
	Shuffle();
	cout << "打乱牌堆" << endl;
	for (int k = 0; k < 54; k++)
		cout << arr[k].color << arr[k].value << " ";
	cout << '\n';
	/////////////////////////////////发送牌堆给客户端
	ZeroMemory(buf, 128);
	retVal = recv(sClient, buf, 128, 0);
	if (SOCKET_ERROR == retVal)
	{
		cout << "recv failed!" << endl;
		closesocket(sServer);   //关闭套接字
		closesocket(sClient);   //关闭套接字     
		WSACleanup();           //释放套接字资源;
		return -1;
	}
	char *card_send = new char[128];
	for (int k = 0, i = 0; k < 54; k++)//将牌堆转化为字符数组
	{
		card_send[i++] = arr[k].color;
		card_send[i++] = arr[k].value;
	}
	int m_loc = choose_m_col_1();
	card_send[127] = m_loc;
	send(sClient, card_send, strlen(card_send), 0);
	delete[]card_send;
	ser.Get_cards();
	cli.Get_cards();
	cout << endl << "主花色" << m_col << endl;
	if (m_loc < 8)
		return PVP_c(cli, ser);
	else
		return PVP_s(ser, cli);
}
int server()
{
	Role ser(1);
	Role cli(2);
	//check
	if (1) {
		if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
		{
			cout << "WSAStartup failed!" << endl;
			return -1;
		}

		//创建套接字
		sServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (INVALID_SOCKET == sServer)
		{
			cout << "socket failed!" << endl;
			WSACleanup();//释放套接字资源;
			return  -1;
		}

		//服务器套接字地址 
		addrServ.sin_family = AF_INET;
		addrServ.sin_port = htons(4999);
		addrServ.sin_addr.s_addr = INADDR_ANY;
		//绑定套接字
		retVal = bind(sServer, (LPSOCKADDR)&addrServ, sizeof(SOCKADDR_IN));
		if (SOCKET_ERROR == retVal)
		{
			cout << "bind failed!" << endl;
			closesocket(sServer);   //关闭套接字
			WSACleanup();           //释放套接字资源;
			return -1;
		}

		//开始监听 
		retVal = listen(sServer, 1);
		if (SOCKET_ERROR == retVal)
		{
			cout << "listen failed!" << endl;
			closesocket(sServer);   //关闭套接字
			WSACleanup();           //释放套接字资源;
			return -1;
		}
		//接受客户端请求
		sockaddr_in addrClient;
		int addrClientlen = sizeof(addrClient);
		sClient = accept(sServer, (sockaddr FAR*)&addrClient, &addrClientlen);
		if (INVALID_SOCKET == sClient)
		{
			cout << "accept failed!" << endl;
			closesocket(sServer);   //关闭套接字
			WSACleanup();           //释放套接字资源;
			return -1;
		}
	}
	//初始化牌堆
	Creat_cards();
	switch(PVP_Turn_1(ser, cli))
	{
	case -1:ser.~Role();cli.~Role(); return -1; break;
	case 1:cout << "1 win" << endl; PVP_Turn_s(ser, cli); break;//根据第一局胜家，进入接下来的几局
	case 2:	cout << "2 win" << endl; PVP_Turn_c(cli, ser); break;//根据第一局胜家，进入接下来的几局
	}
	closesocket(sServer);   //关闭套接字
	closesocket(sClient);   //关闭套接字
	WSACleanup();           //释放套接字资源;
	ser.~Role();
	cli.~Role();
	return 0;
}

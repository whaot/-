#pragma once
#define WIN32_LEAN_AND_MEAN
#include <graphics.h>
#include <conio.h>
#include"Role.h" 
#include"Pc.h"
#include "winsock2.h"
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996) 
#include<iostream>
#include<cstring>
using namespace std;
extern char m_col;
extern card  arr[54];//�ƶ�
extern card  Desk[54];//�����ϵ���
extern int arr_loc;//�ƶѶ�����Ӧ����λ��
extern int Desk_n;//��ǰ����������
extern const int BUF_SIZE;
WSADATA         s_wsd;            //WSADATA����
SOCKET          sServer;        //�������׽���
SOCKET          sClient;        //�ͻ����׽���
SOCKADDR_IN     addrServ;;      //��������ַ
extern char            buf[BUF_SIZE];  //�������ݻ�����
char            sendBuf[BUF_SIZE];//���ظ��ͻ��˵�����
int             s_retVal;         //����ֵ
								//��ʼ���׽��ֶ�̬��
extern char            sendBuf[BUF_SIZE];//���ظ��ͻ��˵�����
								//��ʼ���׽��ֶ�̬��
int s_send(int k);
int send_arr()
{
	for (int k = 0; k < 54; k++)
	{
		if (-1 == s_send(int(arr[k].color)))return -1;;
		if (s_send(arr[k].value) == -1)return -1;
		if (s_send(arr[k].numforimage))return -1;
	}
	return 0;
}
int s_get()
{
	ZeroMemory(buf, BUF_SIZE);
	retVal = recv(sClient, buf, BUF_SIZE, 0);
	if (SOCKET_ERROR == retVal)
	{
		cout << "recv failed!" << endl;
		closesocket(sServer);   //�ر��׽���
		closesocket(sClient);   //�ر��׽���     
		WSACleanup();           //�ͷ��׽�����Դ;
		return -1;
	}
	int k = buf[0];
	if (k == 55)k = 0;
	return k;
}
int s_send(int k)
{
	if (k == 0)
		sendBuf[0] = 55;
	else
		sendBuf[0] = k;
	retVal = send(sClient, sendBuf, BUF_SIZE, 0);
	if (SOCKET_ERROR == retVal)
	{
		cout << "recv failed!" << endl;
		closesocket(sServer);   //�ر��׽���
		closesocket(sClient);   //�ر��׽���     
		WSACleanup();           //�ͷ��׽�����Դ;
		return -1;
	}
	return 0;
}
int ser_PVP_s(Role ser, Role cli);
int ser_PVP_c(Role cli, Role ser);
int ser_PVP_Turn_s(Role ser, Role cli);
int ser_PVP_Turn_c(Role cli, Role ser);
int ser_PVP_s(Role ser, Role cli)
{
	if (arr_loc == 54 && ser.Get_num() == 1 && Desk_n == 0)//�ƶѺľ����������ƣ���ʱ��������ser���н���һ�ţ�
	{										//Ϊ���ƣ������������
		if (ser.joker_left() == 1)
			return cli.Get_NO();
	}
	ser.Show();
	cli.Showem();
	int k, ser_out_num;
	for (k = ser.Attack(ser_out_num = ser.att_get()); k == 0; k = ser.Attack(ser_out_num = ser.att_get()));//k=1��2��2����������
	if (-1 == s_send(ser_out_num))return -1;
	if (k == 2)
	{
		ser.Get_cards(); //ser.Show();
		cli.Get_cards();
		Desk_clean();
		ser.clean();
		return ser_PVP_c(cli, ser);
	}
	//cli.clean();
	Desk_Show();
	//ser.Show();
	if (arr_loc == 54 && ser.Get_num() == 0)
		return ser.Get_NO();
	//cli.Show();
	int get = s_get();
	if (get == -1)return -1;
	int def = cli.Defend(get);////////////�޸�Ϊcliѹ�Ʋ���
	//cli.clean();
	Desk_Show();
	cli.Check_self(def);
	ser.Check_defender(def);
	if (def == 2) { cli.Get_cards(); Desk_clean(); ser.clean(); }
	if (arr_loc == 54 && cli.Get_num() == 0)//�����ϴ��ж���r_1�ڱ��غϲ����ʤ
		return cli.Get_NO();				//��ʱ�ж�r_2�Ƿ��ڱ��غϻ�ʤ
	switch (def)
	{
	case 0:return ser_PVP_s(ser, cli);
	case 1:return ser_PVP_s(ser, cli);
	case 2:return ser_PVP_c(cli, ser);
	default:return -1;
	}
}
int ser_PVP_c(Role cli, Role ser)
{
	if (arr_loc == 54 && cli.Get_num() == 1 && Desk_n == 0)//�ƶѺľ����������ƣ���ʱ��������cli���н���һ�ţ�
	{										//Ϊ���ƣ������������
		if (cli.joker_left() == 1)
			return ser.Get_NO();
	}
	//cli.Show();
	int k;
	cli.Showem();
	ser.Show();
	int get = s_get();
	if (get == -1)return -1;
	k = cli.Attack(get);
	if (k == 2)
	{
		cli.Get_cards();
		ser.Get_cards();
		Desk_clean();
		ser.clean();
		return ser_PVP_s(ser, cli);
	}
	//cli.clean(); 
	Desk_Show();
	if (arr_loc == 54 && cli.Get_num() == 0)//cli win
		return cli.Get_NO();
	//ser.Show();
	int def, ser_out_num;
	for (def = cli.Defend(ser_out_num = cli.def_get()); def == 3; def = cli.Defend(ser_out_num = cli.def_get()));//def!=3

	if (-1 == s_send(ser_out_num))return -1;
	//cli.clean();
	Desk_Show();// ser.Show();
	int i = ser.Check_self(def);
	cli.Check_defender(i);
	if (i == 2) { ser.Get_cards(); Desk_clean(); cli.clean(); }
	if (arr_loc == 54 && ser.Get_num() == 0)//�����ϴ��ж���r_1�ڱ��غϲ����ʤ
		return ser.Get_NO();				//��ʱ�ж�r_2�Ƿ��ڱ��غϻ�ʤ
	switch (i)
	{
	case 0:return ser_PVP_c(cli, ser);
	case 1:return ser_PVP_c(cli, ser);
	case 2:return ser_PVP_s(ser, cli);
	default:return -1;
	}
}
int ser_PVP_Turn_s(Role ser, Role cli)
{
	IMAGE win, lose;
	loadimage(&win, "G:\\ceshi\\win.jpg", 200, 100);
	loadimage(&lose, "G:\\ceshi\\lose.jpg", 200, 100);
	IMAGE cardimage[53];
	loadimage(&cardimage[53], "G:\\ceshi\\bigk.jpg", 75, 100);
	loadimage(&cardimage[0], "G:\\ceshi\\ha.jpg", 75, 100);
	loadimage(&cardimage[1], "G:\\ceshi\\h2.jpg", 75, 100);
	loadimage(&cardimage[2], "G:\\ceshi\\h3.jpg", 75, 100);
	loadimage(&cardimage[3], "G:\\ceshi\\h4.jpg", 75, 100);
	loadimage(&cardimage[4], "G:\\ceshi\\h5.jpg", 75, 100);
	loadimage(&cardimage[5], "G:\\ceshi\\h6.jpg", 75, 100);
	loadimage(&cardimage[6], "G:\\ceshi\\h7.jpg", 75, 100);
	loadimage(&cardimage[7], "G:\\ceshi\\h8.jpg", 75, 100);
	loadimage(&cardimage[8], "G:\\ceshi\\h9.jpg", 75, 100);
	loadimage(&cardimage[9], "G:\\ceshi\\h10.jpg", 75, 100);
	loadimage(&cardimage[10], "G:\\ceshi\\hj.jpg", 75, 100);
	loadimage(&cardimage[11], "G:\\ceshi\\hq.jpg", 75, 100);
	loadimage(&cardimage[12], "G:\\ceshi\\hk.jpg", 75, 100);
	loadimage(&cardimage[13], "G:\\ceshi\\ba.jpg", 75, 100);
	loadimage(&cardimage[14], "G:\\ceshi\\b2.jpg", 75, 100);
	loadimage(&cardimage[15], "G:\\ceshi\\b3.jpg", 75, 100);
	loadimage(&cardimage[16], "G:\\ceshi\\b4.jpg", 75, 100);
	loadimage(&cardimage[17], "G:\\ceshi\\b5.jpg", 75, 100);
	loadimage(&cardimage[18], "G:\\ceshi\\b6.jpg", 75, 100);
	loadimage(&cardimage[19], "G:\\ceshi\\b7.jpg", 75, 100);
	loadimage(&cardimage[20], "G:\\ceshi\\b8.jpg", 75, 100);
	loadimage(&cardimage[21], "G:\\ceshi\\b9.jpg", 75, 100);
	loadimage(&cardimage[22], "G:\\ceshi\\b10.jpg", 75, 100);
	loadimage(&cardimage[23], "G:\\ceshi\\bj.jpg", 75, 100);
	loadimage(&cardimage[24], "G:\\ceshi\\bq.jpg", 75, 100);
	loadimage(&cardimage[25], "G:\\ceshi\\bk.jpg", 75, 100);
	loadimage(&cardimage[26], "G:\\ceshi\\fa.jpg", 75, 100);
	loadimage(&cardimage[27], "G:\\ceshi\\f2.jpg", 75, 100);
	loadimage(&cardimage[28], "G:\\ceshi\\f3.jpg", 75, 100);
	loadimage(&cardimage[29], "G:\\ceshi\\f4.jpg", 75, 100);
	loadimage(&cardimage[30], "G:\\ceshi\\f5.jpg", 75, 100);
	loadimage(&cardimage[31], "G:\\ceshi\\f6.jpg", 75, 100);
	loadimage(&cardimage[32], "G:\\ceshi\\f7.jpg", 75, 100);
	loadimage(&cardimage[33], "G:\\ceshi\\f8.jpg", 75, 100);
	loadimage(&cardimage[34], "G:\\ceshi\\f9.jpg", 75, 100);
	loadimage(&cardimage[35], "G:\\ceshi\\f10.jpg", 75, 100);
	loadimage(&cardimage[36], "G:\\ceshi\\fj.jpg", 75, 100);
	loadimage(&cardimage[37], "G:\\ceshi\\fq.jpg", 75, 100);
	loadimage(&cardimage[38], "G:\\ceshi\\fk.jpg", 75, 100);
	loadimage(&cardimage[39], "G:\\ceshi\\ra.jpg", 75, 100);
	loadimage(&cardimage[40], "G:\\ceshi\\r2.jpg", 75, 100);
	loadimage(&cardimage[41], "G:\\ceshi\\r3.jpg", 75, 100);
	loadimage(&cardimage[42], "G:\\ceshi\\r4.jpg", 75, 100);
	loadimage(&cardimage[43], "G:\\ceshi\\r5.jpg", 75, 100);
	loadimage(&cardimage[44], "G:\\ceshi\\r6.jpg", 75, 100);
	loadimage(&cardimage[45], "G:\\ceshi\\r7.jpg", 75, 100);
	loadimage(&cardimage[46], "G:\\ceshi\\r8.jpg", 75, 100);
	loadimage(&cardimage[47], "G:\\ceshi\\r9.jpg", 75, 100);
	loadimage(&cardimage[48], "G:\\ceshi\\r10.jpg", 75, 100);
	loadimage(&cardimage[49], "G:\\ceshi\\rj.jpg", 75, 100);
	loadimage(&cardimage[50], "G:\\ceshi\\rq.jpg", 75, 100);
	loadimage(&cardimage[51], "G:\\ceshi\\rk.jpg", 75, 100);
	loadimage(&cardimage[52], "G:\\ceshi\\minik.jpg", 75, 100);
	Shuffle();
	if (-1 == send_arr())return -1;
	arr_loc = 0; //����
	Desk_n = 0;
	m_col = arr[53].color;
	putimage(825, 170, &cardimage[arr[53].numforimage]);
	ser.Get_cards(); cli.Get_cards();
	int i = ser_PVP_s(ser, cli);//��ñ���ʤ��
	if (i == -1)return -1;
	//cout << i << "  win" << endl;///////////////////////////////////////////
	if (i == ser.Get_NO())
	{
		putimage(300, 200, &win); Sleep(2000); return ser_PVP_Turn_s(ser, cli);
	}
	else
	{
		putimage(300, 200, &lose); Sleep(2000); return ser_PVP_Turn_c(cli, ser);
	}
}
int ser_PVP_Turn_c(Role cli, Role ser)
{
	IMAGE win, lose;
	loadimage(&win, "G:\\ceshi\\win.jpg", 200, 100);
	loadimage(&lose, "G:\\ceshi\\lose.jpg", 200, 100);
	IMAGE cardimage[53];
	loadimage(&cardimage[53], "G:\\ceshi\\bigk.jpg", 75, 100);
	loadimage(&cardimage[0], "G:\\ceshi\\ha.jpg", 75, 100);
	loadimage(&cardimage[1], "G:\\ceshi\\h2.jpg", 75, 100);
	loadimage(&cardimage[2], "G:\\ceshi\\h3.jpg", 75, 100);
	loadimage(&cardimage[3], "G:\\ceshi\\h4.jpg", 75, 100);
	loadimage(&cardimage[4], "G:\\ceshi\\h5.jpg", 75, 100);
	loadimage(&cardimage[5], "G:\\ceshi\\h6.jpg", 75, 100);
	loadimage(&cardimage[6], "G:\\ceshi\\h7.jpg", 75, 100);
	loadimage(&cardimage[7], "G:\\ceshi\\h8.jpg", 75, 100);
	loadimage(&cardimage[8], "G:\\ceshi\\h9.jpg", 75, 100);
	loadimage(&cardimage[9], "G:\\ceshi\\h10.jpg", 75, 100);
	loadimage(&cardimage[10], "G:\\ceshi\\hj.jpg", 75, 100);
	loadimage(&cardimage[11], "G:\\ceshi\\hq.jpg", 75, 100);
	loadimage(&cardimage[12], "G:\\ceshi\\hk.jpg", 75, 100);
	loadimage(&cardimage[13], "G:\\ceshi\\ba.jpg", 75, 100);
	loadimage(&cardimage[14], "G:\\ceshi\\b2.jpg", 75, 100);
	loadimage(&cardimage[15], "G:\\ceshi\\b3.jpg", 75, 100);
	loadimage(&cardimage[16], "G:\\ceshi\\b4.jpg", 75, 100);
	loadimage(&cardimage[17], "G:\\ceshi\\b5.jpg", 75, 100);
	loadimage(&cardimage[18], "G:\\ceshi\\b6.jpg", 75, 100);
	loadimage(&cardimage[19], "G:\\ceshi\\b7.jpg", 75, 100);
	loadimage(&cardimage[20], "G:\\ceshi\\b8.jpg", 75, 100);
	loadimage(&cardimage[21], "G:\\ceshi\\b9.jpg", 75, 100);
	loadimage(&cardimage[22], "G:\\ceshi\\b10.jpg", 75, 100);
	loadimage(&cardimage[23], "G:\\ceshi\\bj.jpg", 75, 100);
	loadimage(&cardimage[24], "G:\\ceshi\\bq.jpg", 75, 100);
	loadimage(&cardimage[25], "G:\\ceshi\\bk.jpg", 75, 100);
	loadimage(&cardimage[26], "G:\\ceshi\\fa.jpg", 75, 100);
	loadimage(&cardimage[27], "G:\\ceshi\\f2.jpg", 75, 100);
	loadimage(&cardimage[28], "G:\\ceshi\\f3.jpg", 75, 100);
	loadimage(&cardimage[29], "G:\\ceshi\\f4.jpg", 75, 100);
	loadimage(&cardimage[30], "G:\\ceshi\\f5.jpg", 75, 100);
	loadimage(&cardimage[31], "G:\\ceshi\\f6.jpg", 75, 100);
	loadimage(&cardimage[32], "G:\\ceshi\\f7.jpg", 75, 100);
	loadimage(&cardimage[33], "G:\\ceshi\\f8.jpg", 75, 100);
	loadimage(&cardimage[34], "G:\\ceshi\\f9.jpg", 75, 100);
	loadimage(&cardimage[35], "G:\\ceshi\\f10.jpg", 75, 100);
	loadimage(&cardimage[36], "G:\\ceshi\\fj.jpg", 75, 100);
	loadimage(&cardimage[37], "G:\\ceshi\\fq.jpg", 75, 100);
	loadimage(&cardimage[38], "G:\\ceshi\\fk.jpg", 75, 100);
	loadimage(&cardimage[39], "G:\\ceshi\\ra.jpg", 75, 100);
	loadimage(&cardimage[40], "G:\\ceshi\\r2.jpg", 75, 100);
	loadimage(&cardimage[41], "G:\\ceshi\\r3.jpg", 75, 100);
	loadimage(&cardimage[42], "G:\\ceshi\\r4.jpg", 75, 100);
	loadimage(&cardimage[43], "G:\\ceshi\\r5.jpg", 75, 100);
	loadimage(&cardimage[44], "G:\\ceshi\\r6.jpg", 75, 100);
	loadimage(&cardimage[45], "G:\\ceshi\\r7.jpg", 75, 100);
	loadimage(&cardimage[46], "G:\\ceshi\\r8.jpg", 75, 100);
	loadimage(&cardimage[47], "G:\\ceshi\\r9.jpg", 75, 100);
	loadimage(&cardimage[48], "G:\\ceshi\\r10.jpg", 75, 100);
	loadimage(&cardimage[49], "G:\\ceshi\\rj.jpg", 75, 100);
	loadimage(&cardimage[50], "G:\\ceshi\\rq.jpg", 75, 100);
	loadimage(&cardimage[51], "G:\\ceshi\\rk.jpg", 75, 100);
	loadimage(&cardimage[52], "G:\\ceshi\\minik.jpg", 75, 100);
	Shuffle();
	if (-1 == send_arr())return -1;
	arr_loc = 0; //����
	Desk_n = 0;
	m_col = arr[53].color;
	putimage(825, 170, &cardimage[arr[53].numforimage]);
	outtextxy(925, 170, "����ɫ");
	cli.Get_cards(); ser.Get_cards();
	int i = ser_PVP_c(cli, ser);//��ñ���ʤ��
	arr_loc = 0; //����
	Desk_n = 0;
	if (i == -1)return -1;
	//cout << i << "  win" << endl;///////////////////////////////////////
	if (i == ser.Get_NO())
	{
		putimage(300, 200, &win); Sleep(2000); return ser_PVP_Turn_s(ser, cli);
	}
	else
	{
		putimage(300, 200, &lose); Sleep(2000); return ser_PVP_Turn_c(cli, ser);
	}
}
int ser_PVP_Turn_1(Role ser, Role cli)
{

	Shuffle();
	/*cout << "�����ƶ�" << endl;
	for (int k = 0; k < 54; k++)
		cout << arr[k].color << arr[k].value << " ";
	cout << '\n';*/
	/////////////////////////////////�����ƶѸ��ͻ���
	if (-1 == send_arr())return -1;
	int m_loc = choose_m_col_1();
	if (-1 == s_send(m_loc))return -1;
	//cout << "send cards" << endl;
	IMAGE win, lose;
	loadimage(&win, "G:\\ceshi\\win.jpg", 200, 100);
	loadimage(&lose, "G:\\ceshi\\lose.jpg", 200, 100);
	IMAGE bck, mei, hong, hei, fang, emcard;
	initgraph(1000, 600);
	loadimage(&emcard, "G:\\ceshi\\cardback.jpg", 75, 100);
	loadimage(&bck, "G:\\ceshi\\greenbck.jpg", 1000, 600);
	loadimage(&mei, "G:\\ceshi\\mei.jpg", 1000, 600);
	loadimage(&hong, "G:\\ceshi\\hong.jpg", 1000, 600);
	loadimage(&fang, "G:\\ceshi\\fang.jpg", 1000, 600);
	loadimage(&hei, "G:\\ceshi\\hei.jpg", 1000, 600);
	IMAGE cardimage[53];
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
	loadimage(&cardimage[23], "G:\\ceshi\\b10.jpg", 75, 100);
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
	putimage(0, 0, &bck);
	outtextxy(300, 50, "�Է�����");
	outtextxy(300, 525, "�ҷ�����");
	ser.Get_cards();
	cli.Get_cards();

	putimage(825, 170, &cardimage[arr[m_loc].numforimage]);
	outtextxy(925, 170, "����ɫ");
	if (m_loc < 8)
		return ser_PVP_c(cli, ser);
	else
		return ser_PVP_s(ser, cli);
}

int server()
{
	//Desk[0].color = 'r'; Desk[0].value = 1;
	Role ser(1);
	Role cli(2);
	//�������
	if (WSAStartup(MAKEWORD(2, 2), &s_wsd) != 0)
	{
		cout << "WSAStartup failed!" << endl;
		return -1;
	}
	//�����׽���
	sServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == sServer)
	{
		cout << "socket failed!" << endl;
		WSACleanup();//�ͷ��׽�����Դ;
		return  -1;
	}

	//�������׽��ֵ�ַ 
	addrServ.sin_family = AF_INET;
	addrServ.sin_port = htons(4999);
	addrServ.sin_addr.s_addr = INADDR_ANY;
	//���׽���
	s_retVal = bind(sServer, (LPSOCKADDR)&addrServ, sizeof(SOCKADDR_IN));
	if (SOCKET_ERROR == s_retVal)
	{
		cout << "bind failed!" << endl;
		closesocket(sServer);   //�ر��׽���
		WSACleanup();           //�ͷ��׽�����Դ;
		return -1;
	}

	//��ʼ���� 
	s_retVal = listen(sServer, 1);
	if (SOCKET_ERROR == s_retVal)
	{
		cout << "listen failed!" << endl;
		closesocket(sServer);   //�ر��׽���
		WSACleanup();           //�ͷ��׽�����Դ;
		return -1;
	}
	//���ܿͻ�������
	sockaddr_in addrClient;
	int addrClientlen = sizeof(addrClient);
	sClient = accept(sServer, (sockaddr FAR*)&addrClient, &addrClientlen);
	if (INVALID_SOCKET == sClient)
	{
		cout << "accept failed!" << endl;
		closesocket(sServer);   //�ر��׽���
		WSACleanup();           //�ͷ��׽�����Դ;
		return -1;
	}
	//cout << "keep listening" << endl;
	//��ʼ���ƶ�

	Create_cards();
	int turn_1 = ser_PVP_Turn_1(ser, cli);

	IMAGE win, lose;
	loadimage(&win, "G:\\ceshi\\win.jpg", 200, 100);
	loadimage(&lose, "G:\\ceshi\\lose.jpg", 200, 100);
	IMAGE bck, mei, hong, hei, fang, emcard;
	initgraph(1000, 600);
	loadimage(&emcard, "G:\\ceshi\\cardback.jpg", 75, 100);
	loadimage(&bck, "G:\\ceshi\\greenbck.jpg", 1000, 600);
	loadimage(&mei, "G:\\ceshi\\mei.jpg", 1000, 600);
	loadimage(&hong, "G:\\ceshi\\hong.jpg", 1000, 600);
	loadimage(&fang, "G:\\ceshi\\fang.jpg", 1000, 600);
	loadimage(&hei, "G:\\ceshi\\hei.jpg", 1000, 600);
	IMAGE cardimage[53];
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
	loadimage(&cardimage[23], "G:\\ceshi\\b10.jpg", 75, 100);
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
	putimage(0, 0, &bck);
	outtextxy(300, 50, "�Է�����");
	outtextxy(300, 525, "�ҷ�����");
	switch (turn_1)
	{
	case -1:ser.~Role(); cli.~Role(); return -1; break;
	case 1:putimage(300, 200, &win); Sleep(2000); ser_PVP_Turn_s(ser, cli); break;//���ݵ�һ��ʤ�ң�����������ļ���
	case 2:	putimage(300, 200, &lose); Sleep(2000); ser_PVP_Turn_c(cli, ser); break;//���ݵ�һ��ʤ�ң�����������ļ���
	}
	closesocket(sServer);   //�ر��׽���
	closesocket(sClient);   //�ر��׽���
	WSACleanup();           //�ͷ��׽�����Դ;
	ser.~Role();
	cli.~Role();
	return 0;
}

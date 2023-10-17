#pragma once
#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <conio.h>
#include <stdlib.h>
#include <math.h>

#include <Mmsystem.h>
#include <Digitalv.h>
#pragma comment(lib, "winmm.lib")

#define GAME_NAME		"DOSDUNGEON"
#define GAME_VERSION	"0.5.6"

#define SCREEN_SIZE		44

#define TITLE_MENU_MAX  3
#define SIGHT_MAX		3
#define LANGUAGE_MAX	3

#define DATA_WIDTH		18
#define DATA_HEIGHT		22
#define DATA_START		118


#define MAX			999//최소 5이상 && 홀수로 설정해야 미로가 의도한대로 만들어집니다.
#define C_MAX		88 - 14

#define NAME_MAX	10

typedef enum KEY
{
	KEY_ENTER = 13,
	KEY_UP = 72,
	KEY_LEFT = 75,
	KEY_RIGHT = 77,
	KEY_DOWN = 80,

	KEY_a = 97, KEY_b, KEY_c, KEY_d, KEY_e, KEY_f, KEY_g,
	KEY_h, KEY_i, KEY_j, KEY_k, KEY_l, KEY_m, KEY_n, KEY_o, KEY_p,
	KEY_q, KEY_r, KEY_s, KEY_t, KEY_u, KEY_v, KEY_w, KEY_x, KEY_y, KEY_z,

	KEY_A = 65, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, KEY_G, KEY_H, KEY_I, KEY_J, KEY_K, KEY_L, KEY_M, KEY_N, KEY_O, KEY_P, KEY_Q, KEY_R, KEY_S, KEY_T, KEY_U, KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z
} KEY;

typedef enum
{
	TEXT_BLACK = 0, TEXT_DARKBLUE, TEXT_DARKGREEN, TEXT_DARKSKY, TEXT_DARKRED, TEXT_DARKPURPLE, TEXT_DARKYELLOW, TEXT_GRAY, TEXT_DARKGRAY,
	TEXT_BLUE, TEXT_GREEN, TEXT_SKY, TEXT_RED, TEXT_PURPPLE, TEXT_YELLOW, TEXT_WHITE,

	BACK_BLACK = 0, BACK_DARKBLUE = 16, BACK_DARKGREEN = 32, BACK_DARKSKY = 48, BACK_DARKRED = 64, BACK_DARKPURPLE = 80, BACK_DARKYELLOW = 96, BACK_GRAY = 112, BACK_DARKGRAY = 128,
	BACK_BLUE = 144, BACK_GREEN = 160, BACK_SKY = 176, BACK_RED = 192, BACK_PURPPLE = 208, BACK_YELLOW = 224, BACK_WHITE = 240
} COLOR;

#define DTC         7 //기본 글자색상
#define DTB         0 //기본 배경색상
#define DTW1		240//기본 벽색상
#define DTW2		112//안쪽 벽색상
#define DTW3		128//가장안쪽 벽색상
int DTS = BACK_SKY;//기본 하늘색상
int DTE = BACK_GREEN;//기본 땅색상


MCI_PLAY_PARMS m_mciPlayParms;
MCI_OPEN_PARMS SE1;//효과음1
MCI_OPEN_PARMS SE4;//비명
MCI_OPEN_PARMS BGM1;//타이틀
MCI_OPEN_PARMS BGM2;//설명
MCI_OPEN_PARMS BGM3;//필드
MCI_OPEN_PARMS BGM4;//X

int Color_Text = 0;
int Color_Back = 0;
int Interface_Shortcut = 1;
int Interface_Language = 0;//0 : 한국어/1 : 영어/2 : 일본어
int Interface_Focus = 0;//UI선택 포커스

int Player_ID;//플레이어의 맵ID
int Player_X;//X좌표
int Player_Y;//Y좌표
int Player_Angle;//방향
				 //int Player_Sight;
				 //int Player_Xray = 0;

int Player_Gold = 0;//골드
int Player_LV = 1;//레벨
int Player_EXP = 0;//경험치
int Player_EXP_Temp = 0;//경험치 게이지 애니메이션을 위한 임시변수

char Player_Name[NAME_MAX] = "";//이름
int Player_HP_Max = 10;//최대체력
int Player_MP_Max = 10;//최대마나
int Player_HP = 10;//체력
int Player_MP = 10;//마나
int Player_HP_Temp = 10;//체력 게이지 애니메이션을 위한 임시변수
int Player_MP_Temp = 10;//마나 게이지 애니메이션을 위한 임시변수
int Player_STR;//Strength 공격력
int Player_DEX;//Dexterity 민첩성
int Player_INT;//Intelligence 지능
int Player_Inventory[16][2] = { 0 };
int Player_Weapon = 0;//[1] = { 0 };
int Player_Equipment[5] = { 0 };//머리,몸,팔,다리,신발

int NextLevel = 10;//레벨업 하기위해 다음레벨까지 필요한 경험치
int NextLevel_Temp = 10;//경험치 게이지 애니메이션을 위한 임시변수

char Console[7][C_MAX] = { 0 };

int Display[SCREEN_SIZE][SCREEN_SIZE] = { 0 };//화면을 업데이트할때 다른부분만 색칠하게하기위한

typedef struct Data Data;
struct Data {
	int Data;
	Data *Next;
};

typedef struct Image Image;
struct Image
{
	char *Name;
	int ID;
	int Priority;
	int X;
	int Y;
	int Width;
	int Height;
	Image *Next;
	int Image[SCREEN_SIZE][SCREEN_SIZE];
	//int
};


typedef struct Map Map;
struct Map {
	int ID;
	int Map[MAX + 2][MAX + 2];
	int Sight[MAX + 2][MAX + 2];//맵에 표시되었던 곳
	int Size;

	Map *Next;
};

typedef struct Monster Monster;
struct Monster {
	int Number;//몹 고유숫자
	int Map_ID;
	int X;
	int Y;
	int Angle;

	int HP_Max;
	int MP_Max;
	int HP;
	int MP;
	int STR;
	int DEX;
	int INT;
	int Species;//몬스터의 종류

	int EXP;
	int Gold;
	//int Inventory[]
	int Weapon;
	int Equipment;

	int Sight;//시야
	int Sound;//청각
	int Behavior;//행동
	int Personality;//성향
	int Type;//공격 성향
	int Image;//표시되는 그림
	int Group;//공격하는것

	Monster *Next;
};

typedef struct Item Item;
struct Item {
	char Name[10];//아이템의 이름
	int Number;//아이템 고유숫자
	int Usable;//사용 불가능한 아이템 : 0 / 사용 가능한 아이템 : 1
	int HP;
	int MP;
	int STR;
	int DEX;
	int INT;

	int ID;
	int X;
	int Y;

	int Event;
	Item *Next;
};

typedef struct Weapon Weapon;
struct Weapon {
	char Name[10];//무기이름
	int Number;//무기 고유숫자
	int Usable;//사용 불가능한 아이템 : 0 / 사용 가능한 아이템 : 1
	int MHP;//증가하는 최대체력
	int MMP;//증가하는 최대마나
	int STR;//증가하는 힘
	int DEX;//증가하는 민첩
	int INT;//증가하는 지능

	int Range;//공격 범위
	int Penetrate;//몹 관통 가능 횟수
	int Knock;//넉백
	int Pass;//벽 통과 여부

	Weapon *Next;
};

typedef struct Equipment Equipment;
struct Equipment {
	char Name[10];//장비이름
	int Number;//장비 고유숫자
	int Part;//장비 착용위치 /머리,몸,팔,다리,신발
	int HP;//증가하는 체력
	int MP;//증가하는 마나
	int STR;//증가하는 힘
	int DEX;//증가하는 민첩
	int INT;//증가하는 지능

	Weapon *Next;
};

Map *Map_Head = NULL;
Monster *Monster_Head = NULL;
Item *Item_Head = NULL;
Weapon *Weapon_Head = NULL;
Equipment *Equipment_Head = NULL;
Data *Player_Condition = NULL;
Image *Image_Head = NULL;

void PressAnyKey()
{
	while (1)
	{
		if (_kbhit())
		{
			if (_getch() == 224)
				_getch();
			break;
		}
	}
}

int Random(int a)
{
	return rand() % a + 1;
}

void Text_Color(int text, int back)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), text + back);
}

void Draw(int a, int b, int c, int d)//a는 종류/b는 갯수
{
	Text_Color(c, d);
	while (b > 0)
	{
		b--;
		switch (a)
		{
		case 1:
			printf("\n");
			break;
		case 2:
			printf("  ");
			break;
		case 3:
			printf("⊙");
			break;
		case 4:
			switch (Player_Angle)
			{
			case 0:
				printf("▲");
				break;
			case 180:
				printf("▼");
				break;
			case 270:
				printf("◀");
				break;
			case 90:
				printf("▶");
				break;
			}
			break;
		}
	}
	Text_Color(DTC, DTB);
}

void Location(int* x, int* y, int angle, char n, int i)
{
	int w = 0, d = 0;
	switch (n)
	{
	case 'U':
		d = -i;
		break;
	case 'D':
		d = i;
		break;
	case 'L':
		w = -i;
		break;
	case 'R':
		w = i;
		break;
	}
	switch (angle)
	{
	case 0:
		*x += w;
		*y += d;
		break;
	case 90:
		*x -= d;
		*y += w;
		break;
	case 180:
		*x -= w;
		*y -= d;
		break;
	case 270:
		*x += d;
		*y -= w;
		break;
	}
}

void gotoxy(int x, int y)
{
	COORD Pos;
	Pos.X = x << 1;
	Pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

void Dialog(int type, int x, int y, int width, int height)
{
	int i, j;
	if (type == 0)
	{
		for (i = y; i < y + height; i++)
		{
			if (i == SCREEN_SIZE)
				break;
			for (j = x; j < x + width; j++)
			{
				if (j == SCREEN_SIZE)//현재 위치가 최대크기를 벗어나면 종료
					break;
				if (j == x || j == x + width - 1 || i == y || i == y + height - 1)
				{
					gotoxy(j, i);
					Draw(2, 1, DTC, BACK_WHITE);
				}
			}
		}
	}
	/*
					if (j == x)
					{
						if (i == y)
							printf("┌");
						else if (i == y + height - 1)
							printf("└");
						else
							printf("│");
					}
					else if (j == x + width - 1)
					{
						if (i == y)
							printf("┐");
						else if (i == y + height - 1)
							printf("┘");
						else
							printf("│");
					}
					else if (i == y || i == y + height - 1)
						printf("─");
				}*/

}
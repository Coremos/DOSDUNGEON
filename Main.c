/*
Version 5.5
-콘솔화면에서 잔상이 남는 버그 제거
-애니메이션 추가


-UI
플레이어 인벤토리 추가

-플레이어
상태 추가하기

-몬스터
몬스터 인벤토리 추가하기
몬스터가 죽으면 몬스터의 인벤토리의 아이템 드랍
몬스터 타격 애니메이션 추가v


*/
#include "Functions.h"
int Kill_Count = 0;

void Monster_Create(int species);
void Game_End();

void Map_Create()//0 : 길 / 1 : 벽 /
{
	int x = 1, y, i, ii;
	Map *New = (Map*)malloc(sizeof(Map)), *temp;//New = 새로 만들어질 데이터를 저장할 공간,할당
	New->Next = NULL;//새 맵의 다음맵을 초기화/이 부분이 없으면 아래에서 에러

	if (Map_Head == NULL)//맵이 아무것도 생성되지 않았을경우
		Map_Head = New;//새로만든 New를 헤드로 대입
	else//헤드가 비어있지 않을때
	{
		temp = Map_Head;//탐색용 포인터 초기화
		for (x = 2; temp->Next != NULL; x++)//가장 끝의 데이터를 탐색
			temp = temp->Next;//다음 주소로 이동
		temp->Next = New;//새로운 맵을 연결
	}
	New->ID = x;
	New->Size = 5 + New->ID * 4;//맵의 최대 사이즈를 설정
	Player_ID = x;//이 코드는 추후에 포탈과 접촉했을때로 바꿔주어야한다
	for (y = 0; y < New->Size + 2; y++)//맵의 최대사이즈만큼 반복
		for (x = 0; x < New->Size + 2; x++)
		{
			New->Map[x][y] = 0;//맵을 초기화
			New->Sight[x][y] = 0;//맵의 시야를 초기화
		}
	for (x = 0; x < New->Size + 2; x++)//모서리를 벽으로 배치한다
	{
		New->Map[0][x] = 1;
		New->Map[x][0] = 1;
		New->Map[New->Size + 1][x] = 1;
		New->Map[x][New->Size + 1] = 1;
	}
	for (y = 1; y < New->Size + 1; y++)
		for (x = 1; x < New->Size + 1; x++)
			if (!(x & 1) && !(y & 1))//x,y둘다 짝수이면 항상 벽이다.
			{
				New->Map[x][y] = 1;
				switch (Random(4))
				{
				case 1:
					New->Map[x][y - 1] = 1;
					break;
				case 2:
					New->Map[x][y + 1] = 1;
					break;
				case 3:
					New->Map[x - 1][y] = 1;
					break;
				case 4:
					New->Map[x + 1][y] = 1;
					break;
				}
			}
	//여기서 궁금한것, for문의 조건값을 rand함수로 채워놓으면 어떻게 될까?
	//예상되는건 매번 랜덤값을 나와서 10이었다가 2로 줄어들어서 2번만에 끝날지
	//처음만 랜덤으로 나올지
	for (i = 0; i < 10; i++)//몬스터 랜덤생성 부분
		Monster_Create(0);

	for (i = 0; i < New->Size * New->Size / 10; i++)//코인 랜덤생성
	{
		while (New->Map[x][y] != 0)
		{
			x = Random(New->Size + 1);
			y = Random(New->Size + 1);
		}
		New->Map[x][y] = 3;
	}

	while (1)
	{
		x = Random(New->Size + 1);
		y = Random(New->Size + 1);
		if (New->Map[x][y] == 0)
		{
			New->Map[x][y] = 4;
			break;
		}
	}

	if(New->ID >= 3)
		while (1)
		{
			x = Random(New->Size + 1);
			y = Random(New->Size + 1);
			if (New->Map[x][y] == 0)
			{
				New->Map[x][y] = 5;
				break;
			}
		}

	while (1)
	{
		x = Random(New->Size + 1);
		y = Random(New->Size + 1);
		if (New->Map[x][y] == 0)
		{
			Player_X = x;
			Player_Y = y;
			break;
		}
	}
}

Map* Map_Search(int a)
{
	//int i;
	Map *b = Map_Head;
	if (b == NULL)
		return NULL;
	while (b->Next != NULL)//가장 끝의 데이터를 탐색
	{
		if (b->ID == a)
			return b;
		b = b->Next;//다음 주소로 이동
	}
}


Weapon* Weapon_Search(int a)
{
	int i;
	Weapon *b = Weapon_Head;
	if (b == NULL)
		return NULL;
	while (b->Next != NULL)//가장 끝의 데이터를 탐색
	{
		if (b->Number == a)
			return b;
		b = b->Next;//다음 주소로 이동
	}
	return NULL;
}

/*/////Monster_Create(int Type,int HP, int MP, int STR, int DEX, int INT)
//몹을 생성하는 함수.
//int Type ==
//int HP ==
//int MP ==
//int STR ==
//int DEX ==
//int INT ==
*///////////////////////////////
void Monster_Create(int species)
{
	int i = 1, x, y;
	Monster *New = (Monster*)malloc(sizeof(Monster)), *Monster = Monster_Head;//New = 새로 만들어질 데이터를 저장할 공간,할당
	Map *Map = Map_Search(Player_ID);
	New->Next = NULL;
	if (Monster_Head == NULL)
		Monster_Head = New;
	else
	{
		for (i = 2; Monster->Next != NULL; i++)
			Monster = Monster->Next;
		Monster->Next = New;
	}
	New->Number = i;
	switch (Random(4))
	{
	case 1:
		New->Angle = 0;
		break;
	case 2:
		New->Angle = 180;
		break;
	case 3:
		New->Angle = 270;
		break;
	case 4:
		New->Angle = 90;
		break;
	}
	New->Species = species;
	New->Type = 0;
	New->HP_Max = 10;
	New->HP = New->HP_Max;
	New->MP_Max = 10;
	New->MP = New->MP_Max;
	New->STR = 10;
	New->DEX = 10;
	New->INT = 10;
	New->Map_ID = Player_ID;
	New->EXP = Random(10);
	New->Gold = 0;
	Monster = Monster_Head;
	while (Monster != NULL)
	{
		x = Random(Map->Size);
		y = Random(Map->Size);
		if (Map->Map[x][y] == 0)
		{
			Monster = Monster_Head;
			while (Monster != NULL)
			{
				if (Monster->X == x && Monster->Y == y && Monster->Map_ID == Player_ID)
					break;
				Monster = Monster->Next;
			}
		}
	}
	//printf("성공");
	New->X = x;
	New->Y = y;
}

/*/////Monster_Delete(int number)
//몹을 생성하는
//char *name == 파일 이름
//int back == 배경
*///////////////////////////////
void Monster_Delete(int number)
{
	Monster *Monster = Monster_Head, *Temp;
	if (Monster_Head->Number == number)
		Monster_Head = Monster_Head->Next;
	else
	{
		while (Monster != NULL)
		{
			Temp = Monster->Next;
			if (Temp->Number == number)
			{
				Monster->Next = Temp->Next;
				break;
			}
			Monster = Monster->Next;
		}
	}
}

/*/////Monster_Move()
//모든 몹들의 성향에 따라 움직임을 제어하는 함수
*///////////////////////////////
void Monster_Move()
{
	Monster *Monster = Monster_Head, *Temp;
	int t_x, t_y, a;
	while (Monster != NULL)
	{
		t_x = Monster->X;
		t_y = Monster->Y;
		/*switch (Monster->Behavior)
		{
		case 0:
		case 1:
		case 2:
		}
		Temp = Monster_Head;
		while (Temp != NULL)
		{
		if (Monster->Number != Temp->Number)
		{
		a = Temp->X - Monster->X;
		a = (a > 0) ? a : a * -1;
		if (Monster->Sight == a)
		{

		}
		else
		{

		}
		}
		Temp = Temp->Next;
		}*/
		Location(&t_x, &t_y, Monster->Angle, 'U', 1);//몬스터의 좌표를 저장해둔 임시데이터를 전진
													 //printf("%d", Location_Check(Monster->Map_ID, t_x, t_y, Monster->Number));
		if (Location_Check(Monster->Map_ID, t_x, t_y, Monster->Number))//해당 장소가 비어있는지 체크
		{
			//printf("방향");
			switch (Random(2))
			{
			case 1:
				Monster->Angle = (Monster->Angle == 0) ? 270 : Monster->Angle - 90;
				break;
			case 2:
				Monster->Angle = (Monster->Angle == 270) ? 0 : Monster->Angle + 90;
				break;
			}
		}
		else
		{
			switch (Random(1))
			{
			case 1:
				//printf("이동");
				Location(&Monster->X, &Monster->Y, Monster->Angle, 'U', 1);
				break;
			case 2:
				switch (Random(2))
				{
				case 1:
					Monster->Angle = (Monster->Angle == 0) ? 270 : Monster->Angle - 90;
					break;
				case 2:
					Monster->Angle = (Monster->Angle == 270) ? 0 : Monster->Angle + 90;
					break;
				}
				break;
			}
		}
		//printf("ID : %d ,%d", Monster->Number, Monster->Angle);
		Monster = Monster->Next;
	}
}

Monster* Monster_Search(int a)
{
	int i;
	Monster *b = Monster_Head;
	while (b->Next != NULL)//가장 끝의 데이터를 탐색
	{
		if (b->Number == a)
			return b;
		b = b->Next;//다음 주소로 이동
	}
	return NULL;
}


int Printer(char *string)
{
	int oldtime = clock(), i;
	char text[3] = "";//유니코드는 2개의 코드값으로 이루어져 있으므로 2개 저장, 1개는 널문자를 담기위함
	for (i = 0; i < strlen(string); i++)//총 길이만큼 반복
	{
		if (_kbhit())//키보드가 눌렸다면
		{
			while (string[i] != '\n')
			{
				if (string[i] < 0)
					strncpy(text, string + i++, 2);//코드 2개를 text에 복사, 처리가 끝난뒤 i을 1더해줌
				else
				{
					text[0] = string[i];//해당 코드를 text[0]에 대입
					text[1] = '\0';//코드 뒤에 널문자를 대입
				}
				printf("%s", text);//text에 들어있는 문자를 출력
				i++;
			}
			if (_getch() == 224)
				_getch();

			mciSendCommandW(SE1.wDeviceID, MCI_CLOSE, 0, NULL);
			mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD)(LPVOID)&SE1);
			mciSendCommand(SE1.wDeviceID, MCI_PLAY, MCI_NOTIFY, (DWORD)(LPVOID)&m_mciPlayParms);
		}
		if (clock() - oldtime > 125)//현재clock()값과 예전 clock()값이 *보다 많이 차이나면
		{
			if (string[i] < 0)
				strncpy(text, string + i++, 2);//코드 2개를 text에 복사, 처리가 끝난뒤 i을 1더해줌
			else
			{
				text[0] = string[i];//해당 코드를 text[0]에 대입
				text[1] = '\0';//코드 뒤에 널문자를 대입
			}

			if (string[i] != '\n' && string[i] != ' ')
			{
				mciSendCommandW(SE1.wDeviceID, MCI_CLOSE, 0, NULL);
				mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD)(LPVOID)&SE1);
				mciSendCommand(SE1.wDeviceID, MCI_PLAY, MCI_NOTIFY, (DWORD)(LPVOID)&m_mciPlayParms);
			}
			printf("%s", text);//text에 들어있는 문자를 출력
			oldtime = clock();//시간 갱신
			while (((string[i] == '.' || string[i] == '\n') || strcmp(text, "！") == 0 || strcmp(text, "。") == 0) && clock() - oldtime <= 500);
		}
		else
			i--;//시간이 되지않아 넘어가지 않았으므로 i++를 상쇄시켜주기 위함
	}
}

int Damage(int id);
void Attack(int id, int x, int y, int angle, int range, int penetrate, int knock, int damage, int pass);
//void Text_Color(int text, int back);
void Form_Init();
void Title_Draw(int menu);
int Random(int a);
//Map* Map_Search(int a);
//void Map_Create();
//void Location(int* x, int* y, int angle, char n, int i);
void Log(char *msg);

void Image_Create(int species)
{
	int i = 1, x, y;
	Image *New = (Image*)malloc(sizeof(Image)), *Image = Image_Head;//New = 새로 만들어질 데이터를 저장할 공간,할당
	Map *Map = Map_Search(Player_ID);
	New->Next = NULL;
	if (Monster_Head == NULL)
		Monster_Head = New;
	else
	{
		for (i = 2; Image->Next != NULL; i++)
			Image = Image->Next;
		Image->Next = New;
	}
	New->ID = i;
}

/*/////Image_Load(char *name, int back)
//이미지 파일을 읽어 배열에 색상정보를 저장하고, 해당 배열의 시작주소를 반환하는 함수.
//char *name == 파일 이름
//int back == 배경
//return arr
*///////////////////////////////
int(*Image_Load(char *name, int back))[SCREEN_SIZE]
{
	FILE *in = NULL;
static int arr[SCREEN_SIZE][SCREEN_SIZE] = { 0 };//정적변수를 써야 함수가 끝나도 보존
int ch, i = 1, w, h, x, y, padding;
char names[30] = "Image/";
strcat(names, name);
strcat(names, ".bmp");
//flose(in);
if ((in = fopen(names, "rb")) == NULL)
{
	printf("파일열기 에러 : %s", name);
	//fputs("파일 열기 에러!", stderr);
}
fseek(in, DATA_WIDTH, SEEK_SET);//처음부터 가로길이를 나타내는 곳까지 이동
w = fgetc(in);//가로길이 대입
fseek(in, DATA_HEIGHT, SEEK_SET);//처음부터 세로길이를 나타내는 곳까지 이동
h = fgetc(in);//세로길이 대입
fseek(in, DATA_START, SEEK_SET);
//(w & 1)? (w>>1)+1:w>>1;//홀수라면 나누고 나머지 반쪽을 위해 +1,짝수라면 나누기
padding = (4 - (((w & 1) ? (w >> 1) + 1 : w >> 1) % 4)) % 4;
for (x = 0, y = h - 1; (ch = fgetc(in)) != EOF; i++)//파일의 끝까지 ch에 데이터 읽어오기
{
	if (x < w)//진행된 x가 가로길이보다 짧을때
	{
		arr[x][y] = ch >> 4;//16진수에서의 첫번째자리문자를 대입
		x++;
		if (x < w)
		{
			arr[x][y] = ch % 16;//ch - (ch >> 4) * 16;//두번째자리 문자를 대입
			x++;
		}
	}//한번에 두번처리하는 이유는 데이터를 FF와 같이 두개의 문자로써 불러들이기 때문
	if (x >= w)//가로값 전부 처리했을때
	{
		if (y > 0)
			y--;//y값 감소시킬수 있으면 감소
		else
			break;//y가 첫번째줄이면 for문 종료
		x = 0;//가로 0부터
		fseek(in, padding, SEEK_CUR);//패딩의 크기만큼 커서를 이동시킨다
	}
}
for (y = 0; y < h; y++)
{
	for (x = 0; x < w; x++)
	{
		switch (arr[x][y])
		{
		case 0://검정
			arr[x][y] = BACK_BLACK;
			break;
		case 1://어두운 빨강
			arr[x][y] = BACK_DARKRED;
			break;
		case 2://어두운 초록
			arr[x][y] = BACK_DARKGREEN;
			break;
		case 3://어두운 노랑
			arr[x][y] = BACK_DARKYELLOW;
			break;
		case 4://어두운 파랑
			arr[x][y] = BACK_DARKBLUE;
			break;
		case 5://어두운 보라
			arr[x][y] = BACK_DARKPURPLE;
			break;
		case 6://어두운 하늘
			arr[x][y] = BACK_DARKSKY;
			break;
		case 7://어두운 회색
			arr[x][y] = BACK_DARKGRAY;
			break;
		case 8://회색
			arr[x][y] = BACK_GRAY;
			break;
		case 9://빨강
			arr[x][y] = BACK_RED;
			break;
		case 10://초록
			arr[x][y] = BACK_GREEN;
			break;
		case 11://노랑
			arr[x][y] = BACK_YELLOW;
			break;
		case 12://파랑
			arr[x][y] = BACK_BLUE;
			break;
		case 13://분홍
			arr[x][y] = BACK_PURPPLE;
			break;
		case 14://하늘
			arr[x][y] = BACK_SKY;
			break;
		case 15://흰색
			arr[x][y] = BACK_WHITE;
			break;
		}
		arr[x][y] == back ? arr[x][y] = -1 : 0;//해당 색상이 인자로 받은 배경색과 일치하면 -1을 대입
	}
}
fclose(in);//파일 입출력 스트림의 공간을 해제
return arr;//이미지의 색 데이터를 저장한 배열의 시작주소값을 전달
}

/*/////Image(int type, int distance, int angle)
//몹 이미지의 색상 정보를 저장한 배열의 시작주소를 반환하는 함수.
//int type == 몹의 종류
//int distance == 몹과 플레이어간의 거리
//int angle == 몹이 바라보는 방향
//return Monster_Image[type][distance][angle]
*///////////////////////////////
int(*Monster_Image(int species, int distance, int angle))[SCREEN_SIZE]
{
	int back = 240;//초기화하는 배경색은 하양
char name[10] = "";//**_*_*이 대입됨
switch (species)//타입에따라 배경색을 지정
{
case 0:
	back = 240;
	break;
case 1:
	back = 240;
	break;
}
angle < 0 ? angle += 360 : 0;//방향이 음수일때 양수로 바꾼다.//오류났었다
switch (angle)
{
case 0:
	angle = 0;//위0
	break;
case 90:
	angle = 3;//오른쪽
	break;
case 180:
	angle = 1;//아래
	break;
case 270:
	angle = 2;//왼쪽
	break;
}
if (Kill_Count < 5)
sprintf(name, "%02d_%d_%d", species, distance, angle);
else
{
	strcpy(name, "01");
	back = BACK_YELLOW;
}
return Image_Load(name, back);
}

void Animation(char *ani)
{
	int(*image)[SCREEN_SIZE], a, b, t, i, back;
	char tmp[C_MAX];

	if (strcmp(ani, "Attack_0") == 0)
	{
		t = 3;
		back = 160;
	}
	for (i = 0; i < t; i++)
	{
		strcpy(tmp, ani);
		sprintf(tmp, "%s_%d", tmp, i);
		//printf("%s", tmp);
		image = Image_Load(tmp, back);
		for (b = 0; b < 36; b++)//Attack이미지 출력
			for (a = 0; a < SCREEN_SIZE; a++)
				if (image[a][b] != -1)
				{
					gotoxy(a, b);
					Draw(2, 1, DTC, image[a][b]);
				}
		Sleep(100);
		for (b = 0; b < 36; b++)//Attack이미지에 해당하는 부분만 복원
			for (a = 0; a < SCREEN_SIZE; a++)
				if (image[a][b] != -1)
				{
					gotoxy(a, b);
					Draw(2, 1, DTC, Display[a][b]);
				}
	}
}

/*/////Location_Cheack(int id, int x, int y, int number)
//맵 좌표가 비어있으면 0, 비어있지 않으면 1을 반환하는 함수.
//int id == 맵ID
//int x == x좌표
//int y == y좌표
//int number == 자신의 고유숫자
//return 0 | 1
*///////////////////////////////
int Location_Check(int id, int x, int y, int number)
{
	Monster *temp = Monster_Head;
	Map *Map = Map_Search(id);
	if (Map->Map[x][y] != 1)
	{
		if (number > 0 && Player_ID == id && Player_X == x && Player_Y == y)
			return 1;
	}
	else
		return 1;
	while (temp != NULL)
	{
		if (temp->Map_ID == id && temp->X == x && temp->Y == y && temp->Number != number)
			return 1;
		temp = temp->Next;
	}
	return 0;
}

int Damage(int id)//a는
{
	Monster* Monster;
	Weapon* Weapon;
	//int damage;
	if (id == 0)
	{
		Weapon = Weapon_Search(Player_Weapon);
		return Player_STR + Weapon->STR;
	}
	else
	{
		Monster = Monster_Search(id);
		Weapon = Weapon_Search(Monster->Weapon);
		return Monster->STR + Weapon->STR;
	}
}

void EXP(int exp)
{
	char tmp[C_MAX] = "", j[3] = "이";
	Player_EXP += exp;
	while (Player_EXP > NextLevel)
	{
		Player_LV++;
		if (Interface_Language == 0)
		{
			switch (Player_LV % 10)
			{
			case 2:case 4:case 5:case 9:
				strcpy(j, "가");
			}
			sprintf(tmp, "플레이어의 레벨이 %d%s 되었습니다.", Player_LV, j);

		}
		else if (Interface_Language == 1)
		{
			sprintf(tmp, "Player are Level up to %d", Player_LV);
		}
		else if (Interface_Language == 2)
		{
			sprintf(tmp, "プレイアのレベルが%dになりました。", Player_LV);
		}
		Log(tmp);
		Player_EXP -= NextLevel;
		NextLevel *= 1.2;
	}
}

void Attack(int id, int x, int y, int angle, int range, int penetrate, int knock, int damage, int pass)
{
	int i, ii, t_x = x, t_y = y, a, b;
	Monster *Monster = Monster_Head;
	Map *Map = Map_Search(id);
	char tmp[C_MAX] = "";
	for (i = range; i > 0; i--)
	{
		Location(&t_x, &t_y, angle, 'U', 1);
		if (Map->Map[t_x][t_y] == 1 && pass == 0)
			break;
		Monster = Monster_Head;
		while (Monster != NULL && penetrate > 0)
		{
			if (t_x == Monster->X && t_y == Monster->Y && id == Monster->Map_ID)
			{
				Animation("Attack_0");
				Monster->HP -= damage;
				sprintf(tmp, "HP = %d", Monster->HP);
				Log(tmp);
				penetrate--;

				if (Monster->HP <= 0)//몬스터의 체력이 0이하일때
				{
					Player_Gold += Monster->Gold;
					EXP(Monster->EXP);
					Monster_Delete(Monster->Number);

					//Kill_Count += 1; 이거 키면 잔인해짐
					//mciSendCommandW(SE4.wDeviceID, MCI_CLOSE, 0, NULL);
					mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD)(LPVOID)&SE4);
					mciSendCommand(SE4.wDeviceID, MCI_PLAY, MCI_NOTIFY, (DWORD)(LPVOID)&m_mciPlayParms);
					if (Kill_Count == 1)
					{
						mciSendCommandW(BGM3.wDeviceID, MCI_CLOSE, 0, NULL);
					}
					
					if (Kill_Count == 5)
					{
						DTS = BACK_DARKRED;
						DTE = BACK_RED;
						mciSendCommandW(BGM2.wDeviceID, MCI_CLOSE, 0, NULL);
						mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD)(LPVOID)&BGM4);
						mciSendCommand(BGM4.wDeviceID, MCI_PLAY, MCI_DGV_PLAY_REPEAT, (DWORD)(LPVOID)&m_mciPlayParms);
					}
				}
			}
			Monster = Monster->Next;
		}
	}
}

void Form_Init()
{
	char buffer[40] = "";
	sprintf(buffer, "mode con:cols=%d lines=%d", (SCREEN_SIZE << 1)+1, SCREEN_SIZE);//화면의 사이즈를 조절하는 커맨드를 버퍼에 저장.
	system(buffer);//버퍼에 저장한 커맨드를 실행
	SetConsoleTitle(GAME_NAME);//콘솔창의 제목을 변경
	system("cls");//화면을 지움
	Text_Color(7, 0);//기본 텍스트의 색깔을 지정
}

void Title_Draw(int menu)
{
	int(*image)[SCREEN_SIZE], i, j;
	gotoxy(0, 0);
	image = Image_Load("Title", 0);
	for (i = 0; i < SCREEN_SIZE; i++)
	{
		for (j = 0; j < SCREEN_SIZE; j++)
			Draw(2, 1, DTC, image[j][i]);
		(i < SCREEN_SIZE - 1) ? printf("\n") : 0;
	}
	gotoxy(19, 20);
	if (menu == 0)
	{
		Text_Color(TEXT_RED, BACK_BLACK);
		printf("●");
		Text_Color(TEXT_GRAY, BACK_BLACK);
	}
	switch (Interface_Language)
	{
	case 0:
		printf("게임시작");
		break;
	case 1:
		printf("New Game");
		break;
	case 2:
		printf("始 め る");
		break;
	}
	gotoxy(19, 25);
	if (menu == 1)
	{
		Text_Color(TEXT_RED, BACK_BLACK);
		printf("●");
		Text_Color(TEXT_GRAY, BACK_BLACK);
	}
	switch (Interface_Language)
	{
	case 0:
		printf("게임방법");
		break;
	case 1:
		printf("How2play");
		break;
	case 2:
		printf("せつめい");
		break;
	}

	gotoxy(19, 30);
	if (menu == 2)
	{
		Text_Color(TEXT_RED, BACK_BLACK);
		printf("●");
		Text_Color(TEXT_GRAY, BACK_BLACK);
	}
	switch (Interface_Language)
	{
	case 0:
		printf("끝 내 기");
		break;
	case 1:
		printf("Q u  i t");
		break;
	case 2:
		printf("負 わ り");
		break;
	}
}

void Character_Move(char key)
{
	int t_x = Player_X, t_y = Player_Y, angle = Player_Angle;
	Map *Map = Map_Search(Player_ID);
	Location(&t_x, &t_y, Player_Angle, key, 1);
	//if (key == 'U' && Map->Map[t_x][t_y] != 1)
	if (Location_Check(Player_ID, t_x, t_y, 0) == 0)
		Location(&Player_X, &Player_Y, Player_Angle, key, 1);
	if (Map->Map[Player_X][Player_Y] == 5)
	{
		Game_End();
	}
	if (Map->Map[Player_X][Player_Y] == 3)
	{
		Player_Gold++;
		Map->Map[Player_X][Player_Y] = 0;
	}
	if (Map->Map[Player_X][Player_Y] == 4)
	{
		Map_Create();
	}
	
}

void Display_Node(Monster *list)
{
	Monster *p = list;
	while (p != NULL)
	{
		printf("%d-> ", p->Number);
		p = p->Next;
	}
	printf("\n");
}

void Log(char *msg)//(char* a,int b)
{
	int i, ii;// = 7;
	char tmp[C_MAX];
	FILE *out = fopen("DOSDUNGEON.LOG.TXT", "a");
	for (i = 0; i < 7; i++)
		if (Console[i][0] == '\0' || i == 6)
			break;
		else if (Console[6][0] != '\0' && i < 6)
			strcpy(Console[i], Console[i + 1]);
	strcpy(Console[i], msg);

	fprintf(out, msg);
	fprintf(out, "\n");
	fclose(out);
	//strcpy(Console[i + 1],'W0');

	//for (i = 0; i < 7 && ii > 6 ; i++)
	//	if (Console[i][0] == '\0')
	//		ii = i;

	//strncpy(Console[ii], "안녕" + atoi(Player_Gold, Buffer), strlen("안녕"));
	//if (Console[i][0] == '\0')
	//ii = i;


	//strncpy(Console[i]);
	//+ %dG / 골드
	//+ '%s' / 아이템
	//+ '%s' x %d / 아이템 여러개
	//Level Up
	//



	//아이템 획득했을때//''을(를) 획득했다!
	//아이템 획득했을때//''을(를) ''개 획득했다!

	//아이템 복수획득했을때//''을(를) 획득했다!

	//레벨업 했을때//''의 레벨이 ''이(가) 되었다!
	//''에게 ''의 피해를 입혔다!
	//''에게 ''의 피해를 입었다!

}

void Console_Render()
{
	int x, y, i, ii;
	Map *Map = Map_Search(Player_ID);
	Monster *temp = Monster_Head;
	if (Player_X > Map->Size - 2)
		x = Map->Size - 5;
	else if (Player_X < 3)
		x = 0;
	else
		x = Player_X - 3;
	if (Player_Y > Map->Size - 2)
		y = Map->Size - 5;
	else if (Player_Y < 3)
		y = 0;
	else
		y = Player_Y - 3;
	for (i = 0; i < 7; i++)
	{
		for (ii = 0; ii < 7; ii++)
			if (Map->Sight[x + ii][y + i])
			{
				temp = Monster_Head;
				while (temp != NULL)
				{
					if (x + ii == temp->X && y + i == temp->Y && Player_ID == temp->Map_ID)
					{
						if (DTE != BACK_RED)
							Draw(3, 1, 12, DTE);
						else
							Draw(3, 1, 0, DTE);
						goto EXIT;
					}
					temp = temp->Next;
				}
				if (x + ii == Player_X && y + i == Player_Y)
					if(DTE != BACK_RED)
						Draw(4, 1, 12, DTE);
					else
						Draw(4, 1, 0, DTE);
				else
					switch (Map->Map[x + ii][y + i])
					{
					case 0:
						Draw(2, 1, DTC, DTE);
						break;
					case 1:
						Draw(2, 1, DTC, DTW1);
						break;
					case 3:
						Draw(2, 1, DTC, 224);
						break;
					case 4:
						Draw(2, 1, DTC, BACK_BLUE);
						break;
					case 5:
						Draw(2, 1, DTC, BACK_DARKRED);
						break;
					}
			EXIT:
				0;
			}
			else
				Draw(2, 1, DTC, DTW2);

		printf("%s", Console[i]);
		Draw(2, (C_MAX - strlen(Console[i])) >> 1, DTC, DTB);
		(i < 6) ? Draw(1, 1, DTC, DTB) : 0;
	}
}

void HUD()
{
	const int Y_HUD = 36;//, X_HP = 6;
	int a;
	gotoxy(0, Y_HUD);
	printf("LV%02d EXP", Player_LV);
	a = (Player_EXP * 10) / NextLevel;
	Draw(2, a, DTC, 224);
	Draw(2, 10 - a, DTC, 96);
	//Draw(2, 1, DTC, DTB);
	printf("  HP");
	a = ((Player_HP < 0 ? 0 : Player_HP) * 10) / Player_HP_Max;
	Draw(2, a, DTC, 192);
	Draw(2, 10 - a, DTC, 64);
	/*
	while (1)
	{
	gotoxy(X_HP, Y_HUD);
	a = (Player_HP_Temp * 10) / Player_HP_Max;
	Draw(2, a, DTC, 192);
	Draw(2, 10 - a, DTC, 64);
	if (Player_HP_Temp != Player_HP)
	Player_HP_Temp > Player_HP ? Player_HP_Temp-- : Player_HP_Temp++;
	else
	break;
	Sleep(10);
	}*///스레드 안쓰면 못쓴다
	//Draw(2, 1, DTC, DTB);

	printf("  MP");
	a = (Player_MP * 10) / Player_MP_Max;
	Draw(2, a, DTC, 144);
	Draw(2, 10 - a, DTC, 16);

	printf("  %dG", Player_Gold);

	Draw(1, 1, DTC, DTB);//강제개행
}

void Character_View(int x, int y, int angle)
{
	int i, ii, iii, t_x = x, t_y = y, a[3][SIGHT_MAX + 1] = { 0 }, (*image)[SCREEN_SIZE], mob[SIGHT_MAX][2] = { 0 };
	int Stage[SCREEN_SIZE][SCREEN_SIZE] = { DTS };
	Map *Map = Map_Search(Player_ID);
	Monster *Monster;
	//system("cls");
	for (i = 0; i < SIGHT_MAX; i++)
		mob[i][0] = -1;
	for (i = 3; i + 1 > 0; i--)
	{
		Location(&t_x, &t_y, angle, 'L', 1);
		a[0][i] = Map->Map[t_x][t_y];//에러
		Location(&t_x, &t_y, angle, 'R', 1);
		a[1][i] = Map->Map[t_x][t_y];//제자리
		Location(&t_x, &t_y, angle, 'R', 1);
		a[2][i] = Map->Map[t_x][t_y];
		Location(&t_x, &t_y, angle, 'L', 1);
		if (t_x - 1 < 0 || t_y - 1 < 0)
			break;
		Location(&t_x, &t_y, angle, 'U', 1);//좌표를 한칸앞으로 이동합니다.
	}
	t_x = x;
	t_y = y;
	for (i = 0; i - 4 < 0; i++)
	{
		Map->Sight[t_x][t_y] = 1;//현재위치한곳
		Location(&t_x, &t_y, angle, 'L', 1);
		if (Map->Map[t_x][t_y] == 1)
			Map->Sight[t_x][t_y] = 1;
		Location(&t_x, &t_y, angle, 'R', 2);//오른쪽으로 두칸 좌표이동
		if (Map->Map[t_x][t_y] == 1)
			Map->Sight[t_x][t_y] = 1;
		Location(&t_x, &t_y, angle, 'L', 1);//왼쪽으로 한칸 좌표이동
		if (Map->Map[t_x][t_y] == 1)
			break;
		Location(&t_x, &t_y, angle, 'U', 1);//앞으로 한칸 좌표이동
	}

	for (i = 0; i < SCREEN_SIZE; i++)
		for (ii = 0; ii < SCREEN_SIZE; ii++)
			Stage[ii][i] = i < 27 ? DTS : DTE;
	for (i = 0; i < 30; i++)
	{
		if (i < 6)
		{
			if (a[0][3] == 1)
				Stage[i][i] = 0;
			if (a[2][3] == 1)
				Stage[43 - i][i] = 0;
			if (a[0][2] == 1 && !(a[0][3] == 1))
				Stage[i][6] = 0;
			if (a[2][2] == 1 && !(a[2][3] == 1))
				Stage[43 - i][6] = 0;
		}
		Stage[6][6 + i] = 0;//i < 30
		Stage[37][6 + i] = 0;//i < 30
		if (a[1][2] == 1)//i < 30
			Stage[7 + i][6] = 0;
		else
		{
			if (i < 20)
			{
				Stage[12][12 + i] = 0;
				Stage[31][12 + i] = 0;
				if (i < 18)
				{
					Stage[13 + i][31] = 0;
					if (i < 10)
					{
						if (!(a[1][1] == 1))
						{
							Stage[17][17 + i] = 0;
							Stage[26][17 + i] = 0;
							if (i < 8)
							{
								if (!(a[1][1] == 1))
								{
									Stage[18 + i][26] = 0;
									if (a[1][0] == 1)
										Stage[18 + i][17] = 0;
								}
								if (i < 4)
								{
									if (!(a[0][1] == 1))
									{
										Stage[13 + i][17] = 0;
										Stage[13 + i][26] = 0;
									}
									if (!(a[2][1] == 1))
									{
										Stage[30 - i][17] = 0;
										Stage[30 - i][26] = 0;
									}
								}
							}
						}
						if (i < 5)
						{
							Stage[7 + i][36 - i] = 0;
							Stage[36 - i][36 - i] = 0;
							if (!(a[1][1] == 1))
							{
								Stage[13 + i][30 - i] = 0;
								Stage[30 - i][30 - i] = 0;
							}
						}
					}
				}
			}
		}
		if (i < 18)
		{
			if (a[1][1] == 1 && !(a[1][2] == 1))
				Stage[13 + i][12] = 0;
			if (i < 5)
			{
				if (a[0][2] == 1 && !(a[1][2] == 1))
					Stage[7 + i][7 + i] = 0;
				if (a[2][2] == 1 && !(a[1][2] == 1))
					Stage[36 - i][7 + i] = 0;
				if (a[0][1] == 1 && !(a[0][2] == 1 || a[1][2] == 1))
				{
					Stage[7 + i][12] = 0;
					Stage[7 + i][31] = 0;
				}
				if (a[2][1] == 1 && !(a[2][2] == 1 || a[1][2] == 1))
				{
					Stage[32 + i][12] = 0;
					Stage[32 + i][31] = 0;
				}
				if (i < 4)
				{
					if (a[0][1] == 1 && !(a[1][2] == 1 || a[1][1] == 1))
						Stage[13 + i][13 + i] = 0;

					if (a[2][1] == 1 && !(a[1][2] == 1 || a[1][1] == 1))
						Stage[30 - i][13 + i] = 0;
				}
			}
		}
		//////색깔 입히기
		if (i < 6)
		{
			if (a[0][3] == 1)
			{
				for (ii = 0; ii <= i; ii++)
					Stage[ii][1 + i] = DTW2;
				for (ii = 0; ii < 29; ii++)
					Stage[i][7 + ii] = DTW2;
				for (ii = 0; ii <= i; ii++)
					Stage[ii][42 - i] = DTW2;
			}
			else if (a[0][2] == 1)
				for (ii = 0; ii < 29; ii++)
					Stage[i][7 + ii] = DTW1;
			if (a[2][3] == 1)
			{
				for (ii = 0; ii <= i; ii++)
					Stage[43 - ii][1 + i] = DTW2;
				for (ii = 0; ii < 29; ii++)
					Stage[43 - i][7 + ii] = DTW2;
				for (ii = 0; ii <= i; ii++)
					Stage[43 - ii][42 - i] = DTW2;
			}
			else if (a[2][2] == 1)
				for (ii = 0; ii < 29; ii++)
					Stage[43 - i][7 + ii] = DTW1;
			if (i < 5)
			{
				if (a[0][2] == 1)
				{
					for (ii = 0; ii <= i; ii++)
						Stage[7 + ii][8 + i] = DTW2;
					for (ii = 0; ii < 18; ii++)
						Stage[7 + i][13 + ii] = DTW2;
					for (ii = 0; ii <= i; ii++)
						Stage[7 + ii][35 - i] = DTW2;
				}
				else if (a[0][1] == 1)
					for (ii = 0; ii < 18; ii++)
						Stage[7 + i][13 + ii] = DTW1;
				if (a[2][2] == 1)
				{
					for (ii = 0; ii <= i; ii++)
						Stage[36 - ii][8 + i] = DTW2;
					for (ii = 0; ii < 18; ii++)
						Stage[36 - i][13 + ii] = DTW2;
					for (ii = 0; ii <= i; ii++)
						Stage[36 - ii][35 - i] = DTW2;
				}
				else if (a[0][1] == 1)
					for (ii = 0; ii < 18; ii++)
						Stage[36 - i][13 + ii] = DTW1;
				if (i < 4)
				{
					if (a[0][1] == 1)
					{
						for (ii = 0; ii <= i; ii++)
							Stage[13 + ii][14 + i] = DTW2;
						for (ii = 0; ii < 8; ii++)
							Stage[13 + i][18 + ii] = DTW2;
						for (ii = 0; ii <= i; ii++)
							Stage[13 + ii][29 - i] = DTW2;
					}
					else if (a[0][0] == 1)
						for (ii = 0; ii < 8; ii++)
							Stage[13 + i][18 + ii] = DTW1;
					if (a[2][1] == 1)
					{
						for (ii = 0; ii <= i; ii++)
							Stage[30 - ii][14 + i] = DTW2;
						for (ii = 0; ii < 8; ii++)
							Stage[30 - i][18 + ii] = DTW2;
						for (ii = 0; ii <= i; ii++)
							Stage[30 - ii][29 - i] = DTW2;
					}
					else if (a[2][0] == 1)
						for (ii = 0; ii < 8; ii++)
							Stage[30 - i][18 + ii] = DTW1;
				}
			}
		}
		//정면 벽 채색

		if (i < 18)
		{
			if (a[1][1] == 1)//1번째 벽 채색
				for (ii = 0; ii < 18; ii++)
					Stage[13 + ii][13 + i] = DTW1;
			if (a[1][0] == 1 && i < 8)//0번째 벽 채색
				for (ii = 0; ii < 8; ii++)
					Stage[18 + ii][18 + i] = DTW1;
		}
		if (a[1][2] == 1)//2번째 벽 채색 i < 30
			for (ii = 0; ii < 30; ii++)
				Stage[7 + ii][7 + i] = DTW1;
	}
	Monster = Monster_Head;
	while (Monster != NULL)
	{
		t_x = x;
		t_y = y;
		Location(&t_x, &t_y, angle, 'U', 1);
		if (Monster->X == t_x && Monster->Y == t_y && Monster->Map_ID == Player_ID)
		{
			mob[0][0] = Monster->Species;
			mob[0][1] = Monster->Angle - Player_Angle;
		}
		Location(&t_x, &t_y, angle, 'U', 1);
		if (Monster->X == t_x && Monster->Y == t_y && !(a[1][2] == 1) && Monster->Map_ID == Player_ID)
		{
			mob[1][0] = Monster->Species;
			mob[1][1] = Monster->Angle - Player_Angle;
		}
		Location(&t_x, &t_y, angle, 'U', 1);
		if (Monster->X == t_x && Monster->Y == t_y && !(a[1][2] == 1) && !(a[1][1] == 1) && Monster->Map_ID == Player_ID)
		{
			mob[2][0] = Monster->Species;
			mob[2][1] = Monster->Angle - Player_Angle;
		}
		Monster = Monster->Next;
	}

	for (i = SIGHT_MAX - 1; i >= 0; i--)
	{
		if (mob[i][0] != -1)
		{
			image = Monster_Image(mob[i][0], i, mob[i][1]);
			for (ii = 0; ii < 36; ii++)
				for (iii = 0; iii < SCREEN_SIZE; iii++)
					image[iii][ii] >= 0 ? Stage[iii][ii] = image[iii][ii] : 0;
		}
	}
	/*
	if (a[1][0] == 2)
	{
		printf("참이다.");
		image = Image_Load("Chest", BACK_BLUE);
		for (ii = 0; ii < 36; ii++)//Attack이미지 출력
			for (iii = 0; iii < SCREEN_SIZE; iii++)
				if (image[iii][ii] != -1)
				{
					gotoxy(iii, ii);
					Draw(2, 1, DTC, image[iii][ii]);
				}
	}
	else if (a[1][0] == 5)
	{
		image = Image_Load("Coin", BACK_GREEN);
		for (ii = 0; ii < 36; ii++)
			for (iii = 0; iii < SCREEN_SIZE; iii++)
				image[iii][ii] >= 0 ? Stage[iii][ii] = image[iii][ii] : 0;
	}
	else
	{
		printf("%d", a[1][0]);
	}
	*/

	if (a[1][2] == 5)
	{
		image = Image_Load("Chest_0", BACK_SKY);
		for (ii = 0; ii < 36; ii++)//Attack이미지 출력
			for (iii = 0; iii < SCREEN_SIZE; iii++)
				image[iii][ii] >= 0 ? Stage[iii][ii] = image[iii][ii] : 0;
	}
	else if (a[1][1] == 5)
	{
		image = Image_Load("Chest_1", BACK_SKY);
		for (ii = 0; ii < 36; ii++)//Attack이미지 출력
			for (iii = 0; iii < SCREEN_SIZE; iii++)
				image[iii][ii] >= 0 ? Stage[iii][ii] = image[iii][ii] : 0;
	}
	else if (a[1][0] == 5)
	{
		image = Image_Load("Chest_2", BACK_SKY);
		for (ii = 0; ii < 36; ii++)//Attack이미지 출력
			for (iii = 0; iii < SCREEN_SIZE; iii++)
				image[iii][ii] >= 0 ? Stage[iii][ii] = image[iii][ii] : 0;
	}
	else if (a[1][2] == 3)
	{
		image = Image_Load("Coin_0", BACK_SKY);
		for (ii = 0; ii < 36; ii++)
			for (iii = 0; iii < SCREEN_SIZE; iii++)
				image[iii][ii] >= 0 ? Stage[iii][ii] = image[iii][ii] : 0;
	}
	else if (a[1][1] == 3)
	{
		image = Image_Load("Coin_1", BACK_SKY);
		for (ii = 0; ii < 36; ii++)
			for (iii = 0; iii < SCREEN_SIZE; iii++)
				image[iii][ii] >= 0 ? Stage[iii][ii] = image[iii][ii] : 0;
	}
	else if (a[1][0] == 3)
	{
		image = Image_Load("Coin_2", BACK_SKY);
		for (ii = 0; ii < 36; ii++)
			for (iii = 0; iii < SCREEN_SIZE; iii++)
				image[iii][ii] >= 0 ? Stage[iii][ii] = image[iii][ii] : 0;
	}
	else
	{
		//printf("%d", a[1][0]);
	}


	for (i = 0; i < 36; i++)
	{
		for (ii = 0; ii < SCREEN_SIZE; ii++)
		{
			if (Display[ii][i] != Stage[ii][i])
			{
				Display[ii][i] = Stage[ii][i];
				gotoxy(ii, i);
				Draw(2, 1, DTC, Stage[ii][i]);
			}
		}
	}

	
	


}

int Key()
{
	int key;
	if (_kbhit())
	{
		key = _getch();
		if (key == 224)
		{
			return _getch();
		}
		else
		{
			return key;
		}
	}
}

void UI_Inventory()
{
	int cursor_x = 0, cursor_y = 0;

}

void Game_Key()
{
	Weapon *Weapon = Weapon_Search(Player_Weapon);
	int key;
	if (Interface_Focus == 0)
	{
		//if (_kbhit())
		{
			key = _getch();
			if (key == 224)
			{
				key = _getch();
				switch (key)
				{
				case KEY_UP:
					Character_Move('U');
					break;
				case KEY_DOWN:
					Character_Move('D');
					break;
				case KEY_LEFT:
					Player_Angle -= 90;
					(Player_Angle < 0) ? Player_Angle = 270 : 0;
					break;
				case KEY_RIGHT:
					Player_Angle += 90;
					(Player_Angle > 270) ? Player_Angle = 0 : 0;
					break;
				}
			}
			else
			{
				switch (key)
				{
				case KEY_R: case KEY_r:
					Map_Create();
					break;
				case KEY_M: case KEY_m:
					//system("cls");
					Monster_Create(1);
					//Display_Node(Monster_Head);
					//printf("몬스터 생성완료");
					//Player_ID -= 1;
					//Map_Render();
					break;
				case KEY_Z: case KEY_z:
					Attack(Player_ID, Player_X, Player_Y, Player_Angle, 1, 1, 1, 5, 0);
					break;
				case KEY_a:
					Player_ID -= 1;
					break;
					//case KEY_b: case KEY_B:

				}
			}
		}
		Monster_Move();
	}
	else if (Interface_Focus == 1)
	{
		if (_kbhit())
		{
			key = _getch();
			if (key == 224)
			{
				switch (key)
				{
				case KEY_UP:
					break;
				case KEY_DOWN:
					break;
				case KEY_LEFT:
					break;
				case KEY_RIGHT:
					break;
				}
			}
			else
			{
				switch (key)
				{
				case KEY_z:KEY_Z:
				break;
				case KEY_x:KEY_X:
				break;
				}
			}
		}
	}
}

void Rendering()
{
	//system("cls");
	Character_View(Player_X, Player_Y, Player_Angle);
	gotoxy(0, 36);
	HUD();
	gotoxy(0, 37);
	Console_Render();
}

void Game_Init()
{
	int i, j;
	Player_Angle = 0;
	Map_Create();
}

void Game_Intro()
{
	char buffer[500] = "";
	Dialog(0, 0, (SCREEN_SIZE >> 1) - 2, 44, 10);
	//Sleep(10000);
	mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD)(LPVOID)&BGM2);//Music1의 파일을 연다
	mciSendCommand(BGM2.wDeviceID, MCI_PLAY, MCI_DGV_PLAY_REPEAT, (DWORD)(LPVOID)&m_mciPlayParms);//Music1 재생
	//gotoxy(0, SCREEN_SIZE >> 1);
	gotoxy(2, SCREEN_SIZE >> 1);
	switch (Interface_Language)
	{
	case 0:
		strcpy(buffer, "던전을 탐험하는 당신의 이름은?\n");
		Printer(buffer);
		gotoxy(2, (SCREEN_SIZE >> 1) + 1);
		gets(Player_Name);
		system("cls");
		Dialog(0, 0, (SCREEN_SIZE >> 1) - 2, 44, 10);
		gotoxy(2, (SCREEN_SIZE >> 1));
		strcpy(buffer, "\'DOS\'의 세계에는 예전부터 전설로 전해져오는 던전이 있었다.\n");
		Printer(buffer);
		PressAnyKey();
		gotoxy(2, (SCREEN_SIZE >> 1) + 1);
		strcpy(buffer, "\'DOSDUNGEON\'. 그곳에는 어떠한 소원이라도 이루어 주는 보물상자가 있다고.\n");
		Printer(buffer);
		PressAnyKey();
		gotoxy(2, (SCREEN_SIZE >> 1) + 2);
		strcpy(buffer, "수많은 사람들이 그 보물상자를 찾기위해 모험을 떠났지만 그 누구도 돌아오지 않았다.\n");
		Printer(buffer);
		PressAnyKey();
		system("cls");
		Dialog(0, 0, (SCREEN_SIZE >> 1) - 2, 44, 10);
		gotoxy(2, (SCREEN_SIZE >> 1));
		strcpy(buffer, "\'");
		strcat(buffer, Player_Name);
		strcat(buffer, "\'은(는) 사라진 사람들을 찾기위해 모험을 떠났다.\n");
		Printer(buffer);
		PressAnyKey();
		system("cls");
		break;
	case 1:
		strcpy(buffer, "What's your name?\n");
		Printer(buffer);
		gotoxy(2, (SCREEN_SIZE >> 1) + 1);
		gets(Player_Name);
		system("cls");
		Dialog(0, 0, (SCREEN_SIZE >> 1) - 2, 44, 10);
		gotoxy(2, (SCREEN_SIZE >> 1));
		strcpy(buffer, "Once upon a time, There was a myth about a dungeon in \'DOS\'world.\n");
		Printer(buffer);
		PressAnyKey();
		gotoxy(2, (SCREEN_SIZE >> 1) + 1);
		strcpy(buffer, "\'DOSDUNGEON\'. In the dungeon there is a treasure,\n");
		Printer(buffer);
		PressAnyKey();
		gotoxy(2, (SCREEN_SIZE >> 1) + 2);
		strcpy(buffer, "the treasure help the the wish come true.\n");
		Printer(buffer);
		PressAnyKey();
		gotoxy(2, (SCREEN_SIZE >> 1) + 3);
		strcpy(buffer, "A lot of people went to the dungeon to find the treasure, but nobody come back.\n");
		Printer(buffer);
		PressAnyKey();
		system("cls");
		Dialog(0, 0, (SCREEN_SIZE >> 1) - 2, 44, 10);
		gotoxy(2, (SCREEN_SIZE >> 1));
		strcpy(buffer, "\'");
		strcat(buffer, Player_Name);
		strcat(buffer, "\' goes to adventure to find peoples.\n");
		Printer(buffer);
		PressAnyKey();
		system("cls");
		break;
	case 2:
		strcpy(buffer, "ドンジョンを旅するあなたの名前は？\n");
		Printer(buffer);
		gotoxy(2, (SCREEN_SIZE >> 1) + 1);
		gets(Player_Name);
		system("cls");
		Dialog(0, 0, (SCREEN_SIZE >> 1) - 2, 44, 10);
		gotoxy(2, (SCREEN_SIZE >> 1));
		strcpy(buffer, "DOS世界には昔から伝説に伝えてくるドンジョンがあった。\n");
		Printer(buffer);
		PressAnyKey();
		gotoxy(2, (SCREEN_SIZE >> 1) + 1);
		strcpy(buffer, "\'DOSDUNGEON\'。そこにはどんな願いでも叶えてくれる宝物があるって。\n");
		Printer(buffer);
		PressAnyKey();
		gotoxy(2, (SCREEN_SIZE >> 1) + 2);
		strcpy(buffer, "たくさんの人たちがその宝物を探すために旅を行ったけど誰一人も戻って来なかった。\n");
		Printer(buffer);
		PressAnyKey();
		system("cls");
		Dialog(0, 0, (SCREEN_SIZE >> 1) - 2, 44, 10);
		gotoxy(2, (SCREEN_SIZE >> 1));
		strcpy(buffer, "\'");
		strcat(buffer, Player_Name);
		strcat(buffer, "\'はその人達を探すために旅に行く。\n");
		Printer(buffer);
		PressAnyKey();
		system("cls");
		break;
	}
	mciSendCommandW(BGM2.wDeviceID, MCI_CLOSE, 0, NULL);
}

void Game_End()
{
	char buffer[500] = "";
	system("cls");
	switch (Interface_Language)
	{
	case 0:
		Dialog(0, 0, (SCREEN_SIZE >> 1) - 2, 44, 10);
		gotoxy(2, (SCREEN_SIZE >> 1));
		strcpy(buffer, "사람들을 찾던 도중, 나는 보물상자를 발견했다.\n");
		Printer(buffer);
		PressAnyKey();
		gotoxy(2, (SCREEN_SIZE >> 1) + 1);
		strcpy(buffer, "보물상자를 연 바로 그 순간.\n");
		Printer(buffer);
		PressAnyKey();
		gotoxy(2, (SCREEN_SIZE >> 1) + 2);
		Text_Color(TEXT_RED, BACK_BLACK);
		strcpy(buffer, "나는 슬라임이 되었다.");
		Printer(buffer);
		PressAnyKey();
		system("cls");
		Dialog(0, 0, (SCREEN_SIZE >> 1) - 2, 44, 10);
		gotoxy(2, (SCREEN_SIZE >> 1));
		if (Kill_Count > 0)
		{
			Text_Color(TEXT_RED, BACK_BLACK);
			strcpy(buffer, "당신이 죽인 사람은 ");
			Printer(buffer);
			printf("총 %d명.", Kill_Count);
			Text_Color(TEXT_GRAY, BACK_BLACK);
			PressAnyKey();
		}
		break;
	case 1:
		Dialog(0, 0, (SCREEN_SIZE >> 1) - 2, 44, 10);
		gotoxy(2, (SCREEN_SIZE >> 1));
		strcpy(buffer, "In dungeon, I found chest.\n");
		Printer(buffer);
		PressAnyKey();
		gotoxy(2, (SCREEN_SIZE >> 1) + 1);
		strcpy(buffer, "When I open the chest.\n");
		Printer(buffer);
		PressAnyKey();
		gotoxy(2, (SCREEN_SIZE >> 1) + 2);
		Text_Color(TEXT_RED, BACK_BLACK);
		strcpy(buffer, "I become a slime.");
		Printer(buffer);
		PressAnyKey();
		system("cls");
		Dialog(0, 0, (SCREEN_SIZE >> 1) - 2, 44, 10);
		gotoxy(2, (SCREEN_SIZE >> 1));
		if (Kill_Count > 0)
		{
			Text_Color(TEXT_RED, BACK_BLACK);
			strcpy(buffer, "You killed ");
			Printer(buffer);
			printf("%dpeople.", Kill_Count);
			Text_Color(TEXT_GRAY, BACK_BLACK);
			PressAnyKey();
		}
		break;
	case 2:
		Dialog(0, 0, (SCREEN_SIZE >> 1) - 2, 44, 10);
		gotoxy(2, (SCREEN_SIZE >> 1));
		strcpy(buffer, "人達を探すながら私は宝物を発見した。\n");
		Printer(buffer);
		PressAnyKey();
		gotoxy(2, (SCREEN_SIZE >> 1) + 1);
		strcpy(buffer, "宝物を開けた瞬間、\n");
		Printer(buffer);
		PressAnyKey();
		gotoxy(2, (SCREEN_SIZE >> 1) + 2);
		Text_Color(TEXT_RED, BACK_BLACK);
		strcpy(buffer, "私はスライムになった。");
		Printer(buffer);
		PressAnyKey();
		system("cls");
		Dialog(0, 0, (SCREEN_SIZE >> 1) - 2, 44, 10);
		gotoxy(2, (SCREEN_SIZE >> 1));
		if (Kill_Count > 0)
		{
			Text_Color(TEXT_RED, BACK_BLACK);
			strcpy(buffer, "あなたが殺した人は");
			Printer(buffer);
			printf("%d人.", Kill_Count);
			Text_Color(TEXT_GRAY, BACK_BLACK);
			PressAnyKey();
		}
		break;
	}
	exit(0);
}


void Game_Start()
{
	system("cls");
	Game_Intro();
	mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD)(LPVOID)&BGM3);//BGM3열기
	mciSendCommand(BGM3.wDeviceID, MCI_PLAY, MCI_DGV_PLAY_REPEAT, (DWORD)(LPVOID)&m_mciPlayParms);//BGM3반복재생
	while (1)
	{
		Rendering();
		Game_Key();
	}
}

void Game_Explanation()
{
	char buffer[500] = "";
	system("cls");
	switch (Interface_Language)
	{
	case 0:
		Dialog(0, 0, (SCREEN_SIZE >> 1) - 2, 44, 10);
		gotoxy(2, (SCREEN_SIZE >> 1));
		strcpy(buffer, "←→방향키로 바라보는 방향을 바꾸고,\n");
		Printer(buffer);
		PressAnyKey();
		gotoxy(2, (SCREEN_SIZE >> 1) + 1);
		strcpy(buffer, "↑↓방향키로 앞으로 이동, 뒤로 이동합니다.\n");
		Printer(buffer);
		PressAnyKey();
		gotoxy(2, (SCREEN_SIZE >> 1) + 2);
		strcpy(buffer, "Z키를 누르면 ");
		Printer(buffer);
		Text_Color(TEXT_RED, BACK_BLACK);
		strcpy(buffer, "죽입니다.");
		Printer(buffer);
		Text_Color(TEXT_GRAY, BACK_BLACK);
		PressAnyKey();
		break;
	case 1:
		Dialog(0, 0, (SCREEN_SIZE >> 1) - 2, 44, 10);
		gotoxy(2, (SCREEN_SIZE >> 1));
		strcpy(buffer, "Press ←→Key to change direction,\n");
		Printer(buffer);
		PressAnyKey();
		gotoxy(2, (SCREEN_SIZE >> 1) + 1);
		strcpy(buffer, "Press ↑↓Key to move.\n");
		Printer(buffer);
		PressAnyKey();
		gotoxy(2, (SCREEN_SIZE >> 1) + 2);
		strcpy(buffer, "Press Z Key to ");
		Printer(buffer);
		Text_Color(TEXT_RED, BACK_BLACK);
		strcpy(buffer, "KILL.");
		Printer(buffer);
		Text_Color(TEXT_GRAY, BACK_BLACK);
		PressAnyKey();
		break;
	case 2:
		Dialog(0, 0, (SCREEN_SIZE >> 1) - 2, 44, 10);
		gotoxy(2, (SCREEN_SIZE >> 1));
		strcpy(buffer, "←→を押して方向を変わって、");
		Printer(buffer);
		PressAnyKey();
		gotoxy(2, (SCREEN_SIZE >> 1) + 1);
		strcpy(buffer, "↑↓を押して移動します。\n");
		Printer(buffer);
		PressAnyKey();
		gotoxy(2, (SCREEN_SIZE >> 1) + 2);
		strcpy(buffer, "Ｚを押して");
		Printer(buffer);
		Text_Color(TEXT_RED, BACK_BLACK);
		strcpy(buffer, "ころす。");
		Printer(buffer);
		Text_Color(TEXT_GRAY, BACK_BLACK);
		PressAnyKey();
		break;
	}
}

int Form_Title()
{
	int menu = 0, key;
	Title_Draw(menu);
	while (1)
	{
		while (1)
		{
			if (_kbhit())
			{
				key = _getch();
				if (key == 224)
				{
					key = _getch();
					switch (key)
					{
					case KEY_UP:
						menu--;
						(menu < 0) ? menu = TITLE_MENU_MAX - 1 : 0;
						break;
					case KEY_DOWN:
						menu++;
						(menu - TITLE_MENU_MAX + 1 > 0) ? menu = 0 : 0;
						break;
					case KEY_LEFT:
						Interface_Language--;
						(Interface_Language < 0) ? Interface_Language = LANGUAGE_MAX - 1 : 0;
						break;
					case KEY_RIGHT:
						Interface_Language++;
						(Interface_Language - LANGUAGE_MAX + 1 > 0) ? Interface_Language = 0 : 0;
						break;
					}
					Title_Draw(menu);
					break;
				}
				if (key == KEY_Z || key == KEY_z || key == KEY_ENTER)
					return menu;
			}
		}
	}
}

int main()
{
	srand(time(NULL));//랜덤시드를 시간에 따라 생성
	HANDLE hConsole; // 콘솔 핸들
	CONSOLE_CURSOR_INFO ConsoleCursor; // 콘솔커서 정보 구조체
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // 핸들을 구하고
	ConsoleCursor.bVisible = 0; // true 보임 , false 안보임
	ConsoleCursor.dwSize = 1; // 커서 사이즈
	SetConsoleCursorInfo(hConsole, &ConsoleCursor); // 설정

	SE1.lpstrDeviceType = BGM1.lpstrDeviceType = BGM2.lpstrDeviceType = BGM3.lpstrDeviceType = BGM4.lpstrDeviceType = SE4.lpstrDeviceType = "mpegvideo";
	SE1.lpstrElementName = "Sound/SE/SE1.mp3";
	SE4.lpstrElementName = "Sound/SE/SE2.mp3";
	BGM1.lpstrElementName = "Sound/BGM/Title.mp3";
	BGM2.lpstrElementName = "Sound/BGM/Intro.mp3";
	BGM3.lpstrElementName = "Sound/BGM/Field1.mp3";
	BGM4.lpstrElementName = "Sound/BGM/X.mp3";

	while (1)
	{
		Form_Init();
		//mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD)(LPVOID)&BGM1);//BGM1열기
		//mciSendCommand(BGM1.wDeviceID, MCI_PLAY, MCI_DGV_PLAY_REPEAT, (DWORD)(LPVOID)&m_mciPlayParms);//BGM1반복재생

		//mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD)(LPVOID)&SE4);//BGM1열기
		//mciSendCommand(SE4.wDeviceID, MCI_PLAY, MCI_DGV_PLAY_REPEAT, (DWORD)(LPVOID)&m_mciPlayParms);//BGM1반복재생

		switch (Form_Title())
		{
		case 0:
			Game_Init();
			break;
		case 1:
			Game_Explanation();
			continue;
			//break;
		case 2:
			exit(0);
			break;
		}
		mciSendCommandW(BGM1.wDeviceID, MCI_CLOSE, 0, NULL);
		Game_Start();
	}
	return 0;
}

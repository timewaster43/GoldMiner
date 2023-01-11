#include <iostream>
#include <graphics.h>
#include <math.h>
#include <time.h>
using namespace std;

#define WIN_WIDTH 1080
#define WIN_HEIGHT 640
#define MAX_IMAGE 11
#define True 1
#define MAX_MINE 18
#define BROWN 0x0055AA
#define PI 3.1415926
#define MAX_ANGLE 80

IMAGE img[MAX_IMAGE];

enum MINE
{
	i_gold = 1,
	i_money = 3,
	i_soledown = 5,
	i_soleup = 7,
	i_stone = 9,
	i_bk = MAX_IMAGE - 1,
};
enum mine_type
{
	gold = 1,
	stone = 2,
	trophy = 3,
};
enum hook_state
{
	LEFT,
	RIGHT,
	M_NORMAL,
	M_LONG,
	M_SHORT,
};
struct Sole
{
	int x;
	int y;
	int width;
	int height;
	int coin;

}sole;
struct stuff
{
	int x;
	int y;
	int size;
	bool flag;
	int type;
	int val;
}mine[MAX_MINE];
struct HOOK
{
	int x;
	int y;
	int endx;
	int endy;
	int len;
	int angle;
	int dir;
	int vx;
	int vy;
	int state;
	int index;
}hook;
int distance(HOOK hook);
void gameinit()
{
	initgraph(WIN_WIDTH, WIN_HEIGHT);
	srand(GetTickCount());
	for (int i = 0; i < MAX_IMAGE - 1; i++)
	{
		char temp[25] = "";
		sprintf(temp, "./image/%d.jpg", i);
		loadimage(&img[i], temp);
	}
	loadimage(&img[i_bk], "./image/bg.jpg", WIN_WIDTH, WIN_HEIGHT - 120);
	sole.width = 140;
	sole.height = 120;
	sole.x = WIN_WIDTH / 2 - sole.width / 2;
	sole.y = 0;
	sole.coin = 0;
	for (int i = 0; i < MAX_MINE; i++)
	{
		mine[i].flag = true;
		mine[i].val = rand() % 200 + 1;
		mine[i].size = rand() % 70 + 50;
		mine[i].type = rand() % 3;
		mine[i].x = rand() % (WIN_WIDTH - 60);
		mine[i].y = rand() % (WIN_HEIGHT - 150) + 150;
	}
	hook.x = sole.x + 50;
	hook.y = sole.y + 80;
	hook.len = 50;
	hook.endx = hook.x;
	hook.endy = hook.y + hook.len;
	hook.angle = 0;
	hook.dir = RIGHT;
	hook.vx = 0;
	hook.vy = 0;
	hook.state = M_NORMAL;
	hook.index = -1;
}
void gamedraw()
{
	putimage(0, 120, &img[i_bk]);
	setfillcolor(RGB(255, 208, 52));
	solidrectangle(0, 0, WIN_WIDTH, 120);
	putimage(sole.x, sole.y, &img[i_soledown - 1], SRCAND);
	putimage(sole.x, sole.y, &img[i_soledown], SRCPAINT);
	for (int i = 0; i < MAX_MINE; i++)
	{
		if (mine[i].flag == true)
		{
			switch (mine[i].type)
			{
			case 1:
				putimage(mine[i].x, mine[i].y, &img[i_gold - 1], SRCAND);
				putimage(mine[i].x, mine[i].y, &img[i_gold], SRCPAINT);
				break;
			case 2:
				putimage(mine[i].x, mine[i].y, &img[i_stone - 1], SRCAND);
				putimage(mine[i].x, mine[i].y, &img[i_stone], SRCPAINT);
				break;
			case 3:
				putimage(mine[i].x, mine[i].y, &img[i_money - 1], SRCAND);
				putimage(mine[i].x, mine[i].y, &img[i_money], SRCPAINT);
				break;
			}
		}
	}
	setlinestyle(PS_SOLID, 5);
	line(hook.x, hook.y, hook.endx, hook.endy);

	char score[30] = "";
	sprintf(score, "分数：%d", sole.coin);
	settextcolor(WHITE);
	setbkmode(TRANSPARENT);
	settextstyle(30, 0, "等线");
	outtextxy(20, 20, score);
}
void HookRock()
{
	if (hook.state == M_NORMAL)
	{
		if (hook.dir == RIGHT)
		{
			Sleep(1);
			hook.angle++;
		}
		else
		{
			Sleep(1);
			hook.angle--;
		}
		if (hook.angle > MAX_ANGLE)
		{
			hook.dir = LEFT;
		}
		else if (hook.angle < -MAX_ANGLE)
		{
			hook.dir = RIGHT;
		}
		hook.endx = hook.x + sin(PI / 180 * hook.angle) * hook.len;
		hook.endy = hook.y + cos(PI / 180 * hook.angle) * hook.len;
	}
}
void gamecontrol(int speed)
{
	if (GetAsyncKeyState(VK_SPACE) && hook.state == M_NORMAL)
	{
		hook.vx = sin(PI / 180 * hook.angle) * speed;
		hook.vy = cos(PI / 180 * hook.angle) * speed;
		hook.state = M_LONG;
	}
	if (hook.endx<0 || hook.endx>WIN_WIDTH || hook.endy > WIN_HEIGHT)
	{
		hook.state = M_SHORT;
	}
	if (hook.state == M_LONG)
	{
		hook.endx += hook.vx;
		hook.endy += hook.vy;
	}
	else if (hook.state == M_SHORT)
	{
		hook.endx -= hook.vx;
		hook.endy -= hook.vy;
		if (distance(hook))
		{
			mine[hook.index].flag = false;
			hook.index = -1;
			hook.state = M_NORMAL;
		}
	}
	printf("%d %d\n", hook.vx, hook.vy);
}
void judge()
{
	for (int i = 0; i < MAX_MINE; i++)
	{
		if (mine[i].flag == true && hook.endx > mine[i].x && hook.endx<mine[i].x + mine[i].size && hook.endy>mine[i].y && hook.endy < mine[i].y + mine[i].size)
		{
			hook.index = i;
			break;
		}
	}
	if (hook.index != -1)
	{
		sole.coin += mine->val;
		hook.state = M_SHORT;
		mine[hook.index].x = hook.endx;
		mine[hook.index].y = hook.endy;
		if (distance(hook))
		{
			hook.state = M_NORMAL;
		}
	}
}

int main()
{
	gameinit();
	DWORD t1, t2;
	t1 = t2 = GetTickCount();
	BeginBatchDraw();
	while (True)
	{
		if (t2 - t1 > 15)
		{
			Sleep(0.1);
			HookRock();
			t1 = t2;
		}
		t2 = GetTickCount();
		gamedraw();
		FlushBatchDraw();
		gamecontrol(4);
		judge();
	}
	return 0;
}
int distance(HOOK hook)
{
	int dis = sqrt((float)(hook.x - hook.endx) * (hook.x - hook.endx) + (hook.y - hook.endy) * (hook.y - hook.endy));
	return dis <= hook.len;
}
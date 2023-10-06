#pragma once
// ħ�ﲿ��
#pragma once
#include "Map.h"
#include <random>
#include <ctime>
#include <iomanip>
#include <queue>
#include "head.h"
using namespace std;

extern int score;	// ȫ�ֱ������������ħ�����Ƿ�

const int primelist[10] = {5,7,11,13,17,19,23,29,31,37};	// �������������������
const Point stone_boom_area_1[5] = {Point(0,0), Point(0,-1), Point(-1,0), Point(0,1), Point(1,0)};	// ����1��ħʯ�ı�ը��Χ��0��ֻըһ��ħ�
const Point stone_boom_area_2[13] = {Point(0,0), Point(0,-1), Point(0,-2), Point(-1,-1), Point(-1,0), Point(-2,0), Point(-1,1), Point(0,1), Point(0,2), Point(1,1), Point(1,0), Point(2,0), Point(1,-1)}; // ����2��ħʯ�ı�ը��Χ 
const Point monster_boom_area_0[4] = {Point(0,-1), Point(-1,0), Point(0,1), Point(1,0)};	// ����0��ħ�ﱻ��ȼ�ı�ը��Χ
const Point monster_boom_area_1[12] = {Point(0,-1), Point(0,-2), Point(-1,-1), Point(-1,0), Point(-2,0), Point(-1,1), Point(0,1), Point(0,2), Point(1,1), Point(1,0), Point(2,0), Point(1,-1)}; // ����1����Elite����ħ�ﱻ��ȼ�ı�ը��Χ 

/*���岻ͬlevel��ħ��ı�ը�ȼ�*/
#define UN_LEVEL -1	// ��ħ��
#define LEVEL0 0	// ��ӦMONSTER_PROBABILITY
#define LEVEL1 1	// ��ӦELITE_MONSTER_PROBABILITY
#define LEVEL2 2

class Monster :public Point {
public:
	int color;				// ����ɫ��ʾħ������
	int level;				// ħ��ȼ�0~2���̶����ò�ͬ�ȼ��ı�ը��Χ

	Monster();
	Monster(int setlevel);
	Monster(int setlevel, int setcolor);
	Monster(int sx, int sy, int slevel, int scolor);
	void setMonster();
	void setMonster(int setlevel, int setcolor);

	void operator = (const Monster& nm);
};

class MonsterController {
public:
	Monster monsters[MONSTER_HEIGHT][MONSTER_WIDTH] = {0};	// �洢����ħ��
	int width = MONSTER_WIDTH, height = MONSTER_HEIGHT;		// ���ħ�����Ŀ�ȡ�ֻ��ʾ5��*10�У����һ����������
	default_random_engine e{(unsigned)(time(0))};			// һ����������������棬e()����������python�ĵ�������ÿ�η���һ��unsigned�����

	MonsterController();
	bool moveMonsters();
	void killMonster(int h, int w);
	void randomCreat();
	void showMonsters();
	bool update();
	bool inMonsters(int h, int w);
	queue<Monster>* boom(Point choose_stone_p, Point mclick_p);
};

extern queue<Monster> to_boom;	// ���ħ���������б��ݹ��ȼ��ħ������(height, width, level)
extern queue<Monster> to_render;	// ���ħ���������б��ݹ��ȼ��ħ�����꣬��render.cppȥ��Ⱦͼ��

void testfoo();
#pragma once
// 魔物部分
#pragma once
#include "Map.h"
#include <random>
#include <ctime>
#include <iomanip>
#include <queue>
#include "head.h"
using namespace std;

extern int score;	// 全局变量，由消灭的魔物数记分

const int primelist[10] = {5,7,11,13,17,19,23,29,31,37};	// 用来生成随机数的素数
const Point stone_boom_area_1[5] = {Point(0,0), Point(0,-1), Point(-1,0), Point(0,1), Point(1,0)};	// 定义1级魔石的爆炸范围（0级只炸一个魔物）
const Point stone_boom_area_2[13] = {Point(0,0), Point(0,-1), Point(0,-2), Point(-1,-1), Point(-1,0), Point(-2,0), Point(-1,1), Point(0,1), Point(0,2), Point(1,1), Point(1,0), Point(2,0), Point(1,-1)}; // 定义2级魔石的爆炸范围 
const Point monster_boom_area_0[4] = {Point(0,-1), Point(-1,0), Point(0,1), Point(1,0)};	// 定义0级魔物被点燃的爆炸范围
const Point monster_boom_area_1[12] = {Point(0,-1), Point(0,-2), Point(-1,-1), Point(-1,0), Point(-2,0), Point(-1,1), Point(0,1), Point(0,2), Point(1,1), Point(1,0), Point(2,0), Point(1,-1)}; // 定义1级（Elite级）魔物被点燃的爆炸范围 

/*定义不同level下魔物的爆炸等级*/
#define UN_LEVEL -1	// 非魔物
#define LEVEL0 0	// 对应MONSTER_PROBABILITY
#define LEVEL1 1	// 对应ELITE_MONSTER_PROBABILITY
#define LEVEL2 2

class Monster :public Point {
public:
	int color;				// 用颜色表示魔物类型
	int level;				// 魔物等级0~2，固定设置不同等级的爆炸范围

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
	Monster monsters[MONSTER_HEIGHT][MONSTER_WIDTH] = {0};	// 存储所有魔物
	int width = MONSTER_WIDTH, height = MONSTER_HEIGHT;		// 输出魔物大军的宽度。只显示5列*10行，最后一行用作更新
	default_random_engine e{(unsigned)(time(0))};			// 一个生成随机数的引擎，e()方法类似于python的迭代器，每次返回一个unsigned随机数

	MonsterController();
	bool moveMonsters();
	void killMonster(int h, int w);
	void randomCreat();
	void showMonsters();
	bool update();
	bool inMonsters(int h, int w);
	queue<Monster>* boom(Point choose_stone_p, Point mclick_p);
};

extern queue<Monster> to_boom;	// 存放魔物区中所有被递归点燃的魔物坐标(height, width, level)
extern queue<Monster> to_render;	// 存放魔物区中所有被递归点燃的魔物坐标，供render.cpp去渲染图像

void testfoo();
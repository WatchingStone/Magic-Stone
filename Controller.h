#pragma once
#include "Map.h"
#include "MagicCube.h"
#include <iomanip>
#include "head.h"

class StoneController: public Virtual {
public:
	int stonelist[5][3] = {0};		// 以5行3列存储所有魔石：有红RED，蓝BLUE，黄YELLOW，绿GREEN，白WHITE，每种魔石有3级
	
	// 传入的ns.x为魔石种类，ns.y为魔石等级
	int addStone(Point ns);
	// 传入的ns.x为魔石种类，ns.y为魔石等级
	int subStone(Point ns);
	void showStoneList();
	void init();
};

class CubeController{
public:
	BaseMCube* cubelist[MAX_CUBE_COUNT];
	int rear;								// 指向魔法阵数组尾部
	StoneController* stonelist;				

	CubeController();

	void addCube(BaseMCube ncube);
	void showCube();
	void initStoneList(StoneController* stl);
	void update(Map& mymap);// 固定时间间隔更新魔法阵生产魔石和魔力之源
	void init();
};

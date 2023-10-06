#pragma once

#include <ctime>
#include "Map.h"
#include "head.h"

class BaseMCube {
public:
	int category;
	int orient;								// 表示魔法阵的方向，0~3表示旋转后的四种方向
	int cube_area_n;						// 魔法阵所占的n*n范围，基本都是3*3，1个是4*4,1个是5*5
	Point* cube_area;						// 存储魔法阵的点阵图形状
	Point exportp;							// 显示魔法阵的输出方块坐标
	Point shift;							// 魔法阵左上角所在地图中的绝对位置
	Point msrcs[5];							// 存储魔法阵覆盖的魔力之源
	clock_t clocks = 0;						// 魔法阵自身的时钟，每秒增长1000
	Virtual* stonelist = NULL;				// 使用纯抽象类作为接口，只使用指针访问setStone()接口

	BaseMCube();
	BaseMCube(int n, int ex, int ey);
	void setStoneList(Virtual* stl);
	void setN(int n);
	void turnRound();
	void showCube();
	void setShift(Point p);
	int setInMap(Map& mymap);
	int setInMap(Map &mymap, Point pos);			// 输入鼠标对应的核心点pos，将地图上对应位置设置为魔法阵
	int setSourceInMap(Map &mymap);					// 输出新的魔力之源
	int produceStone(Map& mymap);	// 生产魔石
	Point checkSource(Map mymap);
	void operator = (const BaseMCube & nb);
};

// 以下为各形状魔法阵的具体定义
class Cube_V1 :public BaseMCube {
public:
	Cube_V1();
};

class Cube_V2 :public BaseMCube {
public:
	Cube_V2();
};

class Cube_T :public BaseMCube {
public:
	Cube_T();
};

class Cube_B :public BaseMCube {
public:
	Cube_B();
};

class Cube_C :public BaseMCube {
public:
	Cube_C();
};

class Cube_L :public BaseMCube {
public:
	Cube_L();
};
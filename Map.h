// 内含类Point, MFloor, Map
#pragma once
#include <easyx.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "head.h"
using namespace std;

class Point {
public:
	int x, y;	// 该点的横纵坐标
	int v;		// 该点的值					

	Point();
	Point(int sx, int sy);
	Point(int sx, int sy, int sv);

	void setx(int sx);
	void sety(int sy);
	void setv(int sv);
	void setP(int sx, int sy);
	void setP(int sx, int sy, int sv);
	void setP(Point p);
	Point& convert(Point lu);		// 输入绝对坐标作为(左上角)原点，计算返回本Point相对于坐标原点得到的绝对坐标
	bool inRec(RECT rec);			// 检测该点是否在某矩形内
};

class MFloor : public Point {
public:
	int msource;			// 魔力之源的种类		
	int cube_category;		// 所属魔法阵的种类
	int cube_no;			// 所属魔法阵的具体哪一块
	int cube_orient;		// 所属魔法阵的旋转方向

	MFloor();
	MFloor(int sx, int sy);

	void setSource(int srcCategory);
	void setInCube(int c_c, int c_no, int c_o);
};

class Map {
public:
	int width, height;
	MFloor* ori_map;	
	MFloor* map;

	// 地图初始化需要载入地图的宽w与高h，再传入一个魔力之源的数组msrc及数组长度n
	Map();
	Map(int w, int h);
	Map(int w, int h, int n, Point* msrc);

	void reloadmap();			// ori_map存储地图文件中的原始魔力之源信息，map存储动态的地图信息
	void showSource();
	void showCubeCat();
	Map& initMap(char path[20]);
};

// 虚构类，只提供接口用来解耦
class Virtual {
public:
	virtual int addStone(Point ns) = 0;
};
// 地图初始化，输入地图文件路径path，返回Map指针


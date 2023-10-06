#include <easyx.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <stdlib.h>
#include "Map.h"

using namespace std;

Point::Point() :x(0), y(0), v(0) {}	// 构造函数
Point::Point(int sx, int sy) :x(sx), y(sy), v(0) {}
Point::Point(int sx, int sy, int sv) :x(sx), y(sy), v(sv) {}
void Point::setx(int sx) { x = sx; }
void Point::sety(int sy) { y = sy; }
void Point::setv(int sv) { v = sv; }
void Point::setP(int sx, int sy) { x = sx; y = sy; }
void Point::setP(int sx, int sy, int sv) { x = sx; y = sy; v = sv; }
void Point::setP(Point p){ x = p.x; y = p.y; v = p.v; }
Point& Point::convert(Point lu) {
	Point newp;
	newp.setP(lu.x + x, lu.y + y);
	return newp;
}
bool Point::inRec(RECT rec) {		// 检测该点是否在矩形范围内
	if (x >= rec.left && x <= rec.right && y >= rec.top && y <= rec.bottom) return TRUE;
	return FALSE;
}



MFloor::MFloor() :Point(0, 0) {
	msource = 0;
	cube_category = 0;
	cube_orient = 0;
}
MFloor::MFloor(int sx, int sy) :Point(sx, sy) {	// 构造函数
	msource = 0;
	cube_category = 0;
	cube_orient = 0;
}
void MFloor::setSource(int srcCategory) {		// 为当前地图块添加魔力之源
	msource = srcCategory;
}
void MFloor::setInCube(int c_c, int c_no, int c_o) {		// 为当前地图块添加魔法阵的一部分
	cube_category = c_c;
	cube_no = c_no;
	cube_orient = c_o;
}


Map::Map() {}
// 地图初始化需要载入地图的宽w与高h，再传入一个魔力之源的数组msrc及数组长度n
Map::Map(int w, int h) :width(w), height(h) {
	ori_map = new MFloor[w * h];
	int i, j;
	int mx, my;
	for (i = 0; i < w; i++) {				// 初始化地图
		for (j = 0; j < h; j++) {
			ori_map[i * w + j].setP(i, j);
		}
	}
	reloadmap();
}
Map::Map(int w, int h, int n, Point* msrc) :width(w), height(h) {
	ori_map = new MFloor[w * h];
	int i, j;
	int mx, my;
	for (i = 0; i < w; i++) {				// 初始化地图
		for (j = 0; j < h; j++) {
			ori_map[i * w + j].setP(i, j);
		}
	}
	for (i = 0; i < n; i++) {	// 
		mx = msrc[i].x;
		my = msrc[i].y;
		ori_map[mx * w + my].setSource(msrc[i].v);
	}
	reloadmap();
}
// 将ori_map深拷贝给map
void Map::reloadmap() {
	map = new MFloor[width * height];
	for (int i = 0; i < width * height; i++) {
		map[i].setP(ori_map[i]);
		map[i].setSource(ori_map[i].msource);
	}
}
// 显示当前地图上的魔力之源分布
void Map::showSource() {
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			cout << setw(10) << left << map[i * width + j].msource;
			if (j == height - 1) {
				cout << endl;
			}
		}
	}
	cout << "------------" << endl;
}
// 显示当前地图上的魔法阵分布
void Map::showCubeCat() {
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			cout << setw(10) << left << map[i * width + j].cube_category;
			if (j == height - 1) {
				cout << endl;
			}
		}
	}
	cout << "------------" << endl << endl;
}

// 地图初始化，输入地图文件路径path，返回Map引用
Map& Map::initMap(char path[20]) {
	ifstream mapf;
	mapf.open(path, ios::in);
	char temp[6] = {0}, temp1[6] = {0}, temp2[6] = {0};
	int w, h, n;
	int x, y, v;
	Point* mp = new Point[MAX_SOURCE_COUNT];

	mapf >> w >> h >> n;
	//cout << w << "  " << h << "  " << n << endl;
	for (int i = 0; i < n; i++) {
		mapf >> x >> y >> v;
		//cout << x << "  " << y << "  " << v << endl;
		mp[i].setP(x, y, v);
	}
	Map mymap(w, h, n, mp);
	//mymap.showMap();
	return mymap;
}
// �ں���Point, MFloor, Map
#pragma once
#include <easyx.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "head.h"
using namespace std;

class Point {
public:
	int x, y;	// �õ�ĺ�������
	int v;		// �õ��ֵ					

	Point();
	Point(int sx, int sy);
	Point(int sx, int sy, int sv);

	void setx(int sx);
	void sety(int sy);
	void setv(int sv);
	void setP(int sx, int sy);
	void setP(int sx, int sy, int sv);
	void setP(Point p);
	Point& convert(Point lu);		// �������������Ϊ(���Ͻ�)ԭ�㣬���㷵�ر�Point���������ԭ��õ��ľ�������
	bool inRec(RECT rec);			// ���õ��Ƿ���ĳ������
};

class MFloor : public Point {
public:
	int msource;			// ħ��֮Դ������		
	int cube_category;		// ����ħ���������
	int cube_no;			// ����ħ����ľ�����һ��
	int cube_orient;		// ����ħ�������ת����

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

	// ��ͼ��ʼ����Ҫ�����ͼ�Ŀ�w���h���ٴ���һ��ħ��֮Դ������msrc�����鳤��n
	Map();
	Map(int w, int h);
	Map(int w, int h, int n, Point* msrc);

	void reloadmap();			// ori_map�洢��ͼ�ļ��е�ԭʼħ��֮Դ��Ϣ��map�洢��̬�ĵ�ͼ��Ϣ
	void showSource();
	void showCubeCat();
	Map& initMap(char path[20]);
};

// �鹹�ֻ࣬�ṩ�ӿ���������
class Virtual {
public:
	virtual int addStone(Point ns) = 0;
};
// ��ͼ��ʼ���������ͼ�ļ�·��path������Mapָ��


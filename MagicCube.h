#pragma once

#include <ctime>
#include "Map.h"
#include "head.h"

class BaseMCube {
public:
	int category;
	int orient;								// ��ʾħ����ķ���0~3��ʾ��ת������ַ���
	int cube_area_n;						// ħ������ռ��n*n��Χ����������3*3��1����4*4,1����5*5
	Point* cube_area;						// �洢ħ����ĵ���ͼ��״
	Point exportp;							// ��ʾħ����������������
	Point shift;							// ħ�������Ͻ����ڵ�ͼ�еľ���λ��
	Point msrcs[5];							// �洢ħ���󸲸ǵ�ħ��֮Դ
	clock_t clocks = 0;						// ħ���������ʱ�ӣ�ÿ������1000
	Virtual* stonelist = NULL;				// ʹ�ô���������Ϊ�ӿڣ�ֻʹ��ָ�����setStone()�ӿ�

	BaseMCube();
	BaseMCube(int n, int ex, int ey);
	void setStoneList(Virtual* stl);
	void setN(int n);
	void turnRound();
	void showCube();
	void setShift(Point p);
	int setInMap(Map& mymap);
	int setInMap(Map &mymap, Point pos);			// ��������Ӧ�ĺ��ĵ�pos������ͼ�϶�Ӧλ������Ϊħ����
	int setSourceInMap(Map &mymap);					// ����µ�ħ��֮Դ
	int produceStone(Map& mymap);	// ����ħʯ
	Point checkSource(Map mymap);
	void operator = (const BaseMCube & nb);
};

// ����Ϊ����״ħ����ľ��嶨��
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
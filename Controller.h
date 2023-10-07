#pragma once
#include "Map.h"
#include "MagicCube.h"
#include <iomanip>
#include "head.h"

class StoneController: public Virtual {
public:
	int stonelist[5][3] = {0};		// ��5��3�д洢����ħʯ���к�RED����BLUE����YELLOW����GREEN����WHITE��ÿ��ħʯ��3��
	
	// �����ns.xΪħʯ���࣬ns.yΪħʯ�ȼ�
	int addStone(Point ns);
	// �����ns.xΪħʯ���࣬ns.yΪħʯ�ȼ�
	int subStone(Point ns);
	void showStoneList();
	void init();
};

class CubeController{
public:
	BaseMCube* cubelist[MAX_CUBE_COUNT];
	int rear;								// ָ��ħ��������β��
	StoneController* stonelist;				

	CubeController();

	void addCube(BaseMCube ncube);
	void showCube();
	void initStoneList(StoneController* stl);
	void update(Map& mymap);// �̶�ʱ��������ħ��������ħʯ��ħ��֮Դ
	void init();
};

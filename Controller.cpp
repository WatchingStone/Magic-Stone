#include "Map.h"
#include "MagicCube.h"
#include "Controller.h"

// 传入的ns.x为魔石种类，ns.y为魔石等级
int StoneController::addStone(Point ns) {
	switch (ns.x) {
		case 100:
		case 101:
		case 102:
		case 103:
		case 104:
			switch (ns.y) {
				case 0:
				case 1:
				case 2:
					stonelist[ns.x - RED_SIGN][ns.y - 1] += 1;
					return OK;
			}
		default: return ERROR;
	}
}
// 传入的ns.x为魔石种类（已减去RED_SIGN），ns.y为魔石等级
int StoneController::subStone(Point ns) {
	switch (ns.x) {
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
			switch (ns.y) {
				case 0:
				case 1:
				case 2:
					if (stonelist[ns.x][ns.y] > 0) {
						stonelist[ns.x][ns.y] -= 1;
						cout << "OK!" << endl;
						return OK;
					}
			}
		default: break;
	}
	return ERROR;
}
void StoneController::showStoneList() {
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 3; j++) {
			cout << setw(5) << left << stonelist[i][j];
			if (j == 2) cout << endl;
		}
	}
	cout << "-----------" << endl;
}
void StoneController::init() {
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 3; j++) stonelist[i][j] = 0;
}

CubeController::CubeController() {
	rear = 0;
}

// 在数组中新增一项BaseMCube ncube，需要提前为ncube设置好shift等值
// 初始化时会直接给每个cube设置好stonelist
void CubeController::addCube(BaseMCube ncube) {
	BaseMCube* temp = new BaseMCube;
	*temp = ncube;
	cubelist[rear] = temp;
	cubelist[rear]->setStoneList(stonelist);
	rear++;
}
void CubeController::showCube() {
	for (int i = 0; i < rear; i++) {
		cout << "``````CUBE``````" << endl;
		cubelist[i]->showCube();	
	}
}
void CubeController::initStoneList(StoneController* stl) {
	stonelist = stl;
}

// 固定时间间隔更新魔法阵生产魔石
void CubeController::update(Map& mymap) {
	int i;

	for (i = 0; i < rear;i++) {
	//for (i = rear - 1; i >=0 ; i--) {
		cubelist[i]->setInMap(mymap);
		cubelist[i]->produceStone(mymap);
		cubelist[i]->setSourceInMap(mymap);
	}
}

void CubeController::init() {
	for (int i = 0; i < MAX_CUBE_COUNT;i++)		cubelist[i] = NULL;
	rear = 0;
	stonelist = NULL;
}
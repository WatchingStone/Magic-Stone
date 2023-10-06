#include "Monster.h"

int score = 0;

// 这两个queue必须定义在函数体外部才能作为全局变量
queue<Monster> to_boom;	// 存放魔物区中所有被递归点燃的魔物坐标(height, width, level)
queue<Monster> to_render;	// 存放魔物区中所有被递归点燃的魔物坐标，供render.cpp去渲染图像

Monster::Monster() :Point(), color(0), level(UN_LEVEL) {}
Monster::Monster(int setlevel) :Point(), color(0), level(setlevel) {}
Monster::Monster(int setlevel, int setcolor) :Point(), color(setcolor), level(setlevel) {}
Monster::Monster(int sx, int sy, int slevel, int scolor) : Point(sx, sy), color(scolor), level(slevel) {}
void Monster::setMonster() { x = 0; y = 0; color = 0;  level = UN_LEVEL; }
void Monster::setMonster(int setlevel, int setcolor) { x = 0; y = 0; color = setcolor; level = setlevel; }
void Monster::operator = (const Monster& nm) {
	setP(nm.x, nm.y);
	color = nm.color;
	level = nm.level;
}


MonsterController::MonsterController() {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			monsters[i][j].setMonster();
		}
	}
	randomCreat();
}

// 将现有魔物情况整体下移一行。若倒数第二行（显示在画面中的倒数第一行）仍有魔物未处理，则返回TRUE
bool MonsterController::moveMonsters() {
	int i, j;

	for (j = 0; j < width; j++) {
		if (monsters[height - 2][j].color > 0)  return  true;	// 若倒数第二行仍留有魔物，则返回TRUE
	}
	for (i = height - 1; i >= 1; i--) {
		for (j = 0; j < width; j++) {
			monsters[i][j] = monsters[i - 1][j];			// 否则，逐行向下复制魔物情况
		}
	}
	return false;
}

// 与魔石爆炸关联的杀死魔物方法
void MonsterController::killMonster(int h, int w) {
	if (monsters[h][w].color > 0) {
		monsters[h][w].color = 0;
		monsters[h][w].level = 0;
		score++;					// 每消灭1个魔物+1分
	}
}

// 若没有从文件中读取固定的魔物大军信息，则随机生成。（必须在.moveMonsters()方法之后调用）
void MonsterController::randomCreat() {
	int j;
	int randseed, res;
	int tcolor, tlevel;

	// 在第一行随机生成新魔物，只生成一次随机值，降低运算量
	randseed = e();
	for (j = 0; j < width; j++) {
		res = ((unsigned)(randseed / primelist[j])) % 100;
		if (res < MONSTER_PROBABILITY) {
			tcolor = res % 5 + RED_SIGN;
			if (res < ELITE_MONSTER_PROBABILITY) { tlevel = LEVEL1; }	// 生成level1的魔物
			else { tlevel = LEVEL0; }								// 默认生成level0的魔物

		}
		else {
			tcolor = 0;
			tlevel = UN_LEVEL;
		}
		monsters[0][j].setMonster(tlevel, tcolor);
	}
}

// 显示地图上的所有魔物
void MonsterController::showMonsters() {
	for (int i = 0; i < height - 1; i++) {
		for (int j = 0; j < width; j++) {
			cout << setw(6) << left << monsters[i][j].color;
			if (j == width - 1) cout << endl;
		}
	}
	cout << "------------" << endl;
}

// 魔物整体下移一次，下移成功则返回TRUE
bool MonsterController::update() {
	if (moveMonsters()) {
		cout << "game over!" << endl;
		return false;
	}
	randomCreat();
	return true;
}

// 输入坐标(height, width)，判断是否在魔物大军范围内
bool MonsterController::inMonsters(int h, int w) {
	if (h >= 0 && h < height && w >= 0 && w < width) return true;
	return false;
}

// choose_stone_p.x为颜色0~4，choose_stone_p.y为等级0~2，mclick_p为点击的相对坐标(h,w,1)，从(0,0)开始
queue<Monster>* MonsterController::boom(Point choose_stone_p, Point mclick_p) {
	int w = mclick_p.y, h = mclick_p.x;
	int tw, th, tcolor = choose_stone_p.x + RED_SIGN;
	int n, i, j;
	int to_boom_n = 0;
	//queue<Monster> to_boom;	// 存放魔物区中所有被递归点燃的魔物坐标(height, width, level)
	//queue<Monster> to_render;	// 存放魔物区中所有被递归点燃的魔物坐标，供render.cpp去渲染图像

	Monster tp;
	to_render.push(Monster(mclick_p.x, mclick_p.y, choose_stone_p.y, choose_stone_p.x));	// 初始将魔石当作魔物压入待渲染队列
	switch (choose_stone_p.y) {	// 根据待部署的魔石的等级判断其初始爆炸范围
		case 0:
			if (monsters[h][w].color == tcolor) {
				to_boom.push(Monster(h, w, monsters[h][w].level, tcolor));
				to_render.push(Monster(h, w, monsters[h][w].level, tcolor));
			}
			killMonster(h, w);
			break;
		case 1:
			n = 5;
			for (i = 0; i < n; i++) {
				th = h + stone_boom_area_1[i].x;
				tw = w + stone_boom_area_1[i].y;
				if (inMonsters(th, tw)) {
					if (monsters[th][tw].color == tcolor) {
						to_boom.push(Monster(th, tw, monsters[th][tw].level, tcolor));
						to_render.push(Monster(th, tw, monsters[th][tw].level, tcolor));
					}
					killMonster(th, tw);
				}
			}
			break;
		case 2:
			n = 13;
			for (i = 0; i < n; i++) {
				th = h + stone_boom_area_2[i].x;
				tw = w + stone_boom_area_2[i].y;
				if (inMonsters(th, tw)) {
					if (monsters[th][tw].color == tcolor) {
						to_boom.push(Monster(th, tw, monsters[th][tw].level, tcolor));
						to_render.push(Monster(th, tw, monsters[th][tw].level, tcolor));
					}
					killMonster(th, tw);
				}
			}
			break;
		default: break;
	}
	// 此时to_boom队列中存储了to_boom.size()个与待部署魔石同色的魔物坐标

	while (!to_boom.empty()) {	// 依次读取整个队列，对每个魔物再次查看其爆炸范围内是否有其他同色魔物，也压入队列
		tp = to_boom.front();	// tp.x为h, tp.y为w, tp.level为0~2，tp.color为0~4 + RED_SIGN
		to_boom.pop();
		h = tp.x;
		w = tp.y;
		cout << "to boom ( height: " << h << " , width: " << w << " )" << endl;
		switch (tp.level) {
			case 0:
				n = 4;
				for (i = 0; i < n; i++) {
					th = h + monster_boom_area_0[i].x;
					tw = w + monster_boom_area_0[i].y;
					if (inMonsters(th, tw)) {
						if (monsters[th][tw].color == tcolor) {
							to_boom.push(Monster(th, tw, monsters[th][tw].level, tcolor));
							to_render.push(Monster(th, tw, monsters[th][tw].level, tcolor));
						}
						killMonster(th, tw);
					}
				}
				break;
			case 1:
				n = 12;
				for (i = 0; i < n; i++) {
					th = h + monster_boom_area_1[i].x;
					tw = w + monster_boom_area_1[i].y;
					if (inMonsters(th, tw)) {
						if (monsters[th][tw].color == tcolor) {
							to_boom.push(Monster(th, tw, monsters[th][tw].level, tcolor));
							to_render.push(Monster(th, tw, monsters[th][tw].level, tcolor));
						}
						killMonster(th, tw);
					}
				}
				break;
			case 2:
			default: break;
		}
	}
	//此时to_boom队列为空，to_render队列存储了所有发生了爆炸的魔物（height, width, level, color）
	return &to_render;
}


void testfoo() {
	MonsterController mcontroller;
	mcontroller.monsters[0][0].setMonster(2, 101);
	Monster m;
	m = mcontroller.monsters[0][0];
	cout << m.level << " , " << m.color << endl;
	cout << "测试Monster.h" << endl;
}

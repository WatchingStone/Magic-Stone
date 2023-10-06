#include "Monster.h"

int score = 0;

// ������queue���붨���ں������ⲿ������Ϊȫ�ֱ���
queue<Monster> to_boom;	// ���ħ���������б��ݹ��ȼ��ħ������(height, width, level)
queue<Monster> to_render;	// ���ħ���������б��ݹ��ȼ��ħ�����꣬��render.cppȥ��Ⱦͼ��

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

// ������ħ�������������һ�С��������ڶ��У���ʾ�ڻ����еĵ�����һ�У�����ħ��δ�����򷵻�TRUE
bool MonsterController::moveMonsters() {
	int i, j;

	for (j = 0; j < width; j++) {
		if (monsters[height - 2][j].color > 0)  return  true;	// �������ڶ���������ħ��򷵻�TRUE
	}
	for (i = height - 1; i >= 1; i--) {
		for (j = 0; j < width; j++) {
			monsters[i][j] = monsters[i - 1][j];			// �����������¸���ħ�����
		}
	}
	return false;
}

// ��ħʯ��ը������ɱ��ħ�﷽��
void MonsterController::killMonster(int h, int w) {
	if (monsters[h][w].color > 0) {
		monsters[h][w].color = 0;
		monsters[h][w].level = 0;
		score++;					// ÿ����1��ħ��+1��
	}
}

// ��û�д��ļ��ж�ȡ�̶���ħ������Ϣ����������ɡ���������.moveMonsters()����֮����ã�
void MonsterController::randomCreat() {
	int j;
	int randseed, res;
	int tcolor, tlevel;

	// �ڵ�һ�����������ħ�ֻ����һ�����ֵ������������
	randseed = e();
	for (j = 0; j < width; j++) {
		res = ((unsigned)(randseed / primelist[j])) % 100;
		if (res < MONSTER_PROBABILITY) {
			tcolor = res % 5 + RED_SIGN;
			if (res < ELITE_MONSTER_PROBABILITY) { tlevel = LEVEL1; }	// ����level1��ħ��
			else { tlevel = LEVEL0; }								// Ĭ������level0��ħ��

		}
		else {
			tcolor = 0;
			tlevel = UN_LEVEL;
		}
		monsters[0][j].setMonster(tlevel, tcolor);
	}
}

// ��ʾ��ͼ�ϵ�����ħ��
void MonsterController::showMonsters() {
	for (int i = 0; i < height - 1; i++) {
		for (int j = 0; j < width; j++) {
			cout << setw(6) << left << monsters[i][j].color;
			if (j == width - 1) cout << endl;
		}
	}
	cout << "------------" << endl;
}

// ħ����������һ�Σ����Ƴɹ��򷵻�TRUE
bool MonsterController::update() {
	if (moveMonsters()) {
		cout << "game over!" << endl;
		return false;
	}
	randomCreat();
	return true;
}

// ��������(height, width)���ж��Ƿ���ħ������Χ��
bool MonsterController::inMonsters(int h, int w) {
	if (h >= 0 && h < height && w >= 0 && w < width) return true;
	return false;
}

// choose_stone_p.xΪ��ɫ0~4��choose_stone_p.yΪ�ȼ�0~2��mclick_pΪ������������(h,w,1)����(0,0)��ʼ
queue<Monster>* MonsterController::boom(Point choose_stone_p, Point mclick_p) {
	int w = mclick_p.y, h = mclick_p.x;
	int tw, th, tcolor = choose_stone_p.x + RED_SIGN;
	int n, i, j;
	int to_boom_n = 0;
	//queue<Monster> to_boom;	// ���ħ���������б��ݹ��ȼ��ħ������(height, width, level)
	//queue<Monster> to_render;	// ���ħ���������б��ݹ��ȼ��ħ�����꣬��render.cppȥ��Ⱦͼ��

	Monster tp;
	to_render.push(Monster(mclick_p.x, mclick_p.y, choose_stone_p.y, choose_stone_p.x));	// ��ʼ��ħʯ����ħ��ѹ�����Ⱦ����
	switch (choose_stone_p.y) {	// ���ݴ������ħʯ�ĵȼ��ж����ʼ��ը��Χ
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
	// ��ʱto_boom�����д洢��to_boom.size()���������ħʯͬɫ��ħ������

	while (!to_boom.empty()) {	// ���ζ�ȡ�������У���ÿ��ħ���ٴβ鿴�䱬ը��Χ���Ƿ�������ͬɫħ�Ҳѹ�����
		tp = to_boom.front();	// tp.xΪh, tp.yΪw, tp.levelΪ0~2��tp.colorΪ0~4 + RED_SIGN
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
	//��ʱto_boom����Ϊ�գ�to_render���д洢�����з����˱�ը��ħ�height, width, level, color��
	return &to_render;
}


void testfoo() {
	MonsterController mcontroller;
	mcontroller.monsters[0][0].setMonster(2, 101);
	Monster m;
	m = mcontroller.monsters[0][0];
	cout << m.level << " , " << m.color << endl;
	cout << "����Monster.h" << endl;
}

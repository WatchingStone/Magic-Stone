// ��ҪΪ��ħ����������ඨ��
// ����:��ħ�����ͼ��MFloor, ħ��֮ԴMSource, ħ����MCube

#include <string>
#include <iostream>
#include "MagicCube.h"
#include "Map.h"

using namespace std;

BaseMCube::BaseMCube():cube_area_n(5), exportp(0, 0) {
	int n = 5;
	category = BLANK;
	orient = 0;									// ��ʾ��ʼ����
	cube_area = new Point[n * n];
	int i;
	for (i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cube_area[i * n + j].setx(i);
			cube_area[i * n + j].sety(j);
		}
	}
	cube_area[0 * n + 0].setv(-1);			// ħ��������λ������ֵΪ-1
	for (i = 0; i < 5; i++) {				// ��ʼ��ħ�����ڰ�����ħ��֮Դ����
		msrcs[i].setP(0, 0, 0);
	}
}
BaseMCube::BaseMCube(int n, int ex, int ey): cube_area_n(n), exportp(ex, ey){
	category = BLANK;
	orient = 0;									// ��ʾ��ʼ����
	cube_area = new Point[n * n];
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cube_area[i * n + j].setx(i);
			cube_area[i * n + j].sety(j);
		}
	}
	cube_area[ex * n + ey].setv(-1);			// ħ��������λ������ֵΪ-1
}
void BaseMCube::setStoneList(Virtual* stl) {
	stonelist = stl;
}
void BaseMCube::setN(int n) {
	cube_area_n = n;
}
void BaseMCube::turnRound() {									// ��ħ����˳ʱ����ת
	int temp;
	for (int i = 0; i < cube_area_n * cube_area_n; i++) {
			temp = cube_area[i].x;
			cube_area[i].setx(cube_area[i].y);
			cube_area[i].sety(cube_area_n - 1 - temp);
	}
	temp = exportp.x;
	exportp.setx(exportp.y);
	exportp.sety(cube_area_n - 1 - temp);
	exportp.setv(-1);
	orient = (orient + 1) % 4;									// ��ת��ı䵱ǰħ����ķ���
}
void BaseMCube::showCube() {
	int *cube = new int[cube_area_n * cube_area_n];
	int i = 0, j = 0;
	int tx, ty, tv;

	for (i = 0; i < cube_area_n * cube_area_n; i++) {
			tx = cube_area[i].x;
			ty = cube_area[i].y;
			tv = cube_area[i].v;
			cube[tx * cube_area_n + ty] = tv;
	}
	for (i = 0; i < cube_area_n; i++) {
		for (j = 0; j < cube_area_n; j++) {
			cout << cube[i * cube_area_n + j] << "  ";
			if (j == cube_area_n - 1) {
				cout << endl;
			}
		}
	}
	cout << "orient is:  " << orient << endl;
}
void BaseMCube::setShift(Point p) {
	shift.x = p.x;
	shift.y = p.y;
}

// ����pos��ר����cubecontroller���£�����cube�����shift����
int BaseMCube::setInMap(Map& mymap) {
	int lux = shift.x, luy = shift.y;					// left_up�㣬��pos�õ���ħ�������Ͻǵľ�������
	int n = cube_area_n;
	int tempx, tempy;
	int i, v;
	// ����ֻ����cubecontrollerʹ�ã���������ٴ��ж�ħ�����Ƿ���Է��ã���ʼ��ħ����д���ͼ
	for (i = 0; i < n * n; i++) {
		v = cube_area[i].v;
		if (v != 0 && v != -1) {
			tempx = cube_area[i].x + lux;
			tempy = cube_area[i].y + luy;
			mymap.map[tempx * mymap.width + tempy].setInCube(category, v, orient);
		}
	}
	return OK;
}
// ������ħ����ǰ��Ҫ���Ŀ������ĵ�ͼ���Ƿ��Ѵ�������ħ����EXIST_CUBE)���Ƿ񳬳���ͼ�߽磨����OUTBOUND��
// posΪPoint��ָ��ħ���������λ��
int BaseMCube::setInMap(Map &mymap, Point pos) {
	int lux = pos.x, luy = pos.y;					// left_up�㣬��pos�õ���ħ�������Ͻǵľ�������
	int n = cube_area_n;
	switch (n) {
		case 3:
		case 4:
			lux--; luy--; break;
		case 5:
			lux -= 2; luy -= 2; break;
		default: cout << "Cube Error!" << endl;
	}
	shift.setP(lux, luy);
	int tempx, tempy;
	int i, v;
	for (i = 0; i < n * n; i++) {					// ���ħ����ʵ���Ƿ�Խ��
		v = cube_area[i].v;
		if (v != 0 && v != -1) {
			tempx = cube_area[i].x + lux;			// ��left_up��Ϊ��������ħ����������������ɸ���ľ�������
			tempy = cube_area[i].y + luy;
			if (tempx < 0 || tempx > mymap.width - 1 || tempy < 0 || tempy >mymap.height - 1) {	// ����Խ��
				return OUTBOUNDS;
			}
			if (mymap.map[tempx * mymap.width + tempy].cube_category) {		// ���ħ���������������Ƿ��Ѿ�������ħ���󸲸�
				return EXIST_CUBE;
			}
		}
	}
	// �������˵����ǰ����λ�ö���д����ʼ��ħ����д���ͼ
	for (i = 0; i < n * n; i++) {				
		v = cube_area[i].v;
		if (v != 0 && v != -1) {
			tempx = cube_area[i].x + lux;			
			tempy = cube_area[i].y + luy;
			mymap.map[tempx * mymap.width + tempy].setInCube(category, v, orient);
		}
	}
	return OK;
}
// ħ����������������ħ��֮Դ�����ʱ�ӣ���洢ħʯ״����int�����������
int BaseMCube::produceStone(Map &mymap) {
	Point res = checkSource(mymap);			// �ȳ�ʼ������ʱ��
	//cout << "��ħ�������ħʯ���ࣺ" << res.x << ", ħʯ�ȼ���" << res.y << endl;
	if (res.x > 0 && res.y > 0) {
		clock_t nowclocks = clock();
		if ((nowclocks - clocks) >= PRODUCE_TIME) {	// �ٻ�ȡ��ǰʱ�ӣ�ÿ2�����һ��
			stonelist->addStone(res);
			clocks = nowclocks;
			return OK;
		}		
	}
	return 0;
}
// ���ħ���������˿��Ƿ���࣬�����µ�ħ��֮Դ
int BaseMCube::setSourceInMap(Map &mymap) {
	if (checkSource(mymap).x > 0) {
		// cout << "check has source" << endl;
		int i = 0;
		int tx, ty, dessrc = 0;
		int w, h;

		// ���ħ��������˿ڴ����Ƿ��ڵ�ͼ��
		tx = exportp.x + shift.x;
		ty = exportp.y + shift.y;
		if (tx < 0 || tx > mymap.width - 1 || ty < 0 || ty >mymap.height - 1) return OUTBOUNDS;
		// ���ħ��������˿ڴ�����ͼ�Ƿ����
		if (mymap.map[tx * mymap.width + ty].msource == 0) {
			// ���ħ�������Ƿ�����ڴ�ɫħ��֮Դ
			while (msrcs[i].v > 0) {
				if (i == 0) {
					dessrc = msrcs[i].v;			// ��ʱ�洢ǰһ��ħ��֮Դ�����࣬��ʱ�����������ħ��֮Դ������Ϊ������
				}
				else {
					if (msrcs[i].v != dessrc) {	// �������ħ��֮Դ�����෢���ı䣬�������ħ��֮Դ
						dessrc = 0;
						break;
					}
				}
				i++;
				if (i >= 5) break;
			}
			// ��ʱdessrc�洢�˽�Ҫ�����ħ��֮Դ���࣬0��Ϊ�����

			if (dessrc != 0) {
				mymap.map[tx * mymap.width + ty].setSource(dessrc);
				return OK;
			}
		}
		else return ERROR;
	}
}
// ����ħ�����а�����ħ��֮Դ��������������������ħʯ����Point.x�͵ȼ�Point.y
Point  BaseMCube::checkSource(Map mymap) {
	int tx, ty, tv;
	int i, j, v, k = 0;
	Point cubep, mapp;
	for (i = 0; i < cube_area_n * cube_area_n; i++) {
		v = cube_area[i].v;
		if (v > 0) {
			tx = cube_area[i].x + shift.x;
			ty = cube_area[i].y + shift.y;
			tv = mymap.map[tx * mymap.width + ty].msource;
			if (tv > 0) {
				msrcs[k].setP(tx, ty, tv);
				k++;
			}
			if (k >= 5) break;
		}
	}
	i = 0;
	int dessrc = 0, count = 0;
	int cats[5] = {0};		// �洢ħ������ħʯ���м���
	int flag_cats[5] = {0};	// �洢ħ�����ﹲ�м���ħʯ
	int cats_count;
	while (msrcs[i].v > 0) {
		v = msrcs[i].v;
		//if (v == 0) { break; }
		cats[v - RED_SIGN] += 1;
		flag_cats[v - RED_SIGN] = 1;
		i++;
		if (i >= 5) break;
	}
	cats_count = flag_cats[0] + flag_cats[1] + flag_cats[2] + flag_cats[3] + flag_cats[4];
	// ��ʱdessrc�洢�˽�Ҫ�����ħ��֮Դ���࣬0��Ϊ�����
	if (cats_count == 1) {			// ��ʱ�ں���ɫħ��֮Դ
		for (i = 0; i < 5; i++) {
			if (flag_cats[i] == 1) {
				dessrc = i + RED_SIGN;
				count = cats[i];
				break;
			}
		}
	}
	else if (cats_count > 1) {
		dessrc = WHITE_SIGN;
		switch (cats_count) {
			case 2: count = 1; break;
			case 3: count = 2; break;
			case 4: count = 3; break;
			case 5: count = 3; break;
			default: break;
		}
	}
	if (cats_count == 1 && clocks == 0) {
		clocks = clock();			// ����ں���ɫħ��֮Դ������ζ�ſ�����������Գ�ʼ��ħ����ʱ��
	}

	Point res(dessrc, count);
	return res;
}

Cube_V1::Cube_V1() :BaseMCube(3, 1, 2) {
	category = CUBE_V1;
	setN(3);
	int n = 3;
	cube_area[0 * n + 0].setv(1);
	cube_area[1 * n + 0].setv(2);
	cube_area[1 * n + 1].setv(3);
}

Cube_V2::Cube_V2() :BaseMCube(3, 1, 0) {
	category = CUBE_V2;
	setN(3);
	int n = 3;
	cube_area[0 * n + 2].setv(1);
	cube_area[1 * n + 1].setv(2);
	cube_area[1 * n + 2].setv(3);
}

Cube_T::Cube_T() :BaseMCube(3, 2, 1) {
	category = CUBE_T;
	setN(3);
	int n = 3;
	cube_area[0 * n + 0].setv(1);
	cube_area[0 * n + 1].setv(2);
	cube_area[0 * n + 2].setv(3);
	cube_area[1 * n + 1].setv(4);
}

Cube_B::Cube_B() :BaseMCube(3, 1, 2) {
	category = CUBE_B;
	setN(3);
	int n = 3;
	cube_area[0 * n + 0].setv(1);
	cube_area[0 * n + 1].setv(2);
	cube_area[1 * n + 0].setv(3);
	cube_area[1 * n + 1].setv(4);
}

Cube_C::Cube_C() :BaseMCube(3, 2, 2) {
	category = CUBE_C;
	setN(3);
	int n = 3;
	cube_area[0 * n + 1].setv(1);
	cube_area[1 * n + 0].setv(2);
	cube_area[1 * n + 1].setv(3);
	cube_area[1 * n + 2].setv(4);
	cube_area[2 * n + 1].setv(5);
}

Cube_L::Cube_L() :BaseMCube(5,1,4) {
	category = CUBE_L;
	setN(5);
	int n = 5;
	cube_area[2 * n + 0].setv(1);
	cube_area[2 * n + 1].setv(2);
	cube_area[2 * n + 2].setv(3);
	cube_area[2 * n + 3].setv(4);
	cube_area[2 * n + 4].setv(5);
}

// ���ظ�ֵ�����
void BaseMCube::operator = (const BaseMCube& b) {
	category = b.category;
	orient = b.orient;
	cube_area_n = b.cube_area_n;
	cube_area = b.cube_area;
	exportp.setP(b.exportp);
	shift.setP(b.shift);
}
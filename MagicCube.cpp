// 主要为导魔区域所需的类定义
// 包含:导魔区域地图块MFloor, 魔力之源MSource, 魔法阵MCube

#include <string>
#include <iostream>
#include "MagicCube.h"
#include "Map.h"

using namespace std;

BaseMCube::BaseMCube():cube_area_n(5), exportp(0, 0) {
	int n = 5;
	category = BLANK;
	orient = 0;									// 表示初始方向
	cube_area = new Point[n * n];
	int i;
	for (i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cube_area[i * n + j].setx(i);
			cube_area[i * n + j].sety(j);
		}
	}
	cube_area[0 * n + 0].setv(-1);			// 魔法阵的输出位置设置值为-1
	for (i = 0; i < 5; i++) {				// 初始化魔法阵内包含的魔力之源数组
		msrcs[i].setP(0, 0, 0);
	}
}
BaseMCube::BaseMCube(int n, int ex, int ey): cube_area_n(n), exportp(ex, ey){
	category = BLANK;
	orient = 0;									// 表示初始方向
	cube_area = new Point[n * n];
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cube_area[i * n + j].setx(i);
			cube_area[i * n + j].sety(j);
		}
	}
	cube_area[ex * n + ey].setv(-1);			// 魔法阵的输出位置设置值为-1
}
void BaseMCube::setStoneList(Virtual* stl) {
	stonelist = stl;
}
void BaseMCube::setN(int n) {
	cube_area_n = n;
}
void BaseMCube::turnRound() {									// 将魔法阵按顺时针旋转
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
	orient = (orient + 1) % 4;									// 旋转后改变当前魔法阵的方向
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

// 无须pos，专用于cubecontroller更新，利用cube自身的shift属性
int BaseMCube::setInMap(Map& mymap) {
	int lux = shift.x, luy = shift.y;					// left_up点，由pos得到的魔法阵左上角的绝对坐标
	int n = cube_area_n;
	int tempx, tempy;
	int i, v;
	// 由于只允许cubecontroller使用，因此无须再次判定魔法阵是否可以放置，开始将魔法阵写入地图
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
// 在设置魔法阵前需要检查目标区域的地图上是否已存在其他魔法阵（EXIST_CUBE)，是否超出地图边界（返回OUTBOUND）
// pos为Point，指代魔法阵的中心位置
int BaseMCube::setInMap(Map &mymap, Point pos) {
	int lux = pos.x, luy = pos.y;					// left_up点，由pos得到的魔法阵左上角的绝对坐标
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
	for (i = 0; i < n * n; i++) {					// 检查魔法阵实体是否越界
		v = cube_area[i].v;
		if (v != 0 && v != -1) {
			tempx = cube_area[i].x + lux;			// 以left_up点为基础，由魔法阵内相对坐标生成各点的绝对坐标
			tempy = cube_area[i].y + luy;
			if (tempx < 0 || tempx > mymap.width - 1 || tempy < 0 || tempy >mymap.height - 1) {	// 出现越界
				return OUTBOUNDS;
			}
			if (mymap.map[tempx * mymap.width + tempy].cube_category) {		// 检测魔法阵欲覆盖区域是否已经被其他魔法阵覆盖
				return EXIST_CUBE;
			}
		}
	}
	// 遍历完成说明当前所有位置都可写，开始将魔法阵写入地图
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
// 魔法阵根据自身包含的魔力之源情况和时钟，向存储魔石状况的int数组添加新项
int BaseMCube::produceStone(Map &mymap) {
	Point res = checkSource(mymap);			// 先初始化自身时钟
	//cout << "本魔法阵将输出魔石种类：" << res.x << ", 魔石等级：" << res.y << endl;
	if (res.x > 0 && res.y > 0) {
		clock_t nowclocks = clock();
		if ((nowclocks - clocks) >= PRODUCE_TIME) {	// 再获取当前时钟，每2秒更新一次
			stonelist->addStone(res);
			clocks = nowclocks;
			return OK;
		}		
	}
	return 0;
}
// 检测魔法阵的输出端口是否空余，设置新的魔力之源
int BaseMCube::setSourceInMap(Map &mymap) {
	if (checkSource(mymap).x > 0) {
		// cout << "check has source" << endl;
		int i = 0;
		int tx, ty, dessrc = 0;
		int w, h;

		// 检测魔法阵输出端口处，是否在地图内
		tx = exportp.x + shift.x;
		ty = exportp.y + shift.y;
		if (tx < 0 || tx > mymap.width - 1 || ty < 0 || ty >mymap.height - 1) return OUTBOUNDS;
		// 检测魔法阵输出端口处，地图是否空闲
		if (mymap.map[tx * mymap.width + ty].msource == 0) {
			// 检测魔法阵内是否仅存在纯色魔力之源
			while (msrcs[i].v > 0) {
				if (i == 0) {
					dessrc = msrcs[i].v;			// 临时存储前一个魔力之源的种类，暂时将最终输出的魔力之源种类设为此种类
				}
				else {
					if (msrcs[i].v != dessrc) {	// 如果后续魔力之源的种类发生改变，则不输出新魔力之源
						dessrc = 0;
						break;
					}
				}
				i++;
				if (i >= 5) break;
			}
			// 此时dessrc存储了将要输出的魔力之源种类，0则为不输出

			if (dessrc != 0) {
				mymap.map[tx * mymap.width + ty].setSource(dessrc);
				return OK;
			}
		}
		else return ERROR;
	}
}
// 更新魔法阵中包含的魔力之源情况，并输出可以生产的魔石种类Point.x和等级Point.y
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
	int cats[5] = {0};		// 存储魔法阵里魔石各有几个
	int flag_cats[5] = {0};	// 存储魔法阵里共有几种魔石
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
	// 此时dessrc存储了将要输出的魔力之源种类，0则为不输出
	if (cats_count == 1) {			// 此时内含纯色魔力之源
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
		clocks = clock();			// 如果内含纯色魔力之源，则意味着可以输出，所以初始化魔法阵时钟
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

// 重载赋值运算符
void BaseMCube::operator = (const BaseMCube& b) {
	category = b.category;
	orient = b.orient;
	cube_area_n = b.cube_area_n;
	cube_area = b.cube_area;
	exportp.setP(b.exportp);
	shift.setP(b.shift);
}
#pragma once// 图像渲染部分
#pragma comment( lib, "MSIMG32.LIB")
#include <easyx.h>
#include <iostream>
#include <string>
#include <Windows.h>
#include <queue>
#include "Map.h"
#include "MagicCube.h"
#include "Controller.h"
#include "Monster.h"
#include "head.h"

using namespace std;

/* 定义游戏初始菜单界面的两个按钮RECT范围*/
extern RECT start_button_rec;
extern RECT quit_button_rec;

/* 定义5个主要区域的RECT范围*/
extern RECT store_rec;
extern RECT monster_rec;
extern RECT map_rec;
extern RECT storage_rec;
extern RECT money_rec;

Point transtoMapxy(Map mymap, Point click);
Point transtoStoragexy(Point mclick);
Point transtoMonsterxy(Point mclick);
void transparentimage3(IMAGE* dstimg, int x, int y, IMAGE* srcimg);
void clearrectangle(RECT area);
void FlushBatchDraw(RECT area);
void EndBatchDraw(RECT area);
void showFPS(clock_t stime);

class ImageRender {
public:
	IMAGE* imgsource;
	IMAGE* is_store;	// 商店区所需的图片
	IMAGE* is_storage;	// 魔石存储区所需的图片
	bool flag_putstorage = 0;	// 用在存储区的标志位，初始置1代表需要输出魔石图像，后续无需输出魔石图像，只需要修改魔石显示数量
	IMAGE* is_monster;	// 魔物区所需的图片
	IMAGE* is_money;	// 金钱区所需的图片
	IMAGE* is_cursor;	// 跟随鼠标的光标图片
	IMAGE* is_boom;		// 魔石和魔物爆炸的帧图片

	ImageRender() { imgsource = NULL; is_store = NULL; }
	IMAGE* loadimg();		// 加载所有区域的所有图片，包含下列的所有loadimg_x函数，默认返回地图区域图片
	IMAGE* loadimg_store();
	IMAGE* loadimg_storage();
	IMAGE* loadimg_monster();
	IMAGE* loadimg_money();
	IMAGE* loadimg_cursor();
	IMAGE* loadimg_boom();
	void putBackground();
	void putAllImage();
	void putMap(Map mymap);							// 显示地图区
	void putStore(int nmoney, int highlight);		// 显示商店区
	void putStorage(StoneController* scontroller);	// 显示魔石存储区
	void putMonster(MonsterController mcontroller);	// 显示魔物区
	void putMoney(int nmoney);						// 显示金钱区
	void putCursorCube(int cube_cat, int cube_ori);	// 借用金钱区显示待部署魔法阵
	void putCursorStone(Point choose_stone_p);		// 借用金钱区显示待部署魔石
	void putBoom(queue<Monster>* to_render_queue);	// 输出魔石与魔物的爆炸特效

	void iniStartMenu();	// 输出游戏初始界面菜单
	void gameover();		// 输出游戏结束图像
	void clearCursor();		// 及时刷新待部署区
};

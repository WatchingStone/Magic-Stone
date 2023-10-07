#pragma once
#include <stdio.h>
#include <iostream>
#include <Windows.h>
#include <ctime>
#include <queue>
#include "Map.h"
#include "MagicCube.h"
#include "Controller.h"
#include "Monster.h"
#include "render.h"
#include "head.h"

using namespace std;

int main() {
	//testfoo();
	
	initgraph(SCREEN_WIDTH, SCREEN_HEIGHT, EX_SHOWCONSOLE);
	clock_t start_time, temp_time, last_time = 0, monster_last_time = 0;	// last_time用来约束nmoney等事件更新频率为每UPDATE_TIME毫秒更新1次，代表上一次更新的时间
	int nmoney = 40;
	ExMessage msg;
	Point mclick, temp_mclick;	// 记录鼠标点击的坐标
	Point mapxy;				// 鼠标点击坐标在地图区域中的相对位置
	int click_categoray = 0;	// 记录选中的待部署魔法阵种类
	BaseMCube* mouse_cube = NULL;
	int click_price = 0;		// 记录选中的待部署魔法阵价格
	int highlight_store = 0;

	bool flag_putmap = 1;		// 只有新部署了魔法阵才输出地图，初始置1，使其输出第一次地图
	bool flag_putstore = 1;		// 只有nmoney更新时才输出地图
	bool flag_boom = 0;			// 如果部署了魔石，则刷新整个屏幕，因为爆炸范围可能会溢出魔物区
	bool flag_start = 0;		// 当点击初始菜单的start按钮后才会开始游戏
	Point choose_stone_p(0, 0, 0);	// 只有选中了魔石，v才会置1
	queue<Monster>* to_render_queue;
	
	// 初始化地图
	char path[20] = "./map/map1.txt";
	Map mymap;
	mymap = mymap.initMap(path);
	// 初始化魔石控制器，魔法阵控制器
	StoneController scontroller;
	CubeController ccontroller;
	ccontroller.initStoneList(&scontroller);
	// 初始化魔物控制器
	MonsterController mcontroller = MonsterController();

	// 初始化图像渲染器
	ImageRender imgrender;
	imgrender.loadimg();

	// 进入游戏初始菜单界面，选择start或者quit
	imgrender.iniStartMenu();
	while (1) {
		if (peekmessage(&msg, EX_MOUSE)) {
			if (msg.message == WM_LBUTTONDOWN) {
				mclick.setP(msg.x, msg.y);
				cout << "点击了屏幕（ " << mclick.x << " , " << mclick.y << " )" << endl;
				if (mclick.inRec(start_button_rec)) {
					flag_start = 1;						// 点击初始菜单的start按钮以开始游戏
					break;
				}
				if (mclick.inRec(quit_button_rec)) {
					flag_start = 0;
					break;
				}
			}
		}
	}

	// 开始游戏
	if (flag_start) {
		while (flag_start) {
			// 初始化各个参数
			imgrender.putBackground();
			start_time = 0, temp_time = 0, last_time = 0, monster_last_time = 0;
			nmoney = 40;
			score = 0;
			flushmessage(-1);
			mclick.setP(), temp_mclick.setP(), mapxy.setP();
			click_categoray = 0;	
			mouse_cube = NULL;
			click_price = 0;		
			highlight_store = 0;

			flag_putmap = 1;		
			flag_putstore = 1;		
			flag_boom = 0;			
			choose_stone_p.setP();
			to_render_queue = NULL;;

			// 初始化地图
			mymap = mymap.initMap(path);
			// 初始化魔石控制器，魔法阵控制器
			scontroller.init();
			ccontroller.init();
			ccontroller.initStoneList(&scontroller);
			// 初始化魔物控制器
			mcontroller.init();
			// 初始化图像渲染器
			imgrender.init();
			
			// 进入游戏
			while (flag_start) {
				start_time = clock();	// 用来计算游戏帧数
				mymap.reloadmap();		// 每一次先用ori_map重置map，后续运算得到的mcube和msource用另外的形式存储
				ccontroller.update(mymap);

				// 检测鼠标动作
				if (peekmessage(&msg, EX_MOUSE)) {	// 检测鼠标点击
					mclick.setP(msg.x, msg.y);
					switch (msg.message) {
						case WM_LBUTTONDOWN:
							// 如果点击了商店区
							if (mclick.inRec(store_rec)) {
								if (mclick.y <= SCREEN_SQUARE_SIZE_HALF && nmoney >= V1_PRICE) {
									click_categoray = CUBE_V1;
									click_price = V1_PRICE;
									mouse_cube = new Cube_V1;			// 初始化1个对应类型的魔法阵
									highlight_store = 1;				// 设置标志代表需要点亮该种魔法阵
								}
								else if (mclick.y <= SCREEN_SQUARE_SIZE_HALF * 2 && nmoney >= V2_PRICE) {
									click_categoray = CUBE_V2;
									click_price = V2_PRICE;
									mouse_cube = new Cube_V2;
									highlight_store = 2;
								}
								else if (mclick.y <= SCREEN_SQUARE_SIZE_HALF * 3 && nmoney >= T_PRICE) {
									click_categoray = CUBE_T;
									click_price = T_PRICE;
									mouse_cube = new Cube_T;
									highlight_store = 3;
								}
								else if (mclick.y <= SCREEN_SQUARE_SIZE_HALF * 4 && nmoney >= B_PRICE) {
									click_categoray = CUBE_B;
									click_price = B_PRICE;
									mouse_cube = new Cube_B;
									highlight_store = 4;
								}
								else if (mclick.y <= SCREEN_SQUARE_SIZE_HALF * 5 && nmoney >= C_PRICE) {
									click_categoray = CUBE_C;
									click_price = C_PRICE;
									mouse_cube = new Cube_C;
									highlight_store = 5;
								}
								else if (mclick.y <= SCREEN_SQUARE_SIZE_HALF * 6 && nmoney >= L_PRICE) {
									click_categoray = CUBE_L;
									click_price = L_PRICE;
									mouse_cube = new Cube_L;
									highlight_store = 6;
								}

								if (click_categoray > 0) { flag_putstore = TRUE; }
								if (choose_stone_p.v) { choose_stone_p.setP(0, 0, 0); }
								else break;
								// 此时缓存区里已经有待部署的魔法阵了
							}

							// 如果点击了魔物区
							if (mclick.inRec(monster_rec)) {
								if (choose_stone_p.v) {	// 如果已经选中待部署魔石
									temp_mclick = transtoMonsterxy(mclick);
									to_render_queue = mcontroller.boom(choose_stone_p, temp_mclick);
									scontroller.subStone(choose_stone_p);
									imgrender.putStorage(&scontroller);
									choose_stone_p.setP(0, 0, 0);
									temp_mclick.setP(0, 0, 0);
									imgrender.putBoom(to_render_queue);
									flag_boom = 1;
								}
								break;
							}

							// 如果点击了地图区
							if (mclick.inRec(map_rec)) {
								if (mouse_cube != NULL) {	// 如果有待部署的魔法阵，则检测其是否可以安置在地图上
									mapxy = transtoMapxy(mymap, mclick);
									int res;
									res = mouse_cube->setInMap(mymap, mapxy);
									if (res == OK) {
										ccontroller.addCube(*mouse_cube);	// 将新魔法阵压入法阵控制器
										nmoney -= click_price;
										click_categoray = 0;				// 清除缓存
										mouse_cube = NULL;
										click_price = 0;
										highlight_store = 0;
										imgrender.clearCursor();

										flag_putmap = TRUE;
										flag_putstore = TRUE;
									}
								}
								break;
							}

							// 如果点击了魔石存储区
							if (mclick.inRec(storage_rec)) {
								if (mouse_cube != NULL) {	// 如果当前已经选中待部署魔法阵，则先清除魔法阵缓存，再进行魔石输出操作
									click_categoray = 0;
									mouse_cube = NULL;
									click_price = 0;
									highlight_store = 0;
									flag_putstore = TRUE;
								}
								choose_stone_p = transtoStoragexy(mclick);
								if (scontroller.stonelist[choose_stone_p.x][choose_stone_p.y] <= 0) {	// 如果选中的待部署魔石数量不足，则选择无效
									choose_stone_p.setP(0, 0, 0);
								}
								else imgrender.putStore(nmoney, highlight_store);
								break;
							}
							if (mclick.inRec(money_rec)) {
								break;
							}
							break;
						case WM_RBUTTONDOWN:
							if (click_categoray > 0) {		// 鼠标点击右键时，如果已经选中了某一待部署的魔法阵，则会对魔法阵进行旋转
								mouse_cube->turnRound();
							}
							break;
						default:;
					}
				}

				// 渲染屏幕上的5个区域
				if (flag_boom) {
					flag_boom = 0;
					// 刷新背景地图
					imgrender.putBackground();
					// 刷新导魔地图区
					mymap.reloadmap();
					ccontroller.update(mymap);
					imgrender.putMap(mymap);
					flag_putmap = 0;
					// 刷新商店区
					imgrender.putStore(nmoney, highlight_store);
					// 刷新金钱区与待部署区
					imgrender.clearCursor();
					imgrender.putMoney(nmoney);
					// 刷新魔石存储区
					imgrender.flag_putstorage = 0;
					imgrender.putStorage(&scontroller);
					// 刷新魔物区
					imgrender.putMonster(mcontroller);
				}
				/*if (flag_putstore) {
					imgrender.putStore(nmoney, highlight_store);
					flag_putstore = 0;
				}*/
				if (flag_putmap) {
					mymap.reloadmap();
					ccontroller.update(mymap);
					imgrender.putMap(mymap);
					flag_putmap = 0;
				}
				
				imgrender.putMoney(nmoney);

				// 渲染跟随光标的待部署魔法阵
				if (mouse_cube != NULL) {
					mclick.setP(msg.x, msg.y);
					imgrender.putMap(mymap);
					imgrender.putRealCursor(mclick, mymap, mouse_cube->category, mouse_cube->orient);
				}

				// 按UPDATE_TIME更新的事件
				temp_time = clock();
				if ((temp_time - last_time) > UPDATE_TIME) {
					nmoney++;
					last_time = temp_time;
					imgrender.putStore(nmoney, highlight_store);
					imgrender.putStorage(&scontroller);
					if (mouse_cube != NULL) {	// 如果有待部署的魔法阵，输出待部署魔法阵及其方向
						imgrender.putCursorCube(mouse_cube->category, mouse_cube->orient);
					}
					else if (choose_stone_p.v) {		// 如果有待部署的魔石，输出魔石及其等级
						imgrender.putCursorStone(choose_stone_p);
					}
					imgrender.putMonster(mcontroller);
				}
				// 魔物更新
				if ((temp_time - monster_last_time) > MONSTER_UPDATE_TIME) {
					flag_start = mcontroller.update();
					monster_last_time = temp_time;
				}
				showFPS(start_time);	// 输出帧数并控制帧数稳定
			}
			// 游戏结束判断
			cout << "sss游戏结束！" << endl;
			imgrender.gameover();
			while (!flag_start) {
				flushmessage(-1);
				if (peekmessage(&msg, EX_MOUSE)) {
					if (msg.message == WM_LBUTTONDOWN) {
						if (Point(msg.x, msg.y).inRec(restart_button_rec)) {
							flag_start = 1;
						}
						else if (Point(msg.x, msg.y).inRec(end_button_rec)) break;
					}
				}
					
			}
		}
		
	}
	
	closegraph();
	return 0;
	
}

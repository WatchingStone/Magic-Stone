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
	clock_t start_time, temp_time, last_time = 0, monster_last_time = 0;	// last_time����Լ��nmoney���¼�����Ƶ��ΪÿUPDATE_TIME�������1�Σ�������һ�θ��µ�ʱ��
	int nmoney = 40;
	ExMessage msg;
	Point mclick, temp_mclick;	// ��¼�����������
	Point mapxy;				// ����������ڵ�ͼ�����е����λ��
	int click_categoray = 0;	// ��¼ѡ�еĴ�����ħ��������
	BaseMCube* mouse_cube = NULL;
	int click_price = 0;		// ��¼ѡ�еĴ�����ħ����۸�
	int highlight_store = 0;

	bool flag_putmap = 1;		// ֻ���²�����ħ����������ͼ����ʼ��1��ʹ�������һ�ε�ͼ
	bool flag_putstore = 1;		// ֻ��nmoney����ʱ�������ͼ
	bool flag_boom = 0;			// ���������ħʯ����ˢ��������Ļ����Ϊ��ը��Χ���ܻ����ħ����
	bool flag_start = 0;		// �������ʼ�˵���start��ť��ŻῪʼ��Ϸ
	Point choose_stone_p(0, 0, 0);	// ֻ��ѡ����ħʯ��v�Ż���1
	queue<Monster>* to_render_queue;
	
	// ��ʼ����ͼ
	char path[20] = "./map/map1.txt";
	Map mymap;
	mymap = mymap.initMap(path);
	// ��ʼ��ħʯ��������ħ���������
	StoneController scontroller;
	CubeController ccontroller;
	ccontroller.initStoneList(&scontroller);
	// ��ʼ��ħ�������
	MonsterController mcontroller = MonsterController();

	// ��ʼ��ͼ����Ⱦ��
	ImageRender imgrender;
	imgrender.loadimg();

	// ������Ϸ��ʼ�˵����棬ѡ��start����quit
	imgrender.iniStartMenu();
	while (1) {
		if (peekmessage(&msg, EX_MOUSE)) {
			if (msg.message == WM_LBUTTONDOWN) {
				mclick.setP(msg.x, msg.y);
				cout << "�������Ļ�� " << mclick.x << " , " << mclick.y << " )" << endl;
				if (mclick.inRec(start_button_rec)) {
					flag_start = 1;						// �����ʼ�˵���start��ť�Կ�ʼ��Ϸ
					break;
				}
				if (mclick.inRec(quit_button_rec)) {
					flag_start = 0;
					break;
				}
			}
		}
	}

	// ��ʼ��Ϸ
	if (flag_start) {
		while (flag_start) {
			// ��ʼ����������
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

			// ��ʼ����ͼ
			mymap = mymap.initMap(path);
			// ��ʼ��ħʯ��������ħ���������
			scontroller.init();
			ccontroller.init();
			ccontroller.initStoneList(&scontroller);
			// ��ʼ��ħ�������
			mcontroller.init();
			// ��ʼ��ͼ����Ⱦ��
			imgrender.init();
			
			// ������Ϸ
			while (flag_start) {
				start_time = clock();	// ����������Ϸ֡��
				mymap.reloadmap();		// ÿһ������ori_map����map����������õ���mcube��msource���������ʽ�洢
				ccontroller.update(mymap);

				// �����궯��
				if (peekmessage(&msg, EX_MOUSE)) {	// ��������
					mclick.setP(msg.x, msg.y);
					switch (msg.message) {
						case WM_LBUTTONDOWN:
							// ���������̵���
							if (mclick.inRec(store_rec)) {
								if (mclick.y <= SCREEN_SQUARE_SIZE_HALF && nmoney >= V1_PRICE) {
									click_categoray = CUBE_V1;
									click_price = V1_PRICE;
									mouse_cube = new Cube_V1;			// ��ʼ��1����Ӧ���͵�ħ����
									highlight_store = 1;				// ���ñ�־������Ҫ��������ħ����
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
								// ��ʱ���������Ѿ��д������ħ������
							}

							// ��������ħ����
							if (mclick.inRec(monster_rec)) {
								if (choose_stone_p.v) {	// ����Ѿ�ѡ�д�����ħʯ
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

							// �������˵�ͼ��
							if (mclick.inRec(map_rec)) {
								if (mouse_cube != NULL) {	// ����д������ħ�����������Ƿ���԰����ڵ�ͼ��
									mapxy = transtoMapxy(mymap, mclick);
									int res;
									res = mouse_cube->setInMap(mymap, mapxy);
									if (res == OK) {
										ccontroller.addCube(*mouse_cube);	// ����ħ����ѹ�뷨�������
										nmoney -= click_price;
										click_categoray = 0;				// �������
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

							// ��������ħʯ�洢��
							if (mclick.inRec(storage_rec)) {
								if (mouse_cube != NULL) {	// �����ǰ�Ѿ�ѡ�д�����ħ�����������ħ���󻺴棬�ٽ���ħʯ�������
									click_categoray = 0;
									mouse_cube = NULL;
									click_price = 0;
									highlight_store = 0;
									flag_putstore = TRUE;
								}
								choose_stone_p = transtoStoragexy(mclick);
								if (scontroller.stonelist[choose_stone_p.x][choose_stone_p.y] <= 0) {	// ���ѡ�еĴ�����ħʯ�������㣬��ѡ����Ч
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
							if (click_categoray > 0) {		// ������Ҽ�ʱ������Ѿ�ѡ����ĳһ�������ħ��������ħ���������ת
								mouse_cube->turnRound();
							}
							break;
						default:;
					}
				}

				// ��Ⱦ��Ļ�ϵ�5������
				if (flag_boom) {
					flag_boom = 0;
					// ˢ�±�����ͼ
					imgrender.putBackground();
					// ˢ�µ�ħ��ͼ��
					mymap.reloadmap();
					ccontroller.update(mymap);
					imgrender.putMap(mymap);
					flag_putmap = 0;
					// ˢ���̵���
					imgrender.putStore(nmoney, highlight_store);
					// ˢ�½�Ǯ�����������
					imgrender.clearCursor();
					imgrender.putMoney(nmoney);
					// ˢ��ħʯ�洢��
					imgrender.flag_putstorage = 0;
					imgrender.putStorage(&scontroller);
					// ˢ��ħ����
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

				// ��Ⱦ������Ĵ�����ħ����
				if (mouse_cube != NULL) {
					mclick.setP(msg.x, msg.y);
					imgrender.putMap(mymap);
					imgrender.putRealCursor(mclick, mymap, mouse_cube->category, mouse_cube->orient);
				}

				// ��UPDATE_TIME���µ��¼�
				temp_time = clock();
				if ((temp_time - last_time) > UPDATE_TIME) {
					nmoney++;
					last_time = temp_time;
					imgrender.putStore(nmoney, highlight_store);
					imgrender.putStorage(&scontroller);
					if (mouse_cube != NULL) {	// ����д������ħ�������������ħ�����䷽��
						imgrender.putCursorCube(mouse_cube->category, mouse_cube->orient);
					}
					else if (choose_stone_p.v) {		// ����д������ħʯ�����ħʯ����ȼ�
						imgrender.putCursorStone(choose_stone_p);
					}
					imgrender.putMonster(mcontroller);
				}
				// ħ�����
				if ((temp_time - monster_last_time) > MONSTER_UPDATE_TIME) {
					flag_start = mcontroller.update();
					monster_last_time = temp_time;
				}
				showFPS(start_time);	// ���֡��������֡���ȶ�
			}
			// ��Ϸ�����ж�
			cout << "sss��Ϸ������" << endl;
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

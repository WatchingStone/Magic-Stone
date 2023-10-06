#pragma once// ͼ����Ⱦ����
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

/* ������Ϸ��ʼ�˵������������ťRECT��Χ*/
extern RECT start_button_rec;
extern RECT quit_button_rec;

/* ����5����Ҫ�����RECT��Χ*/
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
	IMAGE* is_store;	// �̵��������ͼƬ
	IMAGE* is_storage;	// ħʯ�洢�������ͼƬ
	bool flag_putstorage = 0;	// ���ڴ洢���ı�־λ����ʼ��1������Ҫ���ħʯͼ�񣬺����������ħʯͼ��ֻ��Ҫ�޸�ħʯ��ʾ����
	IMAGE* is_monster;	// ħ���������ͼƬ
	IMAGE* is_money;	// ��Ǯ�������ͼƬ
	IMAGE* is_cursor;	// �������Ĺ��ͼƬ
	IMAGE* is_boom;		// ħʯ��ħ�ﱬը��֡ͼƬ

	ImageRender() { imgsource = NULL; is_store = NULL; }
	IMAGE* loadimg();		// �����������������ͼƬ���������е�����loadimg_x������Ĭ�Ϸ��ص�ͼ����ͼƬ
	IMAGE* loadimg_store();
	IMAGE* loadimg_storage();
	IMAGE* loadimg_monster();
	IMAGE* loadimg_money();
	IMAGE* loadimg_cursor();
	IMAGE* loadimg_boom();
	void putBackground();
	void putAllImage();
	void putMap(Map mymap);							// ��ʾ��ͼ��
	void putStore(int nmoney, int highlight);		// ��ʾ�̵���
	void putStorage(StoneController* scontroller);	// ��ʾħʯ�洢��
	void putMonster(MonsterController mcontroller);	// ��ʾħ����
	void putMoney(int nmoney);						// ��ʾ��Ǯ��
	void putCursorCube(int cube_cat, int cube_ori);	// ���ý�Ǯ����ʾ������ħ����
	void putCursorStone(Point choose_stone_p);		// ���ý�Ǯ����ʾ������ħʯ
	void putBoom(queue<Monster>* to_render_queue);	// ���ħʯ��ħ��ı�ը��Ч

	void iniStartMenu();	// �����Ϸ��ʼ����˵�
	void gameover();		// �����Ϸ����ͼ��
	void clearCursor();		// ��ʱˢ�´�������
};

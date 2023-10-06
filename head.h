// 存放所有文件里的全局变量、宏定义
#pragma once
// main.h
#define UPDATE_TIME 1000	// 单位：毫秒，影响某些固定时间间隔发生的事件

// Map.h
#define MAX_SOURCE_COUNT  20			// 地图载入的魔力之源数量上限
#define MAX_CUBE_COUNT  20

// MagicCube.h
#define PRODUCE_TIME 4000	// 魔法阵生产魔石的时间间隔

#define RED_SIGN 100
#define BLUE_SIGN 101
#define YELLOW_SIGN 102
#define GREEN_SIGN 103
#define WHITE_SIGN 104
#define OUTBOUNDS -1	// 检测魔法阵区域是否超出地图范围
#define EXIST_CUBE -2	// 检测魔法阵区域是否已经存在其他魔法阵
#define OK 666			// 一切正常
#define ERROR 123		// 出现错误

#define BLANK	999		// 非魔法阵的标识符
#define CUBE_V1 1000	// 六个魔法阵类的标识符
#define CUBE_V2 1001
#define CUBE_T	1002
#define CUBE_B	1003
#define CUBE_C	1004
#define CUBE_L	1005

// Monster.h
#define MONSTER_UPDATE_TIME	3500	// 魔物每次更新移动的毫秒数
#define MONSTER_WIDTH 5
#define MONSTER_HEIGHT 11

#define MONSTER_PROBABILITY 60	// 怪物大军中每一格生成魔物的概率60%
#define ELITE_MONSTER_PROBABILITY 10	// 确认生成魔物的时候，生成精英魔物的概率是20%，即总概率为(MONSTER_PROBABILITY * ELITE_MONSTER_PROBABILITY)

// render.h
#define DEFAULT_RT	20			// 默认每一帧运行时间，越大，fps越低
#define UI_LINEWIDTH 10			// 屏幕背景的线条线宽
#define MONSTER_SQUARE_SHIFT 5	// 魔物区的长宽空余

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 750
#define SCREEN_SQUARE_SIZE 250	// 默认屏幕比例4:3，每一小格尺寸为250像素。屏幕分为[3][4]大小的数组
#define SCREEN_SQUARE_SIZE_HALF SCREEN_SQUARE_SIZE / 2
#define STORE_CUBE_SIZE 30		// 商店区魔法阵方块的尺寸

#define DAOMO_LU_X  SCREEN_SQUARE_SIZE	// 导魔区域（地图区域）的左上点坐标，导魔区域覆盖[2][1]~[2][2]的区域  
#define DAOMO_LU_Y	SCREEN_SQUARE_SIZE*2	
#define SQUARE_SIZE 50			// 导魔区域每一块MFloor的像素尺寸，默认为50*50像素

#define C_C 7					// CUBE_CATEGORY
#define C_S 5					// CUBE_SIZE
#define CCS C_C * C_S			// CCS为CUBE_CATEGORY_SIZE的缩写，用来简化imgs的图片索引

#define V1_PRICE 10				// 不同魔法阵的价格
#define V2_PRICE 10
#define T_PRICE 15
#define B_PRICE 15
#define C_PRICE 20
#define L_PRICE 20

#define NO_STORE 1				// 5个区域的代表值
#define NO_MONSTER  2
#define NO_MAP  3
#define NO_STORAGE  4
#define NO_MONEY  5

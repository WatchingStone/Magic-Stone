// ��������ļ����ȫ�ֱ������궨��
#pragma once
// main.h
#define UPDATE_TIME 1000	// ��λ�����룬Ӱ��ĳЩ�̶�ʱ�����������¼�

// Map.h
#define MAX_SOURCE_COUNT  20			// ��ͼ�����ħ��֮Դ��������
#define MAX_CUBE_COUNT  20

// MagicCube.h
#define PRODUCE_TIME 4000	// ħ��������ħʯ��ʱ����

#define RED_SIGN 100
#define BLUE_SIGN 101
#define YELLOW_SIGN 102
#define GREEN_SIGN 103
#define WHITE_SIGN 104
#define OUTBOUNDS -1	// ���ħ���������Ƿ񳬳���ͼ��Χ
#define EXIST_CUBE -2	// ���ħ���������Ƿ��Ѿ���������ħ����
#define OK 666			// һ������
#define ERROR 123		// ���ִ���

#define BLANK	999		// ��ħ����ı�ʶ��
#define CUBE_V1 1000	// ����ħ������ı�ʶ��
#define CUBE_V2 1001
#define CUBE_T	1002
#define CUBE_B	1003
#define CUBE_C	1004
#define CUBE_L	1005

// Monster.h
#define MONSTER_UPDATE_TIME	3500	// ħ��ÿ�θ����ƶ��ĺ�����
#define MONSTER_WIDTH 5
#define MONSTER_HEIGHT 11

#define MONSTER_PROBABILITY 60	// ��������ÿһ������ħ��ĸ���60%
#define ELITE_MONSTER_PROBABILITY 10	// ȷ������ħ���ʱ�����ɾ�Ӣħ��ĸ�����20%�����ܸ���Ϊ(MONSTER_PROBABILITY * ELITE_MONSTER_PROBABILITY)

// render.h
#define DEFAULT_RT	20			// Ĭ��ÿһ֡����ʱ�䣬Խ��fpsԽ��
#define UI_LINEWIDTH 10			// ��Ļ�����������߿�
#define MONSTER_SQUARE_SHIFT 5	// ħ�����ĳ������

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 750
#define SCREEN_SQUARE_SIZE 250	// Ĭ����Ļ����4:3��ÿһС��ߴ�Ϊ250���ء���Ļ��Ϊ[3][4]��С������
#define SCREEN_SQUARE_SIZE_HALF SCREEN_SQUARE_SIZE / 2
#define STORE_CUBE_SIZE 30		// �̵���ħ���󷽿�ĳߴ�

#define DAOMO_LU_X  SCREEN_SQUARE_SIZE	// ��ħ���򣨵�ͼ���򣩵����ϵ����꣬��ħ���򸲸�[2][1]~[2][2]������  
#define DAOMO_LU_Y	SCREEN_SQUARE_SIZE*2	
#define SQUARE_SIZE 50			// ��ħ����ÿһ��MFloor�����سߴ磬Ĭ��Ϊ50*50����

#define C_C 7					// CUBE_CATEGORY
#define C_S 5					// CUBE_SIZE
#define CCS C_C * C_S			// CCSΪCUBE_CATEGORY_SIZE����д��������imgs��ͼƬ����

#define V1_PRICE 10				// ��ͬħ����ļ۸�
#define V2_PRICE 10
#define T_PRICE 15
#define B_PRICE 15
#define C_PRICE 20
#define L_PRICE 20

#define NO_STORE 1				// 5������Ĵ���ֵ
#define NO_MONSTER  2
#define NO_MAP  3
#define NO_STORAGE  4
#define NO_MONEY  5

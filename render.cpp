#include "render.h"
RECT start_button_rec = {430, 420, 570, 480};
RECT quit_button_rec = {440, 530, 540, 590};

RECT store_rec = {0, 0,SCREEN_SQUARE_SIZE, SCREEN_SQUARE_SIZE * 3};
RECT monster_rec = {SCREEN_SQUARE_SIZE, 0,SCREEN_SQUARE_SIZE * 3, SCREEN_SQUARE_SIZE * 2};
RECT map_rec = {SCREEN_SQUARE_SIZE, SCREEN_SQUARE_SIZE * 2,SCREEN_SQUARE_SIZE * 3, SCREEN_SQUARE_SIZE * 3};
RECT storage_rec = {SCREEN_SQUARE_SIZE * 3, 0,SCREEN_SQUARE_SIZE * 4, SCREEN_SQUARE_SIZE * 2};
RECT money_rec = {SCREEN_SQUARE_SIZE * 3, SCREEN_SQUARE_SIZE * 2,SCREEN_SQUARE_SIZE * 4, SCREEN_SQUARE_SIZE * 3};

// 输出png图片，第一个参数默认为NULL即可输出到屏幕，x和y是输出到屏幕上的坐标，srcimg为图片源的地址
// 来源：https://blog.csdn.net/Solahalo/article/details/127598433?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522169537609616800197093408%2522%252C%2522scm%2522%253A%252220140713.130102334..%2522%257D&request_id=169537609616800197093408&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~sobaiduend~default-2-127598433-null-null.142^v94^chatsearchT3_1&utm_term=easyx%E5%8A%A0%E8%BD%BDpng
void transparentimage3(IMAGE* dstimg, int x, int y, IMAGE* srcimg) 
{
	HDC dstDC = GetImageHDC(dstimg);
	HDC srcDC = GetImageHDC(srcimg);
	int w = srcimg->getwidth();
	int h = srcimg->getheight();
	BLENDFUNCTION bf = {AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};
	AlphaBlend(dstDC, x, y, w, h, srcDC, 0, 0, w, h, bf);
}


// 由鼠标点击的绝对坐标转换为地图区域上的相对坐标
Point transtoMapxy(Map mymap, Point click) {
	int w = mymap.width, h = mymap.height;
	int x = click.x, y = click.y;
	x -= SCREEN_SQUARE_SIZE;				// 得到鼠标位置在地图区域中的相对像素坐标
	y -= 2 * SCREEN_SQUARE_SIZE;
	int temp = x % SQUARE_SIZE;
	x = (int)((x - temp) / SQUARE_SIZE);	// x代表屏幕上的width
	temp = y % SQUARE_SIZE;					// y代表屏幕上的height
	y = (int)((y - temp) / SQUARE_SIZE);
	cout << "in map( " << y << " , " << x << " )" << endl;
	return Point(y, x);						// 注意x和y的意义在Point中有所不同
}
// 检测鼠标点击到了魔石存储区的哪一个魔石(第x行，第y列)，返回（h,w,1）
Point  transtoStoragexy(Point mclick) {
	int STORAGE_SQUARE_W = 50;					// 50来自于魔石图片的尺寸为50*50
	int STORAGE_SQUARE_H = 50 + 25;				// +25来自于数字的字号
	STORAGE_SQUARE_W = (SCREEN_SQUARE_SIZE / 4) - 1;
	STORAGE_SQUARE_H = (SCREEN_SQUARE_SIZE / 3) - 1;
	int wshift = 36, hshift = 57;
	int x = mclick.x, y = mclick.y;
	x -= SCREEN_SQUARE_SIZE * 3 + wshift;			// 得到鼠标位置在存储区中的相对像素坐标
	y -= hshift;
	int temp = x % STORAGE_SQUARE_W;
	x = (int)((x - temp) / STORAGE_SQUARE_W);		// x代表屏幕上的width
	temp = y % STORAGE_SQUARE_H;					// y代表屏幕上的height
	y = (int)((y - temp) / STORAGE_SQUARE_H);
	if (y >= 5) return Point(0, 0, 0);
	return Point(y, x, 1);							// 注意x和y的意义在Point中有所不同

}
// 检测鼠标点击到了魔物区的哪一个魔物(第x行，第y列)
Point transtoMonsterxy(Point mclick) {
	int wshift = 0, hshift = 0;
	int luw = SCREEN_SQUARE_SIZE + wshift;
	int luh = hshift;
	int w = mclick.x - luw;
	int h = mclick.y - luh;
	int pw = (w - w % SQUARE_SIZE) / SQUARE_SIZE;
	int ph = (h - h % SQUARE_SIZE) / SQUARE_SIZE;
	//cout << "在魔物区中点击了（ " << ph << " , " << pw << " )" << endl;
	return Point(ph, pw, 1);
}
// 使用RECT类{left, top, right, bottom}对clearrectangle()进行重载
void clearrectangle(RECT area) {
	clearrectangle(area.left, area.top, area.right, area.bottom);
}
// 使用RECT类{left, top, right, bottom}对FlushBatchDraw()进行重载
void FlushBatchDraw(RECT area) {
	FlushBatchDraw(area.left, area.top, area.right, area.bottom);
}
// 使用RECT类{left, top, right, bottom}对EndBatchDraw()进行重载
void EndBatchDraw(RECT area) {
	EndBatchDraw(area.left, area.top, area.right, area.bottom);
}
// 方便地进行坐标转换，输入例（1000， 1， 0）
int index(int cat, int no, int orient) {
	cat -= BLANK;
	return cat * C_S * 4 + no * 4 + orient;
}
// 读入res文件夹中的魔法阵与魔力之源与地图背景图片资源
IMAGE* ImageRender::loadimg() {
	IMAGE* imgs = new IMAGE[8 * 5 * 4];		// 有7种魔法阵（写作8才有足够的空间），每种魔法阵最多有5块，每块有4种朝向
	TCHAR imgpath[30] = {0};
	wchar_t* temp;
	int cat_i;
	int i, j, k;

	// 读取地图区域图片
	for (cat_i = BLANK; cat_i < CUBE_L + 1; cat_i++) {	// 依次遍历载入图片
		switch (cat_i) {
			case BLANK:
				loadimage(&imgs[0], _T("./res/map/map_background.bmp"));	// 图片数组前5项分别为：背景、红色源、蓝色源、黄色源、绿色源
				loadimage(&imgs[1], _T("./res/map/ms_red.bmp"));			// 多余位置空闲
				loadimage(&imgs[2], _T("./res/map/ms_blue.bmp"));
				loadimage(&imgs[3], _T("./res/map/ms_yellow.bmp"));
				loadimage(&imgs[4], _T("./res/map/ms_green.bmp"));
				break; 
			case CUBE_V1:
				_tcscpy_s(imgpath, _T("./res/map/1000_1_0.bmp"));
				for (i = 1; i < 4; i++) {
					for (j = 0; j < 4; j++) {
						imgpath[15] = (TCHAR)(i + '0');
						imgpath[17] = (TCHAR)(j + '0');
						//wcout << imgpath << endl;
						loadimage(&imgs[1 * C_S * 4 + i * 4 + j], imgpath);
					}
				}
				break;
			case CUBE_V2:
				_tcscpy_s(imgpath, _T("./res/map/1001_1_0.bmp"));
				for (i = 1; i < 4; i++) {
					for (j = 0; j < 4; j++) {
						imgpath[15] = (TCHAR)(i + '0');
						imgpath[17] = (TCHAR)(j + '0');
						//wcout << imgpath << endl;
						loadimage(&imgs[2 * C_S * 4 + i * 4 + j], imgpath);
					}
				}
				break;
			case CUBE_T:
				_tcscpy_s(imgpath, _T("./res/map/1002_1_0.bmp"));
				for (i = 1; i < 5; i++) {	// T有4块
					for (j = 0; j < 4; j++) {
						imgpath[15] = (TCHAR)(i + '0');
						imgpath[17] = (TCHAR)(j + '0');
						//wcout << imgpath << endl;
						loadimage(&imgs[3 * C_S * 4 + i * 4 + j], imgpath);
					}
				}
				break;
			case CUBE_B:								
				_tcscpy_s(imgpath, _T("./res/map/1003_1_0.bmp"));
				for (i = 1; i < 5; i++) {	// B有4块						
					for (j = 0; j < 4; j++) {
						imgpath[15] = (TCHAR)(i + '0');
						imgpath[17] = (TCHAR)(j + '0');
						//wcout << imgpath << endl;
						loadimage(&imgs[4 * C_S * 4 + i * 4 + j], imgpath);
					}
				}
				break;
			case CUBE_C:
				_tcscpy_s(imgpath, _T("./res/map/1004_1_0.bmp"));
				for (i = 1; i < 6; i++) {	// C有5块
					for (j = 0; j < 4; j++) {
						imgpath[15] = (TCHAR)(i + '0');
						imgpath[17] = (TCHAR)(j + '0');
						//wcout << imgpath << endl;
						loadimage(&imgs[5 * C_S * 4 + i * 4 + j], imgpath);
					}
				}
				break;
			case CUBE_L:
				_tcscpy_s(imgpath, _T("./res/map/1005_1_0.bmp"));
				for (i = 1; i < 6; i++) {	// L有5块
					for (j = 0; j < 4; j++) {
						imgpath[15] = (TCHAR)(i + '0');
						imgpath[17] = (TCHAR)(j + '0');
						//wcout << imgpath << endl;
						loadimage(&imgs[6 * C_S * 4 + i * 4 + j], imgpath);
					}
				}
				break;
			default: break;
		}		
	}
	imgsource = imgs;	

	// 读取商店区域图片
	loadimg_store();
	loadimg_storage();
	loadimg_cursor();
	loadimg_monster();
	loadimg_boom();
	return imgs;
}
// 读入商店区所需的魔法阵图片资源，按V1,V2,T,B,C,L的顺序载入
IMAGE* ImageRender::loadimg_store() {
	IMAGE* imgs = new IMAGE[13];
	loadimage(&imgs[0], _T("./res/store/0store_v1.png"));
	loadimage(&imgs[1], _T("./res/store/0store_v2.png"));
	loadimage(&imgs[2], _T("./res/store/0store_t.png"));
	loadimage(&imgs[3], _T("./res/store/0store_b.png"));
	loadimage(&imgs[4], _T("./res/store/0store_c.png"));
	loadimage(&imgs[5], _T("./res/store/0store_l.png"));
	loadimage(&imgs[6], _T("./res/store/store_v1.png"));
	loadimage(&imgs[7], _T("./res/store/store_v2.png"));
	loadimage(&imgs[8], _T("./res/store/store_t.png"));
	loadimage(&imgs[9], _T("./res/store/store_b.png"));
	loadimage(&imgs[10], _T("./res/store/store_c.png"));
	loadimage(&imgs[11], _T("./res/store/store_l.png"));
	loadimage(&imgs[12], _T("./res/store/store_highlight.bmp"));
	is_store = imgs;
	return imgs;
}
// 读入商店区所需的魔法阵图片资源，按red，blue，yellow，green，white的顺序载入
IMAGE* ImageRender::loadimg_storage() {
	IMAGE* imgs = new IMAGE[15];
	loadimage(&imgs[0], _T("./res/storage/ms_red_1.bmp"));
	loadimage(&imgs[1], _T("./res/storage/ms_red_2.bmp"));
	loadimage(&imgs[2], _T("./res/storage/ms_red_3.bmp"));
	loadimage(&imgs[3], _T("./res/storage/ms_blue_1.bmp"));
	loadimage(&imgs[4], _T("./res/storage/ms_blue_2.bmp"));
	loadimage(&imgs[5], _T("./res/storage/ms_blue_3.bmp"));
	loadimage(&imgs[6], _T("./res/storage/ms_yellow_1.bmp"));
	loadimage(&imgs[7], _T("./res/storage/ms_yellow_2.bmp"));
	loadimage(&imgs[8], _T("./res/storage/ms_yellow_3.bmp"));
	loadimage(&imgs[9], _T("./res/storage/ms_green_1.bmp"));
	loadimage(&imgs[10], _T("./res/storage/ms_green_2.bmp"));
	loadimage(&imgs[11], _T("./res/storage/ms_green_3.bmp"));
	loadimage(&imgs[12], _T("./res/storage/ms_white_1.bmp"));
	loadimage(&imgs[13], _T("./res/storage/ms_white_2.bmp"));
	loadimage(&imgs[14], _T("./res/storage/ms_white_3.bmp"));
	is_storage = imgs;
	return imgs;
}
IMAGE* ImageRender::loadimg_monster(){
	IMAGE* imgs = new IMAGE[11];
	loadimage(&imgs[0], _T("./res/monster/monster_map.bmp"));
	loadimage(&imgs[1], _T("./res/monster/monster_red_0.bmp"));
	loadimage(&imgs[2], _T("./res/monster/monster_red_1.bmp"));
	loadimage(&imgs[3], _T("./res/monster/monster_blue_0.bmp"));
	loadimage(&imgs[4], _T("./res/monster/monster_blue_1.bmp"));
	loadimage(&imgs[5], _T("./res/monster/monster_yellow_0.bmp"));
	loadimage(&imgs[6], _T("./res/monster/monster_yellow_1.bmp"));
	loadimage(&imgs[7], _T("./res/monster/monster_green_0.bmp"));
	loadimage(&imgs[8], _T("./res/monster/monster_green_1.bmp"));
	loadimage(&imgs[9], _T("./res/monster/monster_white_0.bmp"));
	loadimage(&imgs[10], _T("./res/monster/monster_white_1.bmp"));
	is_monster = imgs;
	return imgs;
}
IMAGE* ImageRender::loadimg_money() {
	IMAGE* imgs = new IMAGE;
	return imgs;
}
IMAGE* ImageRender::loadimg_cursor() {
	IMAGE* imgs = new IMAGE[24];
	loadimage(&imgs[0], _T("./res/cursor/cursor_magiccube_v1_0.bmp"));
	loadimage(&imgs[1], _T("./res/cursor/cursor_magiccube_v1_1.bmp"));
	loadimage(&imgs[2], _T("./res/cursor/cursor_magiccube_v1_2.bmp"));
	loadimage(&imgs[3], _T("./res/cursor/cursor_magiccube_v1_3.bmp"));
	loadimage(&imgs[4], _T("./res/cursor/cursor_magiccube_v2_0.bmp"));
	loadimage(&imgs[5], _T("./res/cursor/cursor_magiccube_v2_1.bmp"));
	loadimage(&imgs[6], _T("./res/cursor/cursor_magiccube_v2_2.bmp"));
	loadimage(&imgs[7], _T("./res/cursor/cursor_magiccube_v2_3.bmp"));
	loadimage(&imgs[8], _T("./res/cursor/cursor_magiccube_t_0.bmp"));
	loadimage(&imgs[9], _T("./res/cursor/cursor_magiccube_t_1.bmp"));
	loadimage(&imgs[10], _T("./res/cursor/cursor_magiccube_t_2.bmp"));
	loadimage(&imgs[11], _T("./res/cursor/cursor_magiccube_t_3.bmp"));
	loadimage(&imgs[12], _T("./res/cursor/cursor_magiccube_b_0.bmp"));
	loadimage(&imgs[13], _T("./res/cursor/cursor_magiccube_b_1.bmp"));
	loadimage(&imgs[14], _T("./res/cursor/cursor_magiccube_b_2.bmp"));
	loadimage(&imgs[15], _T("./res/cursor/cursor_magiccube_b_3.bmp"));
	loadimage(&imgs[16], _T("./res/cursor/cursor_magiccube_c_0.bmp"));
	loadimage(&imgs[17], _T("./res/cursor/cursor_magiccube_c_1.bmp"));
	loadimage(&imgs[18], _T("./res/cursor/cursor_magiccube_c_2.bmp"));
	loadimage(&imgs[19], _T("./res/cursor/cursor_magiccube_c_3.bmp"));
	loadimage(&imgs[20], _T("./res/cursor/cursor_magiccube_l_0.bmp"));
	loadimage(&imgs[21], _T("./res/cursor/cursor_magiccube_l_1.bmp"));
	loadimage(&imgs[22], _T("./res/cursor/cursor_magiccube_l_2.bmp"));
	loadimage(&imgs[23], _T("./res/cursor/cursor_magiccube_l_3.bmp"));

	is_cursor = imgs;
	return imgs;
}
IMAGE* ImageRender::loadimg_boom() {
	IMAGE* imgs = new IMAGE[9];
	loadimage(&imgs[0], _T("./res/boom/boom_0_0.png"));
	loadimage(&imgs[1], _T("./res/boom/boom_0_1.png"));
	loadimage(&imgs[2], _T("./res/boom/boom_0_2.png"));
	loadimage(&imgs[3], _T("./res/boom/boom_1_0.png"));
	loadimage(&imgs[4], _T("./res/boom/boom_1_1.png"));
	loadimage(&imgs[5], _T("./res/boom/boom_1_2.png"));
	loadimage(&imgs[6], _T("./res/boom/boom_2_0.png"));
	loadimage(&imgs[7], _T("./res/boom/boom_2_1.png"));
	loadimage(&imgs[8], _T("./res/boom/boom_2_2.png"));
	is_boom = imgs;
	return imgs;
}

// 输出背景图片
void ImageRender::putBackground() {
	IMAGE* img = new IMAGE;
	loadimage(img, _T("./res/BACKGROUND.png"));
	putimage(0, 0, img);
}
// 测试所有图片的输出，已完成导魔区域部分
void ImageRender::putAllImage() {
	IMAGE* imgs = imgsource;
	int i, j, k;
	BeginBatchDraw();
	for (i = 0; i < 5; i++) {	// 输出0~4号背景图片与4个魔力之源
		transparentimage3(NULL, DAOMO_LU_X + i*SQUARE_SIZE, 0,&imgs[i]);
	}
	for (i = 1; i < 7; i++) {	// 输出六个魔法阵
		switch (i) {		
			case 1:
			case 2:
				for (j = 1; j < 4; j++) {		// V1和V2有3块，编号为1~3
					for (k = 0; k < 4; k++) {	// 每一块有4个朝向
						transparentimage3(NULL, DAOMO_LU_X + k * SQUARE_SIZE, ((i-1) * 3 + j) * SQUARE_SIZE, &imgs[i * C_S * 4 + j * 4 + k]);
					}
				}
				break;
			case 3:
			case 4:
				for (j = 1; j < 5; j++) {		// T和B有4块，编号为1~4
					for (k = 0; k < 4; k++) {	// 每一块有4个朝向
						transparentimage3(NULL, DAOMO_LU_X + k * SQUARE_SIZE, ((i - 3) * 4 + j + 6) * SQUARE_SIZE, &imgs[i * C_S * 4 + j * 4 + k]);
					}
				}
				break;
			case 5:
			case 6:
				for (j = 1; j < 6; j++) {		// C和L有5块，编号为1~5
					for (k = 0; k < 4; k++) {	// 每一块有4个朝向
						transparentimage3(NULL, DAOMO_LU_X + (k+4) * SQUARE_SIZE, ((i - 5) * 5 + j) * SQUARE_SIZE, &imgs[i * C_S * 4 + j * 4 + k]);
					}
				}
				break;
			default:
				break;
		}
	}
	FlushBatchDraw();
	EndBatchDraw();
}
// 输出地图状态，含魔力之源和魔法阵
void ImageRender::putMap(Map mymap) {
	int w = mymap.width, h = mymap.height;
	int i, j, k;
	int mfx, mfy;		// 每一块地图在屏幕上的坐标
	int tms, tccat,tcno, tcor;		// temp_msource, temp_cube_category,temp_cube_no, temp_cube_orient
	
	clearrectangle(map_rec);
	//initgraph(SCREEN_WIDTH, SCREEN_HEIGHT, EX_SHOWCONSOLE);
	for (i = 0; i < w;i++) {
		for (j = 0; j < h; j++) {
			mfx = DAOMO_LU_X + j * SQUARE_SIZE;
			mfy = DAOMO_LU_Y + i * SQUARE_SIZE;
			transparentimage3(NULL, mfx, mfy, &imgsource[0]);							// 先输出背景
			tms = mymap.map[i * w + j].msource;

			if( tms > 0)	transparentimage3(NULL, mfx, mfy, &imgsource[tms - 99]);	// 再输出魔力之源
			tccat = mymap.map[i * w + j].cube_category;
			if (tccat > 0) {															// 最后输出魔法阵
				tcno = mymap.map[i * w + j].cube_no;
				tcor = mymap.map[i * w + j].cube_orient;
				// tccat取值为1000~1005，tcno取值为1~5，tcor取值为0~4
				transparentimage3(NULL, mfx, mfy, &imgsource[(tccat-999)*C_S*4 + tcno*4 + tcor]);
			}
		}
	}
}
// 输出商店区域，实时根据当前金钱量点亮可选择的魔法阵，根据hightlight(1~6)选中高亮某一魔法阵
void ImageRender::putStore(int nmoney, int highlight) {
	clearrectangle(0, 0, SCREEN_SQUARE_SIZE - UI_LINEWIDTH, SCREEN_SQUARE_SIZE * 3);
	BeginBatchDraw();

	int flag[6] = {0};			// 标志位置1时代表对应种类的魔法阵可以购买
	int i = 0;
	if (nmoney >= V1_PRICE) {	// 当金钱足够购买某一法阵时，将代表该法阵的标志位置1
		flag[i++] = 1;
		if (nmoney >= V2_PRICE) {
			flag[i++] = 1;
			if (nmoney >= T_PRICE) {
				flag[i++] = 1;
				if (nmoney >= B_PRICE) {
					flag[i++] = 1;
					if (nmoney >= L_PRICE) {
						flag[i++] = 1;
						if (nmoney >= C_PRICE) {
							flag[i++] = 1;
						}
					}
				}
			}
		}
	}
	for (i = 0; i < 6; i++) {
		putimage(0, SCREEN_SQUARE_SIZE * i / 2, &is_store[i + flag[i] * 6]);
	}
	highlight--;
	transparentimage3(NULL, 0, SCREEN_SQUARE_SIZE * highlight / 2, &is_store[12]);
	FlushBatchDraw();
	EndBatchDraw();
}
// 输出魔石存储区域
void ImageRender::putStorage(StoneController* scontroller) {
	int i, j;
	int wshift = 36, hshift = 57;	// 最左上角的图片离存储区边缘的间隔
	int tw, th;
	int luw = SCREEN_SQUARE_SIZE * 3, luh = 0;	// 魔石存储区左上角点的width和height
	int STORAGE_SQUARE_W = (SCREEN_SQUARE_SIZE / 4) - 1;
	int STORAGE_SQUARE_H = (SCREEN_SQUARE_SIZE / 3) - 1;
	BeginBatchDraw();
	if (!flag_putstorage) {	// 如果没有输出过5种魔石的图像，则初始化输出
		for (i = 0; i < 5; i++) {
			for (j = 0; j < 3; j++) {
				tw = luw + j * STORAGE_SQUARE_W + wshift;
				th = luh + i * STORAGE_SQUARE_H + hshift;
				putimage(tw, th, &is_storage[i * 3 + j]);
			}
		}
		flag_putstorage = 1;
	}
	TCHAR count[3] = {0};	// 存储单个魔石个数
	for (i = 0; i < 5; i++) {
		for (j = 0; j < 3; j++) {
			tw = luw + j * STORAGE_SQUARE_W + wshift;
			th = luh + i * STORAGE_SQUARE_H + hshift + 50;	// +50是因为存储区的魔石图片大小为50*50
			_stprintf_s(count, _T("%d"), scontroller->stonelist[i][j]);
			settextstyle(25, 0, _T("黑体"));
			clearrectangle(tw, th, tw + 70, th + 30);
			outtextxy(tw, th, count);
		}
	}
	FlushBatchDraw();
	EndBatchDraw();
}
// 输出金钱和得分
void ImageRender::putMoney(int nmoney) {
	TCHAR money[10] = {0};
	TCHAR info[] = _T("金钱：");
	TCHAR scores[5] = {0};
	int w = SCREEN_SQUARE_SIZE * 3 + UI_LINEWIDTH;
	int h = SCREEN_SQUARE_SIZE * 2 + UI_LINEWIDTH + 160;	// +160是来自+150的待部署图片尺寸以及+10的余量
	_stprintf_s(money, _T("%d"), nmoney);
	_stprintf_s(scores, _T("%d"), score);
	
	LOGFONT f;
	settextcolor(YELLOW);
	settextstyle(30, 0, _T("黑体"));
	outtextxy(w , h, info);
	outtextxy(w + textwidth(_T("金钱：")), h, money);
	settextcolor(WHITE);	
	outtextxy(w, h + 35, _T("得分："));						// +30来自字号30
	outtextxy(w + textwidth(_T("得分：")), h + 35, scores);
}
// 输出待部署魔法阵
void ImageRender::putCursorCube(int cube_cat, int cube_ori) {
	IMAGE* img = &is_cursor[(cube_cat - CUBE_V1) * 4 + cube_ori];
	int luw, luh;
	luw = SCREEN_SQUARE_SIZE * 3 + UI_LINEWIDTH;
	luh = SCREEN_SQUARE_SIZE * 2 + UI_LINEWIDTH;
	clearrectangle(luw, luh, luw + 150, luh + 150);
 	transparentimage3(NULL, luw, luh, img);
}
// 输出待部署魔石
void ImageRender::putCursorStone(Point choose_stone_p) {
	IMAGE* img = &is_storage[choose_stone_p.x * 3 + choose_stone_p.y];
	cout << "点击的相对坐标为：<" << choose_stone_p.x << " , " << choose_stone_p.y << " )" << endl;
	int luw, luh;
	luw = SCREEN_SQUARE_SIZE * 3 + UI_LINEWIDTH;
	luh = SCREEN_SQUARE_SIZE * 2 + UI_LINEWIDTH;
	clearrectangle(luw, luh, luw + 150, luh + 150);
	transparentimage3(NULL, luw, luh, img);
}

// 输出魔物地图与魔物大军
void ImageRender::putMonster(MonsterController mcontroller) {
	int luw = SCREEN_SQUARE_SIZE + MONSTER_SQUARE_SHIFT, luh = MONSTER_SQUARE_SHIFT;			// MONSTER_SQUARE_SHIFT=5来自于地图尺寸为490*490
	int tw, th;	
	int i, j;
	int tcolor, tlevel;
	clearrectangle(monster_rec);
	transparentimage3(NULL, luw, luh, &is_monster[0]);	// 先输出魔物地图
	for (i = 0; i < mcontroller.height - 1; i++) {
		for (j = 0; j < mcontroller.width; j++) {
			tcolor = mcontroller.monsters[i][j].color;
			if (tcolor > 0) {
				tcolor -= RED_SIGN;
				tlevel = mcontroller.monsters[i][j].level;
				tw = j * SQUARE_SIZE + luw;						// 计算每个地块的具体左上坐标
				th = i * SQUARE_SIZE + luh;
				transparentimage3(NULL, tw, th, &is_monster[tcolor * 2 + tlevel + 1]);	// +1是来自于is_monster[0]存储了魔物地图，所以后续都要偏移+1
			}
		}
	}
}

// 输出魔石和魔物的爆炸特效
void ImageRender::putBoom(queue<Monster>* to_render_queue) {
	Monster tm;									// 输出部署魔石和魔物的区域（暂时都调用魔物爆炸的特效）
	Point* to_boom_p = new Point[50];			// 记录所有待渲染的点的坐标与等级
	int count = 0;
	int luw = SCREEN_SQUARE_SIZE + MONSTER_SQUARE_SHIFT, luh = MONSTER_SQUARE_SHIFT;
	int tw, th, tv;
	int i, j;
	while (!to_render_queue->empty()) {
		tm = to_render_queue->front();
		to_render_queue->pop();
		to_boom_p[count].setP(tm.x, tm.y, tm.level);	// to_boom_p的每个Point为(h,w,level)
		count++;
	}
	for (i = 0; i < 3; i++) {		// 由于爆炸特效只有3帧，因此只循环3次
		j = 0;
		tv = to_boom_p[j].v;
		tw = to_boom_p[j].y * SQUARE_SIZE + luw - SQUARE_SIZE * tv;		// 计算每个地块的具体左上坐标，再将图片置中
		th = to_boom_p[j].x * SQUARE_SIZE + luh - SQUARE_SIZE * tv;
		transparentimage3(NULL, tw, th, &is_boom[tv * 3 + i]);
		for (j = 1; j < count; j++) {
			tv = to_boom_p[j].v + 1;									// 暂时让魔物爆炸的等级比魔石高1级
			tw = to_boom_p[j].y * SQUARE_SIZE + luw - SQUARE_SIZE * tv;	// 计算每个地块的具体左上坐标，再将图片置中
			th = to_boom_p[j].x * SQUARE_SIZE + luh - SQUARE_SIZE * tv;
			transparentimage3(NULL, tw, th, &is_boom[tv * 3 + i]);
		}
		Sleep(200);
	}
	delete to_boom_p;
}

// 输出游戏初始界面菜单
void ImageRender::iniStartMenu() {
	IMAGE* img = new IMAGE;
	loadimage(img, _T("./res/start_menu.png"));
	putimage(0, 0, img);
}
// 输出游戏结束图像
void ImageRender::gameover() {
	IMAGE* img = new IMAGE;
	loadimage(img, _T("./res/gameover.png"));
	transparentimage3(NULL,0, 0, img);
}
void ImageRender::clearCursor() {
	clearrectangle(UI_LINEWIDTH + SCREEN_SQUARE_SIZE * 3, UI_LINEWIDTH + SCREEN_SQUARE_SIZE * 2, UI_LINEWIDTH + SCREEN_SQUARE_SIZE * 3 + 200, UI_LINEWIDTH + SCREEN_SQUARE_SIZE * 2 + 200);
}
// 实时输出当前画面帧数，输入start_time
void showFPS(clock_t stime) {
	clock_t running_time = clock() - stime;
	if ((DEFAULT_RT - running_time) >= 0)  Sleep(DEFAULT_RT - running_time);	//	动态调整帧数
	TCHAR fpsinfo[15];
	running_time = clock() - stime;
	int fps = 1000 / running_time;
	_stprintf_s(fpsinfo, _T("FPS:%d"), fps);
	settextstyle(20, 0, _T("黑体"));
	//clearrectangle(0, 0, 125, 20);
	outtextxy(0, 0, fpsinfo);
}

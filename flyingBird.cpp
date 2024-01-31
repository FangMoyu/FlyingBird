#include <graphics.h>		// 引用图形库头文件
#include <conio.h>
#include<stdio.h>
#include<stdlib.h>
#include<mmsystem.h>
#pragma comment(lib,"WINMM.LIB")


#define width 685 //窗口宽度
#define height 800 //窗口高度
#define pipoWidth 140 //障碍物宽度
#define Bird_Width 34 //鸟宽度
#define Bird_height 24 //鸟高度
#define obstacle1_With 140 //障碍物宽度1
#define obstacle1_height 600 //障碍物高度1
#define obstacle2_With 140 //障碍物宽度2
#define obstacle2_height 600 //障碍物高度2
#define coins_Width 32 //金币宽度
#define coins_height 32 //金币高度

//图片对象
IMAGE bgImages;//背景图片
IMAGE BirdImage;//鸟图
IMAGE BirdBgImage;//鸟的背景图
IMAGE obstacle1;//障碍物图1
IMAGE bgobstacle1;//障碍物背景图1
IMAGE obstacle2;//障碍物图2
IMAGE bgobstacle2;//障碍物背景图2
IMAGE coins;//金币图
IMAGE GameOverImg;//游戏结束图标
IMAGE GameOverImgBg;
IMAGE gamerule;//游戏规则图片


//主菜单界面矩形对象
RECT rect1 = { 200,200,400,250 };
RECT rect2 = { 200,250,400,300 };
RECT rect3 = { 200,300,400,350 };
RECT rect4 = { 150, 200, 530, 250 };
RECT rect6 = { 200, 350, 400, 400 };

//显示详情页面的内容
RECT rect5 = { 0, 0, 100, 100 };
//4个障碍物对应的X，Y坐标
int Obstacle1X = 400;
int Obstacle2X = 350;
int Obstacle3X = 150;
int Obstacle4X = 200;
int Obstacle1Y = -300;
int Obstacle2Y = 500;
int Obstacle3Y = 600;
int Obstacle4Y = -550;

//用于计数金币旋转
int count = 0;
int coinX = 300;
int coinY = 300;


//鸟的X,Y坐标
int BirdX = 100;
int BirdY = 100;
//设置物体初始化移动速度
int removePoints = 5;
//惩罚计数器
int punishPoints = 0;
//显示得分
int Points = 0;
//保存玩家菜单选择结果
int playerChoice;

//障碍物移动和鸟自由下落
void removeObj();

//检查障碍物碰撞
int checkObstacles();

//按键监听
void keyPress();

//画障碍物
void drawObject();

//检查障碍物和鸟的位置是否超出边界
void checkObject();
//加载所有音乐
void loadAllMusic();
//检查是否碰到金币
int checkCoins();

//游戏界面主菜单
void gameHome();

//画主菜单
void drawMain();

//绘制返回主菜单
void drawReturnMain();

//重置游戏
void resetGame();

//处理主菜单函数
void handleMenuItem();

//画出详情界面
void drawInfoView();

//胜利界面
void drawWinView();

//绘制文本函数
void drawText(RECT rect, LPCTSTR str);

//开始游戏
void PlayGame();
int main(int argc ,char* argv[])
{
	
	initgraph(width, height, NOMINIMIZE|SHOWCONSOLE);
	/*
	NOMINIMIZE 禁用绘图环境的最小化按钮
	SHOWCONSOLE 保留控制台原窗口
	*/
	//窗口背景图
	loadimage(&bgImages,_T("D:/Fang/FlyingBird/image/image/back12.jpg"));//参数2是图片路径

	//小鸟的背景图片
	loadimage(&BirdBgImage, _T("D:/Fang/FlyingBird/image/image/bird1_1.gif"));

	//小鸟图片
	loadimage(&BirdImage, _T("D:/Fang/FlyingBird/image/image/bird1_2.gif"));

	//障碍物图片
	loadimage(&obstacle1, _T("D:/Fang/FlyingBird/image/image/pipe_down1.gif"));
	//障碍物背景1
	loadimage(&bgobstacle1, _T("D:/Fang/FlyingBird/image/image/pipe_down2.gif"));

	//障碍物图片2
	loadimage(&obstacle2, _T("D:/Fang/FlyingBird/image/image/pipe_up1.gif"));
	//障碍物背景2
	loadimage(&bgobstacle2, _T("D:/Fang/FlyingBird/image/image/pipe_up2.gif"));

	//加载 bmp 图
	loadimage(&coins, _T("D:/Fang/FlyingBird/image/image/ani.bmp"));

	//加载 GameOver 图
	loadimage(&GameOverImg, _T("D:/Fang/FlyingBird/image/image/gameover.jpg"));
	loadimage(&GameOverImgBg, _T("D:/Fang/FlyingBird/image/image/gameoverback.jpg"));


	loadimage(&gamerule, _T("D:/Fang/FlyingBird/image/image/gamerule.jpg"));


	//加载和播放背景音乐
	loadAllMusic();
	//绘图
	//绘制窗口
	putimage(0, 0, &bgImages);
	
	//进入游戏主界面
	gameHome();

	
	
		
	return 0;
}
int checkObstacles() {
	/*
			鸟的 x 坐标 >= 障碍物 x 坐标 - 小鸟的宽度

			鸟的 x 坐标 <= 障碍物的 x 坐标 + 障碍物的宽度

			鸟的 y 坐标 <= 障碍物的 y 坐标 + 障碍物的高度

			鸟的 y 坐标 + 鸟的高度 >= 障碍物的 y 坐标
			*/
	if ((BirdX >= Obstacle1X - Bird_Width && BirdX <= Obstacle1X + obstacle1_With && BirdY <= Obstacle1Y + obstacle1_height)
		|| (BirdX >= Obstacle2X - Bird_Width && BirdX <= Obstacle2X + obstacle2_With && BirdY + Bird_height >= Obstacle2Y)
		|| (BirdX >= Obstacle4X - Bird_Width && BirdX <= Obstacle4X + obstacle1_With && BirdY <= Obstacle4Y + obstacle1_height)
		|| (BirdX >= Obstacle3X - Bird_Width && BirdX <= Obstacle3X + obstacle2_With && BirdY + Bird_height >= Obstacle3Y)) {
		mciSendString(_T("close deadMusic"), NULL, 0, NULL);
		mciSendString(_T("open D:/Fang/FlyingBird/sound/sound/dead.mp3 alias deadMusic"), NULL, 0, NULL);
		mciSendString(_T("play deadMusic"), NULL, 0, NULL);
		return 1;
	}
	return 0;
}

//障碍物自由移动和鸟下落
void removeObj() {
	
	Obstacle1X -= removePoints;
	Obstacle2X -= removePoints;
	Obstacle3X -= removePoints;
	Obstacle4X -= removePoints;
	BirdY += 5;
	coinX -= 5;

	//增加惩罚机制，如果金币漏掉3的倍数，则障碍物移动速度+2
	if (punishPoints==3) {
		removePoints +=2;
		punishPoints = 0;
	}
}
void keyPress() {
	//按键来让鸟跳跃,按下键实现小鸟下移
	if (_kbhit()) {
		if (GetAsyncKeyState(VK_SPACE)) {
			mciSendString(_T("close jumpMusic"), NULL, 0, NULL);
			mciSendString(_T("open D:/Fang/FlyingBird/sound/sound/jump.mp3 alias jumpMusic"), NULL, 0, NULL);
			mciSendString(_T("play jumpMusic"), NULL, 0, NULL);
			if (BirdY > 0) {
				BirdY -= 40;
			}
		}
		if (GetAsyncKeyState(VK_DOWN)) {
				BirdY += 10;
		}

		
	}
}

void drawObject() {
	
	
	//绘制鸟背景图
	putimage(0, 0, &bgImages);
	putimage(BirdX, BirdY, &BirdBgImage, NOTSRCERASE);
	//绘制鸟
	putimage(BirdX, BirdY, &BirdImage, SRCINVERT);
	//画障碍物，共四个障碍物
	putimage(Obstacle1X, Obstacle1Y, &bgobstacle1, NOTSRCERASE);
	putimage(Obstacle1X, Obstacle1Y, &obstacle1, SRCINVERT);

	putimage(Obstacle4X, Obstacle4Y, &bgobstacle1, NOTSRCERASE);
	putimage(Obstacle4X, Obstacle4Y, &obstacle1, SRCINVERT);

	putimage(Obstacle2X, Obstacle2Y, &bgobstacle2, NOTSRCERASE);
	putimage(Obstacle2X, Obstacle2Y, &obstacle2, SRCINVERT);

	putimage(Obstacle3X, Obstacle3Y, &bgobstacle2, NOTSRCERASE);
	putimage(Obstacle3X, Obstacle3Y, &obstacle2, SRCINVERT);
	
	//绘制显示得分
	char str[100];
	sprintf_s(str, "%d", Points);
	outtextxy(0, 0, _T("您的得分为："));
	outtextxy(width/3, 0, *str);

	//画金币和实现旋转
	if (count > 3) {
		count = 0;
	}
	
		putimage(coinX, coinY, coins_Width, coins_height, &coins, 32 * count, 9 * 32, SRCAND);
		putimage(coinX, coinY, coins_Width, coins_height, &coins, 32 * count, 8 * 32, SRCPAINT);

	//画出得分特效，如果鸟碰到了金币，就显示得分
	if (checkCoins()) {
		putimage(BirdX+1, BirdY-15, coins_Width, coins_height, &coins, 32 * count, 10 * 32, SRCPAINT);
		
		Sleep(30);
	}
	count++;
	
	EndBatchDraw();

	
	
}

void checkObject() {
	//实现功能：若鸟的位置超出了下界，就让其复位到对应X轴的最高点
	if (BirdY > height) {
		BirdY = 0;
	}
	//判断障碍物移动到最左侧，让其重画在最右侧
	if (Obstacle1X < -pipoWidth) {
		Obstacle1X = width;
	}

	if (Obstacle3X < -pipoWidth) {
		Obstacle3X = width;
	}

	if (Obstacle2X < -pipoWidth) {
		Obstacle2X = width;
	}

	if (Obstacle4X < -pipoWidth) {
		Obstacle4X = width;
	}
	//若金币超出了界限就随机生成位置
	if (coinX < -pipoWidth) {
		coinX = rand()% width;
		coinY = rand()% width;
		punishPoints++;
	}
}
//检查是否碰撞到金币，同时不断重播碰撞音乐
int checkCoins() {
	if (BirdX + Bird_Width >= coinX && BirdX <= coinX + coins_Width &&BirdY+Bird_height>=coinY &&BirdY<coinY+coins_height) {
		coinX = rand() % width;
		coinY = rand() % width;
		//碰到金币就播放音乐
		mciSendString(_T("close coinMusic"), NULL, 0, NULL);
		mciSendString(_T("open D:/Fang/FlyingBird/sound/sound/coins.mp3 alias coinMusic"), NULL, 0, NULL);
		mciSendString(_T("play coinMusic"), NULL, 0, NULL);
		//并让得分不断增加
		Points++;
		if (Points == 5&&playerChoice!=4) {//如果得分达到5，且不是无尽模式，就跳转到胜利页面
			drawWinView();
		}
		return 1;
	}
	return 0;
}

//加载全部音乐
void loadAllMusic() {
	mciSendString(_T("open D:/Fang/FlyingBird/sound/sound/menu.mp3 alias bgMusic"), NULL, 0, NULL);
	mciSendString(_T("open D:/Fang/FlyingBird/sound/sound/dead.mp3 alias deadMusic"), NULL, 0, NULL);
	mciSendString(_T("open D:/Fang/FlyingBird/sound/sound/coins.mp3 alias coinMusic"), NULL, 0, NULL);
	mciSendString(_T("open D:/Fang/FlyingBird/sound/sound/jump.mp3 alias jumpMusic"), NULL, 0, NULL);
	
	mciSendString(_T("play bgMusic repeat"), NULL, 0, NULL);
}


void gameHome() {
	
	//绘制游戏主界面
	drawMain();
	//菜单选择函数调用
	handleMenuItem();
	//开始游戏
	PlayGame();
}

//按键实现菜单切换
void handleMenuItem() {
	while (true) {
		BeginBatchDraw();
		
		if (GetAsyncKeyState(VK_F1)) {
			
			setfillcolor(RED);//设置画笔颜色
			drawText(rect1, _T("开始游戏"));
			setfillcolor(BLUE);//设置画笔颜色
			drawText(rect2, _T("游戏详情"));
			drawText(rect3, _T("退出游戏"));
			drawText(rect6, _T("无尽模式"));
			playerChoice = 1;
			
		}

		if (GetAsyncKeyState(VK_F2)) {
			setfillcolor(RED);//设置画笔颜色
			drawText(rect2, _T("游戏详情"));
			setfillcolor(BLUE);//设置画笔颜色
			drawText(rect1, _T("开始游戏"));
			drawText(rect3, _T("退出游戏"));
			drawText(rect6, _T("无尽模式"));
			playerChoice = 2;
			
		}

		if (GetAsyncKeyState(VK_F3)) {
			setfillcolor(RED);//设置画笔颜色
			drawText(rect3, _T("退出游戏"));
			setfillcolor(BLUE);//设置画笔颜色
			drawText(rect1, _T("开始游戏"));
			drawText(rect2, _T("游戏详情"));
			drawText(rect6, _T("无尽模式"));
			playerChoice = 3;	
		}

		if (GetAsyncKeyState(VK_F4)) {
			setfillcolor(RED);//设置画笔颜色
			drawText(rect6, _T("无尽模式"));
			setfillcolor(BLUE);//设置画笔颜色
			drawText(rect1, _T("开始游戏"));
			drawText(rect2, _T("游戏详情"));
			drawText(rect3, _T("退出游戏"));
			playerChoice = 4;
		}
		EndBatchDraw();
		if (GetAsyncKeyState(VK_RETURN)) {
			switch (playerChoice) {
				//开始游戏
			case 1:
				return;
				//跳转详情界面
			case 2:
				drawInfoView();
				break;
			case 3:
				exit(0);
				break;
			case 4:
				return;
			default:
				break;
			}
		}
		
	}
	
}

//画主界面函数
void drawMain() {
	BeginBatchDraw();
	putimage(0, 0, &bgImages);
	setbkmode(TRANSPARENT);  // 在设置字体之前设置背景：透明的,而不是黑色
	settextstyle(40, 0, _T("方正喵呜简体"));
	outtextxy(200, 30, _T("飞扬的小鸟"));
	setfillcolor(BLUE);//设置画笔颜色
	drawText(rect1, _T("开始游戏"));
	drawText(rect6, _T("无尽模式"));
	drawText(rect2, _T("游戏详情"));
	drawText(rect3, _T("退出游戏"));
	EndBatchDraw();
}

//封装菜单界面绘制函数
void drawText(RECT rect, LPCTSTR str) {
	rectangle(rect.left, rect.top, rect.right, rect.bottom);//绘制矩形3
	fillrectangle(rect.left, rect.top, rect.right, rect.bottom);//绘制矩形
	drawtext(str, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

//画鸟碰到障碍物后返回主菜单
void drawReturnMain() {
	putimage(0, 0, &bgImages);
	//游戏结束背景图
	putimage(width/3-30, height/2, &GameOverImgBg, NOTSRCERASE);
	putimage(width/3-30, height/2, &GameOverImg, SRCINVERT);
	int second = 2;
	while (true) {
		BeginBatchDraw();
		setfillcolor(BLUE);//设置画笔颜色
		char str[100];
		sprintf_s(str, "%d", second);
		drawText(rect4, _T("秒后返回主菜单"));
		outtextxy(180,210, *str);
		second--;
		EndBatchDraw();
		Sleep(1000);
		if (second == 0) {
			break;
		}
	}
	gameHome();
}

//开始游戏
void PlayGame() {

	//重置游戏的各个对象的基本属性
	resetGame();
	//利用循环阻止程序结束
	while (true) {

		//障碍物移动和鸟自由下落
		removeObj();
		
		

		//按键监听，小鸟空格跳跃功能
		keyPress();

		//画障碍物
		drawObject();

		//检查鸟和障碍物是否到达边界
		checkObject();

		//检查障碍物碰撞
		if (checkObstacles()) {
			drawReturnMain();
		}
		Sleep(40);
	}
	
}

void drawWinView() {
	putimage(0, 0, &bgImages);
	outtextxy(width/3, width/3, _T("恭喜你，获胜了"));
	outtextxy(width/4, height/2, _T("按下ESC键回到主菜单"));
	while (true) {
		if (GetAsyncKeyState(VK_ESCAPE)) {
			break;
		}
	}
	gameHome();
}
void drawInfoView() {
	putimage(0, 0, &gamerule);
	outtextxy(0, 0, _T("按下ESC键返回"));
	while (true) {
		if (GetAsyncKeyState(VK_ESCAPE)) {
			break;
		}
	}
	gameHome();
}

//为了能够重新开始游戏，故将各种值重置
void resetGame() {
	 Obstacle1X = 400;
	 Obstacle2X = 350;
	 Obstacle3X = 150;
	 Obstacle4X = 200;
	 Obstacle1Y = -300;
	 Obstacle2Y = 500;
	 Obstacle3Y = 600;
	 Obstacle4Y = -550;
	 removePoints = 5;

	//用于计数金币旋转
	 count = 0;
	 coinX = 300;
	 coinY = 300;


	//鸟的X,Y坐标
	 BirdX = 100;
	 BirdY = 100;

	 //得分归零
	 Points = 0;
}
#include <stdio.h>
//#include "Console.h"
#include <windows.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#define ScreenWIDTH  40
#define ScreenHEIGHT 30
//define 정의를 하겠다라는 의미 코드에서 숫자로 쓰는것보다 해석하기 편하니까 보기편함. 
//#는 지시어  
enum ConsolColor{ BLACK, DARKBLUE, DARKGREEN,
    DARKCYAN, DARKRED, DARKMAGENTA, DARKYELLOW,
    GRAY, DARKGRAY, BLUE, GREEN, CYAN,
    RED, MAGENTA, YELLOW, WHITE, LIGHTGRAY = 7,
    ORIGINAL = 7, ORIGINALFONT = 7, ORIGINALBG = 0
}; 
//초기화면 제작 
void init(int ScrWidth,int ScrHeight) {
   char temp[40];
   sprintf(temp,"mode con cols=%d lines=%d | title 꼬리키우기",
                 ScrWidth,ScrHeight);
   system(temp); 
}


void setColor(int color) {
   // 텍스트 색상 변경
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (info.wAttributes&0xf0) | (color &0xf));
}
//문자열을 움직이는 역할 
void gotoxy(int x,int y) {
   HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
   COORD pos;
   pos.X = x;
   pos.Y = y;
   SetConsoleCursorPosition(consoleHandle,pos);
}
//커서삭제 
void noCursor() {   
    CONSOLE_CURSOR_INFO CurInfo;
    CurInfo.dwSize=1;
    CurInfo.bVisible=FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&CurInfo);
}

void consoleCls() {
   HANDLE hConsole;
   SMALL_RECT scrollRect;
   COORD scrollTarget;
   CHAR_INFO fill;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    scrollRect.Left = 0;
    scrollRect.Top = 0;
    scrollRect.Right = csbi.dwSize.X;
    scrollRect.Bottom = csbi.dwSize.Y;
    scrollTarget.X = 0;
    scrollTarget.Y = (SHORT)(0 - csbi.dwSize.Y);

    //버퍼의 기본 텍스트 속성으로 빈 공간을 채움
    fill.Char.UnicodeChar = TEXT(' ');
    fill.Attributes = csbi.wAttributes;

    // 스크롤 함
    ScrollConsoleScreenBuffer(hConsole, &scrollRect, NULL, scrollTarget, &fill);   
}


char scr[ScreenWIDTH][ScreenHEIGHT]={0,};

void clearScreen() {
   int i;
   int j;
   for(i=0;i<ScreenWIDTH;i++) 
      for(j=0;j<ScreenHEIGHT;j++)
         scr[i][j]=0;
   //system("cls");
   consoleCls();
}

void draw(int x,int y,int color) {
   scr[x][y]=color;
}


void renderScreen() {
   int i;
   int j;
   for(i=0;i<ScreenWIDTH;i++) {
      for(j=0;j<ScreenHEIGHT;j++) {      
         if(scr[i][j]!=0) {
            setColor(scr[i][j]);
            gotoxy(i,j);
            printf("*");
         }
      }
   }
}

//랜덤값 생성함수 
int randomnumW(){
    srand(time(NULL));
    return (rand() % ScreenWIDTH) ;
}
int randomnumH(){
    srand(time(NULL));
    return (rand() % ScreenHEIGHT) ;
}


//꼬리 충돌  
int checkCollision(int x, int y, int arrx[], int arry[], int key) {
	int i;
    for (i = 1; i < key; i++) { // i = 1 인 이유는 자기 자신(선두)는 제
        if (arrx[i] == x && arry[i] == y) {
            return 1; // 충돌 발생
        }
    }
    return 0;
}




main() {	
   int x=15,k,y=15,key;
   int moveVecter=1/*움직이는 방향*/,timer=0,gamelevel; 
   int keep=1;
   int arrx[99];
   int arry[99];
   int i,j;
    // 랜덤한 숫자 생성
    int numx = randomnumW(); //0~29
	int numy = randomnumH(); //0~29
    // 생성된 숫자 출력

   init(ScreenWIDTH,ScreenHEIGHT);
   noCursor();
	printf("게임 난이도 0 어려움 1쉬움\n");
	scanf("%d",&gamelevel);
  	while (keep) {
    	clearScreen();  
    	// 객체를 이동시키는 로직 추가
    	
		if (GetAsyncKeyState(VK_LEFT) & 0x8000) { // 왼쪽 화살표 키를 누르면
        	if (x > 0){
				 moveVecter=1; // x 값을 감소시켜서 왼쪽으로 이동
   		 	}
   		}
    	
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000) { // 오른쪽 화살표 키를 누르면
        	if (x < ScreenWIDTH - 1){
				 moveVecter=2; // x 값을 증가시켜서 오른쪽으로 이동
    		}
    	}
    	
		if (GetAsyncKeyState(VK_UP) & 0x8000) { // 위쪽 화살표 키를 누르면
        	if (y > 0){ 
				moveVecter=3; // y 값을 감소시켜서 위쪽으로 이동
    		}
    	}
    	if (GetAsyncKeyState(VK_DOWN) & 0x8000) { // 아래쪽 화살표 키를 누르면
        	if (y < ScreenHEIGHT - 1){
				 moveVecter=4; // y 값을 증가시켜서 아래쪽으로 이동
   			 }
   		}
	    if(GetAsyncKeyState(VK_SPACE) & 0x8000){
    		break;//그냥종료 
		}
	
		switch(moveVecter){
		case 1:
			if (x > 0){
	 			x--;
			}
		break;	
		case 2:
			if (x < ScreenWIDTH - 1) {
				x++;
			}
		break;
		case 3:
			if (y > 0){
				y--;
			}
		break;
		case 4:
			if (y < ScreenHEIGHT - 1){
				y++;
			}
		break;
		default:
		break;	
    	}
	
		if(timer>=150){
			break;
		}
	
		if(x== numx && y==numy){
			key++;
			timer=0; 
    		// 랜덤한 숫자 생성
    		numx =randomnumW(); //0~39
			numy =randomnumH(); //0~29
		}
	
		if(key ==0){
			arrx[0]=x;
			arry[0]=y;	
		}else if(key !=0){
			for(k=key;k>=0;k--){
				if(k==0){
					arrx[k]=x;
					arry[k]=y;
				}else{
					arrx[k]=arrx[k-1];
					arry[k]=arry[k-1];
				}
    			draw(arrx[k],arry[k], GREEN); // 꼬리 따라오는거 그리기  
			}
		}
		

		
    	draw(x, y, GREEN); // 새로운 위치에 객체 그림
		draw(numx, numy, YELLOW);//먹어야하는 별 
		timer++;//시간 
    	renderScreen();
    	Sleep(20);
    	if(gamelevel==1){
    		Sleep(80);//80ms딜레이 
		} 
		if (checkCollision(x, y, arrx, arry, key)) { //1은 true  0은 false 
            printf("자신의 꼬리에 닿았습니다! 게임 종료\n");
            keep = 0;
        }

	}
   printf("시간초과\n");
   printf("점수 = %d \n",key);
   
   printf("Game Over");
	sleep(5);
}

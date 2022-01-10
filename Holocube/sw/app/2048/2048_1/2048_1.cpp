#define _CRT_SCURE_NO_WARNINGS //防止编译器内扩增问题，in-complier amplification
#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h> // keydown
#include<graphics.h> // graphics lib
#include <time.h>
#include <iostream>
using namespace std;
/*image lib*/
int map[4][4]; //bottom board
IMAGE img[12]; // store 12 images of game number
int imgIndex[12] = {0,2,4,8,16,32,64,128,256,512,1024,2048};


/*here load all the images in order to be used in the future*/
void LoadResource()
{
	for(int i =0; i<12; i++)
	{
		char fileName[20] = "";
		sprintf(fileName, "%d.png", imgIndex[i]);
		loadimage(img + i,fileName);
	}
}

void drawMap()
{   
	setbkcolor(RGB(244, 215, 215)); // set background color
	cleardevice();
	settextcolor(BLUE);
	settextstyle(20, 0, "times");
	outtextxy(50, 10, "Let's start the puzzle");
	//draw the graphs by using array
	/*here build the x axis and y axis*/
	int x, y, k;
	for(int i =0; i<4; i++)
	{
		for(int j = 0; j<4; j++)
		{
			x = 60*j;  // width
			y = 60* i +60;  //height of images(background)
			for(k=0; k <12; k++)
			{
				if(imgIndex[k]== map[i][j])
					break;
			}
			putimage(x,y, img +k);//put image in corresponding location
		} 
	}
}

/*random function which produce 0,2,4...*/
int randIntNum()
{ 
	srand((unsigned int)time(NULL)); //seed
	for(int i=0; i< 4; i++)
	{
		for(int j =0; j<4; j++)
		{   
			if(map[i][j]==0) // anyplace the value is 0 then it produce 2...
			{
			map[i][j] =(rand()%3)*2;// x%3: 0,1,2; (x%3)*2 =0,2,4
			if(map[i][j]==0) //if it is still 0, it does not count
			   continue;
			return 0;
			}
				
		}
	}
	return 0;
}

/*same idea, but it moves along row(i) in (i,j)*/
int moveUp()
{
	int flag = 0;
	for(int i = 0; i< 4; i++)
	{
		for(int j = 4-1; j >=0; j--)
		{   
			int curKey = map[i][j];
			if(curKey !=0)
			{
			//int curKey = map[i][j];
			int k = i+1;
			while(k <4)
			 {  
               int curKeyNext = map[k][i]; // next element of current element
				if(curKeyNext != 0)
				{
					if(curKeyNext == curKey)
					{
						flag = 1;
						map[i][j] +=map[k][i]; //[2,2,4,4]--->[0,4,0,8]
						map[k][i] = 0;// need to change?
					}
					k = 4;
					break;// outof loop
				}
				k++; // check other positions
			 }
            
			}	
		}
	}
	//[0,2,0,8]--->[0,0,2,8]
	for(int i=0; i<4-1; i++)
	{
		for(int j=0; j<4; j++)
		{
			int curKey = map[i][j];
			if(curKey ==0)
			{
				int k = i+1;
				while(k<4)
				{
					int curKeyNext = map[k][j];
					if (curKeyNext !=0)
					{
						flag = 1;
						map[i][j] = curKeyNext;// move the nonzero to the postion of zero
						map[k][j] = 0; // change to zero after moving 
						k = 4;
					}
					k++;
				}
			}
		}
	}
	if(flag)
		return 0;
	else
		return 4; // end flag set as 4
}

/*move down*/
int moveDown()
{ 
 int flag = 0;
 for(int i = 4-1; i>=0; i--)
 {
	 for(int j=0; j<4; j++)
	 {
		 int curKey = map[i][j];
		 if(curKey !=0)
		 {
			 int k = i-1;
			 while(k>=0)
			 {
				 int curKeyNext = map[k][j];
				 if(curKeyNext !=0)
				 {
					 if(map[i][j]== map[k][j])
					 {
						 flag = 1; 
						 map[i][j] += map[k][j];
						 map[k][j] =0;
					 }
					 k =0;
					 break;
				 }
				 k--;
			 }
		 }
	 }
 }
 for(int i =4-1; i>0; i--)
 {
	 for(int j =0; j<4;j++)
	 {
		 int curKey = map[i][j];
		 if(curKey ==0)
		 {
			 int k = i-1;
			 while(k >=0)
			 { 
				 int curKeyNext = map[k][j];
				 if(curKeyNext !=0)
				 {
					 flag = 1;
					 map[i][j] = curKeyNext;
					 map[k][j] = 0;
					 k =0;
				 }
				 k--;
			 }

		 }
	 }
 }
 if(flag)
   return 0;
 else 
	 return 4;
}

/*moveLeft: sysmmetric to the moveRight*/
int moveLeft()
{
	int flag = 0;
	for(int i = 0; i< 4; i++)
	{
		for(int j = 0; j <4; j++)
		{
			int curKey = map[i][j];
			if(curKey !=0)
			{
				int k =j +1;
				while(k <4)
				 {
					int curKeyNext = map[i][k]; // next element of current element
					if(curKeyNext != 0)
					{
						if(curKeyNext == curKey)
						{
							flag = 1;
							map[i][j] +=map[i][k]; //[2,2,4,4]--->[0,0,4,8]
							map[i][k] = 0;
						}
						k = 4;
						break;// outof loop
					}
					k++; // check other positions
				 }
			}
		}
	}
	//[0,2,0,8]--->[0,0,2,8]
	for(int i=0; i<4; i++)
	{
		for(int j=0; j<4; j++)
		{
			int curKey = map[i][j];
			if(curKey ==0)
			{
				int k = j+1;
				while(k<4)
				{
					int curKeyNext = map[i][k];
					if (curKeyNext !=0)
					{
						flag = 1;
						map[i][j] = curKeyNext;// move the nonzero to the postion of zero
						map[i][k] = 0; // change to zero after moving 
						k = 4;
					}
					k++;
				}
			}
		}
	}
	if(flag)
		return 0;
	else
		return 4; //end flag set as 4
}
/*
merge according to differnt keys
right key: [2,2,4,4]--->[0,4,0,8]/[0,0,8,4]
same idea, but it moves along colomn(i) in (i,j)
*/
int moveRight()
{
	int flag = 0;
	for(int i = 0; i< 4; i++)
	{
		for(int j = 4-1; j >=0; j--)
		{   
			int curKey = map[i][j];
			if(curKey !=0)
			{
				int k =j -1;
			while(k >=0)
			 {
				int curKeyNext = map[i][k]; // next element of current element
				if(curKeyNext != 0)
				{
					if(curKeyNext == curKey)
					{
						flag = 1;
						map[i][j] +=map[i][k]; //[2,2,4,4]--->[0,4,0,8]
						map[i][k] = 0;
					}
					k = -1;
					break;// outof loop
				}
				k--; // check other positions
			 }
            
			}	
		}
	}
	//[0,2,0,8]--->[0,0,2,8]
	for(int i=0; i<4; i++)
	{
		for(int j=4-1; j>=0; j--)
		{
			int curKey = map[i][j];
			if(curKey ==0)
			{
				int k = j-1;
				while(k>=0)
				{
					int curKeyNext = map[i][k];
					if (curKeyNext !=0)
					{
						flag = 1;
						map[i][j] = curKeyNext;// move the nonzero to the postion of zero
						map[i][k] = 0; // change to zero after moving 
						k = -1;
					}
					k--;
				}
			}
		}
	}
	if(flag)
		return 0;
	else
		return 4; // end flag set as 4
}
void Replay()
{  

	for(int i =0; i<4; i++ )
	{
		for(int j =0; j<4; j++ )
		{
			map[i][j] =0;
		}
	}

}


/*key define(holocubic by using gyscope)*/
void keyPress()
{   
	IMAGE background; 
	//initgraph(60*4, 60*4+60); //terminal size: width, height
	char key = _getch();
	switch (key)
	{
	 case 'w':
	 case 'W':
	 case 72:
		randIntNum(); // press buttom, then produce different numbers
		moveUp();
		break;
	 case 's':
	 case 'S':
	 case 80:
		randIntNum();
		moveDown();
		break;
	 case 'a':
     case 'A':
	 case 75:
		randIntNum();
		moveLeft();
		break;
	 case 'd':
	 case 'D':
	 case 77:
		randIntNum();
		//drawMap();
		moveRight();
		break;
	 case 'q':
	 case 'Q':
		  loadimage(&background ,_T("one.png"), 60*4, 60*4+60); //normally there is no '_T'
          putimage(0,0,&background);
		  Sleep(1000);
		  //getchar();
		 // closegraph();
		  Replay();
		break;

	}

}
int main()
{ 
   char Password[] = "password: hello_esp32";
   IMAGE background; 
   LoadResource();
   initgraph(60*4, 60*4+60); //terminal size: width, height
   while(1)
   {
	  drawMap();
	  keyPress();
	  for(int i =0; i<4; i++ )
	 {
		for(int j =0; j<4; j++ )
		{
			if(map[i][j]==32)
			{
			cleardevice();
			settextcolor(RGB(127,255,0));
			settextstyle(20, 0, "times");
			setbkmode(TRANSPARENT);
			setbkmode(DT_CENTER);
			loadimage(&background ,_T("tu.jpg"), 60*4, 60*4+60); //normally there is no '_T'
            putimage(0,0,&background);
			 outtextxy(40, 10, Password);
			 getchar();
			}
		}
	  }
	  
   }
   getchar(); // avoid splash screen
   closegraph();
   system("pause");
   return 0;
}
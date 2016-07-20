#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include<stdio.h>
#include"zrcar.c"
#include"zrcar.h"
#include<iostream>
using namespace std;
using namespace cv;


void car_stop();
void car_back();
void car_forward();
void car_turn();

int main()
{
	int r,g,b;
	printf("hello world");
	int count=0;
    VideoCapture cap;
	cap.open(0);
	zrcar_wheel_init(ZRCAR_WHEEL_DEV);
	zrcar_opt_init(ZRCAR_OPTICAL_DEV);
	zrcar_ultrasonic_init(ZRCAR_ULTRASONIC_DEV);
/*	Mat frame=imread("mouse.jpg");
	if(!sign.data)
		cout<<"读取图片出现出错";
	cout<<"rows:"<<sign.rows<<"\t"<<"cols"<<sign.cols;*/
    
	while(1)
	{
	
	car_forward();
	count=0;
	Mat frame;  //定义一个Mat变量，用于存储每一帧的图像
	cap>>(frame);  //读取当前帧


	for(int row=0;row<frame.rows;row+=10)
	{
	
		for(int col=0;col<frame.cols;col+=10)
			{
				b=frame.at<Vec3b>(row,col)[0];
				g=frame.at<Vec3b>(row,col)[1];
				r=frame.at<Vec3b>(row,col)[2];
		//	cout<<"R:"<<r<<"  G:"<<g<<"  B:"<<b<<endl;
				//cout<<x<<"\t"<<y;

				if(r<80&&g<80&&b<100)
				{
                  	cout<<"R:"<<r<<"  G:"<<g<<"  B:"<<b<<endl;
					count++;
					//return ;
				}
				
			}
		
	}

	if(count>5)
	{
 	car_stop();
	car_forward();
	usleep(1000000);
	car_turn();
	usleep(4000000);
	car_stop();
    return 0;
	}

	}//end while
}//end main


void car_stop()
{
	zrcar_wheel_l_set(0);
	zrcar_wheel_r_set(0);
}
void car_forward()
{
	zrcar_wheel_l_set(15);
	zrcar_wheel_r_set(15);
}
void car_back()
{
	zrcar_wheel_l_set(-10);
	zrcar_wheel_r_set(-10);
}
void car_turn()
{
	zrcar_wheel_l_set(-6);
	zrcar_wheel_r_set(-15);
}
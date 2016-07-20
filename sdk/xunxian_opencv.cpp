#include "zrcam.h"
#include"zrcar.c"
#include"zrcar.h"
#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

void  car_stop();
void car_forward();
void car_back();
void car_left();
void car_right();

int main()
{
    Mat srcImage,midImage,dstImage;
    VideoCapture cap;
	vector<Vec4i> lines;//定义一个矢量结构lines用于存放得到的线段矢量集合
	int x1,y1,x2,y2,i;
	float k,k_average=0.0;
	int straight=0;
	int count;

	zrcar_wheel_init(ZRCAR_WHEEL_DEV);
	zrcar_opt_init(ZRCAR_OPTICAL_DEV);
	zrcar_ultrasonic_init(ZRCAR_ULTRASONIC_DEV);

    cap.open(0);
    car_forward();
	while(1)
	{
	count=0;
	k_average=0.0;
	straight=0;
	lines.clear();
 //   cap.read(srcImage);
	cap>>(srcImage);
	blur(srcImage,srcImage,Size(3,3));

	//【2】进行边缘检测和转化为灰度图
	Canny(srcImage, midImage, 50, 200, 3);//进行一此canny边缘检测
//	cvtColor(midImage,dstImage, CV_GRAY2BGR);//转化边缘检测后的图为灰度图

	//【3】进行霍夫线变换
	HoughLinesP(midImage, lines, 1, CV_PI/180, 80, 50, 10 );

	//【4】依次在图中绘制出每条线段
	for(  i = 0; i < lines.size(); i++ )
	{
		Vec4i l = lines[i];
		x1=l[0],y1=l[1],x2=l[2],y2=l[3];
		if((x2-x1)!=0)
		{
			k=(y2-y1)/(x2-x1+0.0);
			if(k>8||k<-8)
				straight++;
			else 
			{
				k_average+=k;
				count++;
			}
		}
		else 
			straight++;


//		cout<<"x"<<i<<":"<<l[0]<<"  y"<<i<<":"<<l[1]<<"     "<<"x"<<i<<":"<<l[2]<<"  y"<<i<<":"<<l[3]<<endl;
//		line( dstImage, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(186,88,255), 1, CV_AA);
	}
	if(count==0)
		car_forward();
	else
	{
		if(straight>3)
			car_forward();
		else
		{
			cout<<"k_average="<<k_average<<endl;
			if(k_average>0)
				car_left();
			if(k_average<0)
				car_right();
		}
	

	}//end else
	}//end while
}//end main()

void car_stop()
{
	zrcar_wheel_l_set(0);
	zrcar_wheel_r_set(0);
}
void car_forward()
{
	zrcar_wheel_l_set(8);
	zrcar_wheel_r_set(8);
}
void car_back()
{
	zrcar_wheel_l_set(-8);
	zrcar_wheel_r_set(-8);
}
void car_right()
{
	zrcar_wheel_l_set(8);
	zrcar_wheel_r_set(-8);

	
//	usleep(1950000);
}
void car_left()
{
	zrcar_wheel_l_set(-8);
	zrcar_wheel_r_set(8);
	usleep(100000);
	zrcar_wheel_l_set(0);
	zrcar_wheel_r_set(0);
//	usleep(1950000);
}
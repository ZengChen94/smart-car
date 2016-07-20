#include "zrcam.h"
#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>


using namespace std;
using namespace cv;

void  car_stop();
void car_forward();
void car_back();
void car_left();
void car_right();

int main()
{
    Mat srcImage;
    VideoCapture cap;
	vector<Vec4i> lines;//定义一个矢量结构lines用于存放得到的线段矢量集合
    cap.open(0);
    if(!cap.isOpened())
    printf("不能打开");
    car_forward();
	while(1)
	{
	
    cap.read(srcImage);
	blur(srcImage,srcImage,Size(3,3));

	//【2】进行边缘检测和转化为灰度图
	Canny(srcImage, midImage, 50, 150, 3);//进行一此canny边缘检测
	cvtColor(midImage,dstImage, CV_GRAY2BGR);//转化边缘检测后的图为灰度图

	//【3】进行霍夫线变换
	HoughLinesP(midImage, lines, 1, CV_PI/180, 40, 20, 10 );

	//【4】依次在图中绘制出每条线段
	for( size_t i = 0; i < lines.size(); i++ )
	{
		Vec4i l = lines[i];
		x1=l[0],y1=l[1],x2=l[2],y2=l[3];

//		cout<<"x"<<i<<":"<<l[0]<<"  y"<<i<<":"<<l[1]<<"     "<<"x"<<i<<":"<<l[2]<<"  y"<<i<<":"<<l[3]<<endl;
//		line( dstImage, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(186,88,255), 1, CV_AA);
	}
	}//end while
}//end main()

void car_stop()
{
	zrcar_wheel_l_set(0);
	zrcar_wheel_r_set(0);
}
void car_forward()
{
	zrcar_wheel_l_set(25);
	zrcar_wheel_r_set(25);
}
void car_back()
{
	zrcar_wheel_l_set(-25);
	zrcar_wheel_r_set(-25);
}
void car_right()
{
	zrcar_wheel_l_set(8);
	zrcar_wheel_r_set(-8);
	usleep(1950000);
}
void car_left()
{
	zrcar_wheel_l_set(-8);
	zrcar_wheel_r_set(8);
	usleep(1950000);
}
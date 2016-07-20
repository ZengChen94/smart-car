#include "zrcam.h"
#include <vector>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;
using namespace cv;

int DELAY_CAPTION = 1500;
int DELAY_BLUR = 100;
int MAX_KERNEL_LENGTH = 31;
RNG rng(12345);

int main()
{
	Mat src, src_gray, dst, elements,result;
	vector<vector<Point> > contours;
	Point rectCenter;
	Point heavy;
		
	int i = 0;
	char name_1[256] = "/root/_1.jpg";
	char name_2[256] = "/root/_2.jpg";
	char name_3[256] = "/root/_3.jpg";
	char name_4[256] = "/root/_4.jpg";
	char name_5[256] = "/root/_5.jpg";
	char name_6[256] = "/root/_6.jpg";
	char name_7[256] = "/root/_7.jpg";
	char name_8[256] = "/root/_8.jpg";
	
	src = imread("/root/2.jpg");  				//read the picture
	
	cvtColor( src, src_gray, CV_RGB2GRAY );			//set the gray
	imwrite(name_2,src_gray);
	
	elements = getStructuringElement(MORPH_RECT,Size(5,5));
	
	/// 使用中值平滑
	for ( int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2 )
		{ medianBlur ( src_gray, dst, i );}
	imwrite(name_3,dst);					
	
	threshold(dst,dst,70,255,0);		//set black and white
	imwrite(name_4,dst);
	
	Point center;
	center = Point(dst.rows/2,dst.cols/2);	//get the picture's center
	circle(dst,center,2,Scalar(100,100,100),2);
	//imwrite(name_6,dst);
	
	findContours(dst,contours,CV_RETR_LIST,CV_CHAIN_APPROX_NONE);//get the contours
	
    /// 计算矩
	vector<Moments> mu(contours.size() );
	for( int i = 0; i < contours.size(); i++ )
	{
		mu[i] = moments( contours[i], false ); 
	}
	
	///  计算中心矩	
	vector<Point2f> mc( contours.size() );
	for( int i = 0; i < contours.size(); i++ )
    { 
		mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );
		Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
		circle(dst,center,2,Scalar(100,100,100),2);
		//circle( dst, mc[i], 4, color, -1, 8, 0 );
	}
		//circle(dst,Point(mom.m10/mom.m00,mom.m01/mom.m00),2,Scalar(111,111,111),2);
		
	
	imwrite(name_7,dst);
	
	drawContours( dst, contours, -1, Scalar(255,255,255),2);
	imwrite(name_8,dst);
	
	cout<<"all:"<<contours.size()<<endl;
	
	return 0;
}

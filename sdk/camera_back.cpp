#include "zrcam.h"
#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>


using namespace std;
using namespace cv;

void car_back();
void car_forward();
void car_turn();
void car_stop();

int main()
{
Mat frame;
VideoCapture cap;
cap.open(0);
if(!cap.isOpened())
printf("不能打开");
CvScalar c;
IpImage * src=NULL;
int zhishi=0;
while(1)
 {
	car_forward();
    cap.read(frame);
	src=&(IpImage(frame));
	int redCount=0;
	for(int i=0;i<src->height/2;i++) {
		for(int j=0;j<src->width/2;j++) {
			s=cvGet2D(src,i,j);
			if((s.val[0]>=200)&&(s.val[1]<=50)&&(s.avl[2]<=50)) redCount++;
		}
	}
	if(redCount>10) 
	{
		car_stop();
	    car_turn();
	    usleep(2000000);
	    car_stop();
		break;
	}
    // imwrite("12.jpg",frame);
   //cap.read(frame);
     //imwrite("34.jpg",frame);
}
return 0;
}
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
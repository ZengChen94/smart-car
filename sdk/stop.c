#include <stdio.h>
#include<zrcar.c>
#include "zrcar.h"
void car_stop()
{
	zrcar_wheel_l_set(0);
	zrcar_wheel_r_set(0);
}
void car_forward()
{
	zrcar_wheel_l_set(-10);
	zrcar_wheel_r_set(-10);
}
void car_back()
{
	zrcar_wheel_l_set(10);
	zrcar_wheel_r_set(10);
}
void car_right_two()
{
	zrcar_wheel_l_set(8);
	zrcar_wheel_r_set(-8);
	usleep(1900000);
}
void car_left_two()
{
	zrcar_wheel_l_set(-8);
	zrcar_wheel_r_set(8);
	usleep(1900000);
}
int main()
{
	zrcar_wheel_init(ZRCAR_WHEEL_DEV);
	zrcar_opt_init(ZRCAR_OPTICAL_DEV);
	zrcar_ultrasonic_init(ZRCAR_ULTRASONIC_DEV);

 
car_stop();
//	car_left_two();
	//car_stop();
	return 0;
}

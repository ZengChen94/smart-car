#include <stdio.h>
#include"zrcar.c"
#include "zrcar.h"

void car_stop();
void car_forward();
void car_back();
void car_left();
void car_right();


int main()
{
	zrcar_wheel_init(ZRCAR_WHEEL_DEV);
	zrcar_opt_init(ZRCAR_OPTICAL_DEV);
	zrcar_ultrasonic_init(ZRCAR_ULTRASONIC_DEV);

	int optical_data;

	while(1)
	{
		/***************optical read**************/
		zrcar_opt_get_all(&optical_data);
		printf("%d\n",optical_data);
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
	zrcar_wheel_l_set(10);
	zrcar_wheel_r_set(10);
}
void car_back()
{
	zrcar_wheel_l_set(-10);
	zrcar_wheel_r_set(-10);
}
void car_right()
{
	zrcar_wheel_l_set(8);
	zrcar_wheel_r_set(-8);
}
void car_left()
{
	zrcar_wheel_l_set(-8);
	zrcar_wheel_r_set(8);
}

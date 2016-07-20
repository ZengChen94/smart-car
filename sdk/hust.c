#include <stdio.h>
#include"zrcar.c"
#include "zrcar.h"
#define T 1000000
void  car_stop();
void car_forward();
void car_back();
void car_left();
void car_right();

int main()
{
	zrcar_wheel_init(ZRCAR_WHEEL_DEV);
	zrcar_opt_init(ZRCAR_OPTICAL_DEV);
	zrcar_ultrasonic_init(ZRCAR_ULTRASONIC_DEV);

			car_forward();
			usleep(4*T);
                                                car_stop();

			car_back();
			usleep(2*T);
                            car_stop();

			car_right();
			car_forward();
			usleep(2*T);
                        car_stop();

                            
			car_right();
			car_forward();
			usleep(2*T);
			car_back();
			usleep(4*T);
                        car_stop();//h


			car_left();
			car_forward();
			usleep(T);
			car_right();
			car_forward();
			usleep(4*T);
			car_left();
			car_forward();
			usleep(2*T);
			car_left();
			car_forward();
			usleep(4*T);
			car_back();
			usleep(4*T);//u


			car_right();
			car_forward();
			usleep(3*T);

			car_left();
                            car_forward();
			usleep(2*T);

			car_left();
			car_forward();
			usleep(2*T);

			car_right();
			car_forward();
			usleep(2*T);

			car_right();
			car_forward();
			usleep(6*T);   //s

			car_back();
			usleep(2*T);
			car_right();
			car_forward();
			usleep(4*T);
                           
                            car_stop();


	return 0;	
}
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


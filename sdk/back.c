
#include <stdio.h>
#include"zrcar.c"
#include "zrcar.h"

void car_stop();
void car_back();
void car_forward();
void car_turn();
int main()
{
	 float ult_data[3];
	/*********init*******/
	zrcar_wheel_init(ZRCAR_WHEEL_DEV);
	zrcar_opt_init(ZRCAR_OPTICAL_DEV);
	zrcar_ultrasonic_init(ZRCAR_ULTRASONIC_DEV);
	zrcar_ultra_get_all(ult_data);

	while(ult_data[1]>280)
	{
         zrcar_ultra_get_all(ult_data);
		 car_forward();
	}

	car_stop();
	car_turn();
	usleep(2000000);
	car_stop();
    


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